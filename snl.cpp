#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Status Operators::SNL(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The left attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
  cout << "Algorithm: Simple NL Join" << endl;

	Status status;
	RID rid1;
	RID rid2;
	RID joined_rid;
	Record record1;
	Record record2;
	void* record_data = NULL;
	HeapFile* heap_file = NULL;
	HeapFileScan* heap_scan1 = NULL;
	HeapFileScan* heap_scan2 = NULL;
	
	try {

		heap_file = new HeapFile(result, status);
		if(!heap_file) throw INSUFMEM;
		if(status != OK) throw status;
		
		heap_scan1 = new HeapFileScan(attrDesc2.relName, status);
		if(!heap_scan1) throw INSUFMEM;
		if(status != OK) throw status;

		heap_scan2 = new HeapFileScan(attrDesc1.relName, status);
		if(!heap_scan2) throw INSUFMEM;
		if(status != OK) throw status;

		record_data = operator new(reclen);
		if(!record_data) throw INSUFMEM;
			
		while((status = heap_scan1->scanNext(rid1, record1)) == OK) {
		
			
			char *filter = ((char*)record1.data) + attrDesc2.attrOffset;
      
			// Start a new scan, this will reuse the same heapfile so 
			// it doesnt have to keep deconstructing the HeapFile Obj
			status = heap_scan2->startScan(
					attrDesc1.attrOffset,
					attrDesc1.attrLen,
					static_cast<Datatype>(attrDesc1.attrType),
					filter,
					op
			);
			if(status != OK) throw status;
			

			while((status = heap_scan2->scanNext(rid2, record2)) == OK) {

				
				int offset = 0;
				for(int i = 0; i < projCnt; ++i) {
					void* data = NULL;

					//printf("%s %s %d %d %d \n", attrDescArray[i].relName, attrDescArray[i].attrName, attrDescArray[i].attrOffset, attrDescArray[i].attrType, attrDescArray[i].attrLen);

					if(strcmp(attrDescArray[i].relName, attrDesc1.relName) == 0) {
						data = (char*)record2.data + attrDescArray[i].attrOffset;
					} else {
						data = (char*)record1.data + attrDescArray[i].attrOffset;
					}
					
					memcpy((char*)record_data + offset, data, attrDescArray[i].attrLen);
					offset += attrDescArray[i].attrLen;
				}
				
				Record joined_record = {record_data, reclen};
				status = heap_file->insertRecord(joined_record, joined_rid);
				if(status != OK) throw status;
			}
			if (status != FILEEOF) throw status;
			
			status = heap_scan2->endScan();
			if(status != OK) throw status;
			
		}
		if (status != FILEEOF) throw status;

		status = heap_scan1->endScan();
		if(status != OK) throw status;
		
		// no exceptions thrown, so status is OK
		status = OK;
	} catch (Status s) {
		status = s;
	}

	// Free memory
	if(record_data) operator delete(record_data);
	if(heap_file) delete heap_file;
	if(heap_scan1) delete heap_scan1;
	if(heap_scan2) delete heap_scan2;
	return status;
}