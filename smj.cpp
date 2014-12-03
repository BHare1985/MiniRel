#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include "string.h"

/* Consider using Operators::matchRec() defined in join.cpp
 * to compare records when joining the relations */
  
Status Operators::SMJ(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
    cout << "Algorithm: SM Join" << endl;
    
    //initializing variables used throughout function:
    Status status, status_1, status_2;
    string rel_name_1 = attrDesc1.relName;
    string rel_name_2 = attrDesc2.relName;    
    Record record_1, record_2;
    AttrDesc* rel0;
    AttrDesc* rel1;
    AttrDesc* rel2;
    
    int size_1=0, size_2=0, num=0; 
    
    //get k value:
    int k = 0.8 * bufMgr->numUnpinnedPages(); //how many pages in the buffer pool we can use to sort
    
    //get datatypes
    Datatype type_1 = static_cast<Datatype>(attrDesc1.attrType); //datatype attr1
    Datatype type_2 = static_cast<Datatype>(attrDesc2.attrType); //datatype attr2
    
    //get the max # of tuples for each relation
    status = attrCat->getRelInfo(result, num, rel0); 
    if (status != OK) return status;
    
    status = attrCat->getRelInfo(attrDesc1.relName, num, rel1);
    if (status != OK) return status;
    for (int i=0; i < num; i++) {
    	size_1 = size_1 + rel1[i].attrLen;
    }

    status = attrCat->getRelInfo(attrDesc2.relName, num, rel2);
    if (status != OK) return status; 
    for(int i=0; i < num; i++) {
    	size_2 = size_2 + rel2[i].attrLen;
    }
    
    //open rel 1 sorted file
    SortedFile file_1(rel_name_1, attrDesc1.attrOffset, attrDesc1.attrLen, type_1, (k * 1024) / size_1, status);
    if(status != OK) return status;

    k = .8 * bufMgr->numUnpinnedPages(); //recalculate k value

    //open rel 2 sorted file
    SortedFile file_2(rel_name_2, attrDesc2.attrOffset, attrDesc2.attrLen, type_2, (k * 1024) / size_2, status);
    
    if(status != OK) return status;

    //open result heap file to insert into later
    HeapFile heap_file(result, status);
    if(status != OK) return status;

    //scan through the sorted files now:
    status_1 = file_1.next(record_1);
    status_2 = file_2.next(record_2);
    
    //variables used for when a mark is used so that more reads/writes are not used
    int compareNum = 0;
    bool goToMark = false;
    int numEqual = 0;
    
    while((status_1 == OK) && (status_2 == OK)) {
        
        //increment smaller if records not equal:
        if(goToMark == false) compareNum = matchRec(record_1, record_2, attrDesc1, attrDesc2);
        //this is so compareNum doesn't have to be recalculated
        
        if(compareNum > 0) status_2 = file_2.next(record_2);
        
        else if(compareNum < 0) status_1 = file_1.next(record_1);
        
        //otherwise, set a mark in file 2
        else {
            Record tmp_record = record_2;
	        file_2.setMark();
	       
	        //insert to the result heap file while the records equal
            
            
            if(goToMark == true){
                for(int i = 0; i < numEqual; ++i){
                    int offset_num = 0; //shows where in the result table you are
                    char *rec_data = new char[reclen]; //Making char pointer array for the data to be stored as 
                    Record rec = {rec_data, reclen}; //Initializing the record

                    //Same projection as other files
                    for(int i=0; i<projCnt; i++) {
	                    	                
	                    int bytes = attrDescArray[i].attrLen; //number of bytes to allocate
	                    char *new_place = offset_num + rec_data; //char pointer to new address for memory
	                    
	                    char *place = NULL; //char pointer to the beginning of the column
	                    if(attrDescArray[i].relName == rel_name_1) {
	                        place = ((char*)record_1.data) + attrDescArray[i].attrOffset;
	                    }
	                    else {
	                        place = ((char*)record_2.data) + attrDescArray[i].attrOffset;
	                    }

	                    memcpy(new_place, place, bytes); //copy memory over
	                    offset_num = offset_num + bytes; //remember where to start next memory copy
                    
                    }
                    
                    //insert result record
                    RID rec_rid;
                    status = heap_file.insertRecord(rec, rec_rid); //insert record into result heap file
                    delete [] rec_data; //data cleanup
                    if(status != OK) return status;

                    
	                status_2 = file_2.next(record_2); //get next record from file 2
	                if(status_2 != OK) break;                
                }
                
            } 
            
            else if(goToMark == false){
	            bool go = true;
	            numEqual = 0;
	            while(go){
                    ++numEqual;
                    int offset_num = 0; //shows where in the result table you are
                    char *rec_data = new char[reclen]; //Making char pointer array for the data to be stored as 
                    Record rec = {rec_data, reclen}; //Initializing the record

                    //Same projection as other files
                    for(int i=0; i<projCnt; i++) {
	                    	                
	                    int bytes = attrDescArray[i].attrLen; //number of bytes to allocate
	                    char *new_place = offset_num + rec_data; //char pointer to new address for memory
	                    
	                    char *place = NULL; //char pointer to the beginning of the column
	                    if(attrDescArray[i].relName == rel_name_1) {
	                        place = ((char*)record_1.data) + attrDescArray[i].attrOffset;
	                    }
	                    else {
	                        place = ((char*)record_2.data) + attrDescArray[i].attrOffset;
	                    }

	                    memcpy(new_place, place, bytes); //copy memory over
	                    offset_num = offset_num + bytes; //remember where to start next memory copy
                    
                    }
                    
                    //insert result record
                    RID rec_rid;
                    status = heap_file.insertRecord(rec, rec_rid); //insert record into result heap file
                    delete [] rec_data; //data cleanup
                    if(status != OK) return status;

	                status_2 = file_2.next(record_2); //get next record from file 2
	                if(status_2 != OK) break;
	                go = (matchRec(record_1, record_2, attrDesc1, attrDesc2) == 0);
	            } 
	        }
	        
	        
	        status_1 = file_1.next(record_1); //get next record from file 1
	        int matchRecNum = 1;
	        if(status_1 == OK){
	            matchRecNum = matchRec(record_1, tmp_record, attrDesc1, attrDesc2);
	        }
	        if(matchRecNum == 0 && (status_1 == OK)) {
	            //go back to the mark for file 2 if the new file 1 record is ==
	            //to the file 2 record where the mark was set
	            status = file_2.gotoMark();
	            if(status != OK) return status;
	            status_2 = file_2.next(record_2);
	            compareNum = matchRecNum;
	            goToMark = true;
	        } else{
	            goToMark = false;
	            numEqual = 0;
	        }
        }
    }
 
    return OK;
}

