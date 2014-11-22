#include "catalog.h"
#include "query.h"
#include "index.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Status Updates::Insert(const string& relation,      // Name of the relation
                       const int attrCnt,           // Number of attributes specified in INSERT statement
                       const attrInfo attrList[])   // Value of attributes specified in INSERT statement
{
	Status status;
	AttrDesc *attrs;
	Record record;
	RID rid;
	
	int real_attrCnt = 0;
	int record_size = 0;
	void* record_mem = NULL;

	try {
		HeapFile heap(relation, status);
		if(status != OK) throw status;

		
		// get attribute data
		status = attrCat->getRelInfo(relation, real_attrCnt, attrs);
		if(status != OK) throw status;

		// If the relational attr count isn't equal to attr count specified
		// This handles if no value is specified and # attr is right
		if (real_attrCnt != attrCnt) throw ATTRTYPEMISMATCH;


		// Get the size in bytes the record should be
		for (int i = 0; i < attrCnt; ++i) {
			record_size += attrs[i].attrLen;
		}

		// Memory allocation for record
		record_mem = malloc(record_size);
		if (record_mem == NULL) throw INSUFMEM;

		int inserted = 0;
		for (int i = 0; i < attrCnt; ++i) {
			for (int j = 0; j < attrCnt; ++j) {
				// Ensures in correct order according to attrs
				if(strcmp(attrList[j].attrName, attrs[i].attrName) == 0) {
					void* insert_location = (char*)record_mem + attrs[i].attrOffset;
					memcpy(insert_location, attrList[j].attrValue, attrs[i].attrLen);
					inserted++;
					break;
				}
			}
		}
		
		if(inserted != attrCnt) throw ATTRNOTFOUND;
		
		record.data = record_mem;
		record.length = record_size;

		status = heap.insertRecord(record, rid);
		if(status != OK) throw status;
		
		for (int i = 0; i < attrCnt; ++i) {
			if(attrs[i].indexed == false) continue;
			
			Index idx(
				relation,
				attrs[i].attrOffset,
				attrs[i].attrLen, 
				static_cast<Datatype>(attrs[i].attrType), // https://piazza.com/class/hzcwxwdhmhv5r0?cid=999
				NONUNIQUE, // not unique (https://piazza.com/class/hzcwxwdhmhv5r0?cid=995)
				status
			);
			if(status != OK) throw status;
			
			status = idx.insertEntry((char*)record.data + attrs[i].attrOffset, rid);
			if(status != OK) throw status;
		}
		
		// no exceptions thrown, so status is OK
		status = OK;
	} catch (Status s) {
		status = s;
	}
	
	// Free memory
	if(attrs) delete[] attrs;
	if(record_mem) free(record_mem);
	
	printf("========DEBUG=======\n");
	Utilities::Print(relation);
	printf("========DEBUG=======\n");

	return status;
}
