#include "catalog.h"
#include "query.h"
#include "index.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
In addition to inserting the tuple, this operator must also update all the hash indices on the relation. Use
the Index::insertEntry to insert an entry into the index. Look up the system catalogs to find out
information about the relation and attributes. (See Section “Getting Started” for more details.) 



*/

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
		
		for (int i = 0; i < attrCnt; ++i) {
			void* insert_location = (char*)record_mem + attrs[i].attrOffset;
			memcpy(insert_location, attrList[i].attrValue, attrs[i].attrLen);
		}
		
		record.data = record_mem;
		record.length = record_size;

		status = heap.insertRecord(record, rid);
		if(status != OK) throw status;
		
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
