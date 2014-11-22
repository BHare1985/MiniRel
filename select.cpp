#include "catalog.h"
#include "query.h"
#include "index.h"


/*
 * Selects records from the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

Status Operators::Select(const string & result,      // name of the output relation
				 const int projCnt,          // number of attributes in the projection
		         const attrInfo projNames[], // the list of projection attributes
		         const attrInfo *attr,       // attribute used inthe selection predicate 
		         const Operator op,         // predicate operation
		         const void *attrValue)     // literal value in the predicate
{
	Status status;
	AttrDesc* attrs = NULL;
	AttrDesc* projList = NULL;
	int record_length;
	bool conditional;
	bool equality;
	
	try {
		
		projList = new AttrDesc[projCnt];
		if(projList == NULL) throw INSUFMEM;
		
		// For each projection attrInfo in projNames, store attrDesc in projList
		for(int i = 0; i < projCnt; ++i) {
		   status = attrCat->getInfo(projNames[i].relName, projNames[i].attrName, projList[i]);
		   if(status != OK) throw status;
		}

		// get record length for Select functions
		for(int i = 0; i < projCnt; ++i) {
		   record_length += projList[i].attrLen;
		}

		conditional = (attr == NULL) ? false : true;
		equality = (op == EQ || op == NE) ? true : false;
		
		if(conditional) {
			attrs = new AttrDesc;
			status = attrCat->getInfo(attr->relName, attr->attrName, *attrs);
			if(status != OK) throw status;
		}
		
		
		if(conditional && attrs->indexed && equality) {
			status = IndexSelect(result, projCnt, projList, attrs, op, attrValue, record_length);
			if(status != OK) throw status;
		} else {
			status = ScanSelect(result, projCnt, projList, attrs, op, attrValue, record_length);
			if(status != OK) throw status;
		}
		
		// no exceptions thrown, so status is OK
		status = OK;
	} catch (Status s) {
		status = s;
	}
	
	// Free memory
	if(attrs) delete attrs;
	if(projList) delete[] projList;

	return status;
}

