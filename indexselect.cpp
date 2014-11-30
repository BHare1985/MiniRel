#include "catalog.h"
#include "query.h"
#include "index.h"
#include "string.h"

Status Operators::IndexSelect(const string& result,       // Name of the output relation
                              const int projCnt,          // Number of attributes in the projection
                              const AttrDesc projNames[], // Projection list (as AttrDesc)
                              const AttrDesc* attrDesc,   // Attribute in the selection predicate
                              const Operator op,          // Predicate operator
                              const void* attrValue,      // Pointer to the literal value in the predicate
                              const int reclen)           // Length of a tuple in the output relation
{

    cout << "Algorithm: Index Select" << endl;
    //Initializing vars to be used later
    Status status;
    string relName = projNames[0].relName;  
    RID rid;
    Record record;
    
    // Open the HeapFile for the Result Table
    HeapFile heap_file(result, status);
    if(status != OK) return status; //return if status is not ok 
    
    //Creating the index:
    Index index(relName, attrDesc->attrOffset, attrDesc->attrLen, 
        static_cast<Datatype>(attrDesc->attrType), 0, status); //0 indicates not making sure that unique

    
    if(status != OK) return status; //return if status is not ok
    index.startScan(attrValue); //starting the index scan w the value in the predicate
    
    //Creating a HeapFileScan to use later to get records:
    HeapFileScan heap_file_scan(relName, status);
    if(status != OK) return status; //return if status is not ok

  
    
    while(index.scanNext(rid) == OK) { //getting the next rid from the index
        heap_file_scan.getRandomRecord(rid, record); //getting a record from filescan using the rid just retrieved
        
        //same as scanselect:
        char *rec_data = new char[reclen];
        Record rec = {rec_data, reclen};
        int offset = 0;

        for(int i=0; i<projCnt; i++) {
            
            int bytes = projNames[i].attrLen; //length of the column retrieving
            char *place = ((char*)record.data) + projNames[i].attrOffset; //char pointer to the beginning of the column
            char *new_place = rec_data + offset; //char pointer to the resultdata

            memcpy(new_place, place, bytes); //copy column to result tuple
            offset = offset + bytes;
        
        }
        
        //Inserting record into heap_file
        RID rec_rid;
        status = heap_file.insertRecord(rec, rec_rid);
        delete [] rec_data;
        if(status != OK) return status; //return if status is not ok
    
    }
    
    return status;
}

