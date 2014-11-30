#include "catalog.h"
#include "query.h"
#include "index.h"
#include <string.h>

/* 
 * A simple scan select using a heap file scan
 */

Status Operators::ScanSelect(const string& result,       // Name of the output relation
                             const int projCnt,          // Number of attributes in the projection
                             const AttrDesc projNames[], // Projection list (as AttrDesc)
                             const AttrDesc* attrDesc,   // Attribute in the selection predicate
                             const Operator op,          // Predicate operator
                             const void* attrValue,      // Pointer to the literal value in the predicate
                             const int reclen)           // Length of a tuple in the result relation
{
    cout << "Algorithm: File Scan" << endl;
    

    ////MADDIE CODE:
  
    //initializing variables used later
    RID rid; //will be passed into function and changed, so doesn't need to be set.
    Record record;
    Status status; 
    HeapFileScan *heap_file_scan = NULL;
    string rel_name = projNames[0].relName;


    //initializing heap file scan.
    if(attrDesc == NULL){

        heap_file_scan = new HeapFileScan(rel_name, status); 

    } //Only choosing from one table, and no constraints
    else {

        heap_file_scan = new HeapFileScan(rel_name, attrDesc->attrOffset, attrDesc->attrLen,
				   static_cast<Datatype>(attrDesc->attrType), (char*)attrValue, op, status);

    } //choosing from one table, and a constraint

    //Return if not ok.
    if(status != OK) {
        if(heap_file_scan) delete heap_file_scan; //Memory clean-up
        return status; //return
    }  

    //Open heapfile for result table
    HeapFile heap_file(result, status);
    //Return if not ok.
    if(status != OK) {

        if(heap_file_scan) delete heap_file_scan; //Memory clean-up
        return status;
    }
    


    //scan through the heap file

    
    while(heap_file_scan->scanNext(rid, record) == OK) {//get each tuple from the relation

        //Making a record to insert into the result file.
        char *rec_data = new char[reclen]; //Making char pointer array for the data to be stored as  
       
        Record rec = {rec_data, reclen}; //Initializing the record
        
        int offset = 0; //shows where in the result record you are on.
        
        //for each column that you are supposed to be getting:
        for(int i=0; i<projCnt; i++) {
            char *new_place = rec_data + offset; //char pointer to the resultdata
            int bytes = projNames[i].attrLen; //length of the column retrieving
            
            char *place = ((char*)record.data) + projNames[i].attrOffset; //char pointer to the beginning of the column
            memcpy(new_place, place, bytes); //copy column to result tuple
            offset = offset + bytes;
        
        }
        
        /* Insert the Projected Record into the Result HeapFile */
        RID rec_rid; 
        status = heap_file.insertRecord(rec, rec_rid); 
        delete [] rec_data; //Memory clean-up. Necessary? 
        
        //Return if not ok.
        if(status != OK) {
            if(heap_file_scan) delete heap_file_scan; //Memory clean-up
            return status;
        }
    }
    
    if(heap_file_scan) delete heap_file_scan; //Memory clean-up
    //END MADDIE CODE

    return status;
}
