#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include "string.h"
/* 
 * Indexed nested loop evaluates joins with an index on the 
 * inner/right relation (attrDesc2)
 */

Status Operators::INL(const string& result,           // Name of the output relation
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // The projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // Length of a tuple in the output relation
{
    
    cout << "Algorithm: Indexed NL Join" << endl;
    
    Status status;   
    string rel_name_o;
    string rel_name_i;
    string attr_name_o;
    string attr_name_i;
    
    AttrDesc * rel_i = new AttrDesc;
    AttrDesc * rel_o = new AttrDesc;
    
    
    //NOT guaranteed which attr has an index from join.cpp
    if(attrDesc1.indexed && attrDesc2.indexed){
        rel_name_o = attrDesc2.relName;
        rel_name_i = attrDesc1.relName;
        attr_name_o = attrDesc2.attrName;
        attr_name_i = attrDesc1.attrName;         
        
    } //indexes on both, so arbitrarily pick one
    else if(attrDesc1.indexed){
        rel_name_o = attrDesc2.relName;
        rel_name_i = attrDesc1.relName;
        attr_name_o = attrDesc2.attrName;
        attr_name_i = attrDesc1.attrName;  
    } //index on attrDesc1
    else if(attrDesc2.indexed){
        rel_name_o = attrDesc1.relName;
        rel_name_i = attrDesc2.relName; 
        attr_name_o = attrDesc1.attrName;
        attr_name_i = attrDesc2.attrName;        

    } //index on attrDesc2
    
    attrCat->getInfo(rel_name_i, attr_name_i, *rel_i);
    attrCat->getInfo(rel_name_o, attr_name_o, *rel_o); 
       
    HeapFileScan file_scan_outer(rel_name_o, status); //choose rel w/o index to be outer
    
    if(status != OK) return status; //return if status is not ok

    //Open heapfile for later use
    HeapFile heap_file(result, status);
    if(status != OK) return status; //return if status is not ok  
  
    //Scan through the outer relation
    RID rid_o, rid_i;
    Record record_o, record_i;
    
    Datatype type = static_cast<Datatype>(rel_o->attrType);
    Index index_inner(rel_name_i, rel_i->attrOffset, rel_i->attrLen, type, 0, status); //choose rel1 arbitrarily to be inner
    if(status != OK) return status; //return if status is not ok  
    
    HeapFileScan file_scan_inner(rel_name_i, status);
    if(status != OK) return status; //return if status is not ok 
    
    while(file_scan_outer.scanNext(rid_o, record_o) == OK) { //get the next outer record
        // Open the Index on the Inner attribute (the one with the index)

       //file_scan_inner. need to start a scan? declaration used to go here.
            
        /* Start the Scan through the Matching Records Using the Index */
        char *value = new char[rel_i->attrLen];
        char *source = ((char*)record_o.data) + rel_i->attrOffset;
        memcpy(value, source, rel_i->attrLen); 
 

        index_inner.startScan(value);
        while(index_inner.scanNext(rid_i) == OK) {
	        
	        file_scan_inner.getRandomRecord(rid_i, record_i);
            char *rec_data = new char[reclen]; //Making char pointer array for the data to be stored as 
            Record rec = {rec_data, reclen}; //Initializing the record
            int offset = 0; //shows where in the result table you are
            
            //same projection routine as other files
            for(int i=0; i<projCnt; i++) {
                int bytes = attrDescArray[i].attrLen; //number of bytes to allocate
                char *new_place = rec_data + offset; //char pointer to new address for memory

                char *place = NULL; //char pointer to the beginning of the column
                if(attrDescArray[i].relName == rel_name_o){
                
                    place = ((char*)record_o.data) + attrDescArray[i].attrOffset;
                }
                else {
                    place = ((char*)record_i.data) + attrDescArray[i].attrOffset;
                }

                memcpy(new_place, place, bytes); //copy memory over
                offset = offset + bytes; //remember where in file you are for next insert
            
            }
            
            //Insert result record
            RID rec_rid;
            status = heap_file.insertRecord(rec, rec_rid);
            
            delete [] rec_data;
            if(status != OK) return status; //return if status is not ok 
                   
        }
        
        if(value) delete value; //data cleanup
        index_inner.endScan();
    }
    
    if(rel_i) delete rel_i; //data cleanup
    if(rel_o) delete rel_o; //data cleanup
    
    return status;
}

