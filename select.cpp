#include "catalog.h"
#include "query.h"
#include "index.h"
/*

This function implements the select operator, selecting all tuples that match the input predicate. In your
case, the predicate is specified via three variables: attr, op, and attrValue. (If attr is null, this means that
the selection is unconditional.) You must implement two access methods for the select operator: one using
a HeapFileScan and the other using an IndexScan. These access methods will be implemented as separate
functions called Operators::ScanSelect (in the file scanselect.cpp) and Operators::IndexSelect (in the
file indexselect.cpp).


The Operators::Select function must implement a very simple rule for “optimizing” select queries. It
must check if an index exists on the attribute in the predicate, and if the predicate is an equality predicate.
If both these conditions are met, it calls the IndexSelect function; otherwise it calls ScanSelect. This
criteria works since hash indices are generally very efficient for evaluating equality predicates.
The results of the selection are inserted into a result file (a HeapFile) called result, which is created by the
parser before calling Select. The names of the attributes of this relation are derived from the
corresponding attributes in projNames, and have as suffix the attribute number.


Projection, defined by projCnt and projames, should be done on the fly when each result tuple is being
written out. Don’t worry about eliminating duplicates during the projection.
Finally, the search value, attrValue, is a pointer to a value that has the same type as attr. For SQL data
types INTEGER, DOUBLE, CHAR, the attrValue points to the C++ types int&, double&, and char*
respectively.


*/
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
	AttrDesc* attrs;
	AttrDesc* projList = new AttrDesc[projCnt];
	
	bool conditional;
	int record_length;
	
	try {
		conditional = (attr == NULL) ? false : true;
		
		// For each projection attrInfo in projNames, store attrDesc in projList
		for(int i = 0; i < projCnt; ++i) {
		   status = attrCat->getInfo(projNames[i].relName, projNames[i].attrName, projList[i]);
		   if(status != OK) throw status;
		}

		// get record length for Select functions
		for(int i = 0; i < projCnt; ++i) {
		   record_length += projList[i].attrLen;
		}
		
		if(conditional) {
			attrs = new AttrDesc;
			status = attrCat->getInfo(attr->relName, attr->attrName, *attrs);
			if(status != OK) throw status;
			
			if(attrs->indexed && op == EQ) {
				status = IndexSelect(result, projCnt, projList, attrs, op, attrValue, record_length);
				if(status != OK) throw status;
			} else {
				status = ScanSelect(result, projCnt, projList, attrs, op, attrValue, record_length);
				if(status != OK) throw status;
			}
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
	if(attrs) delete[] attrs;
	if(projList) delete[] projList;

	return status;
}

