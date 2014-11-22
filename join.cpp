#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <cmath>
#include <cstring>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define DOUBLEERROR 1e-07

/*
 * Joins two relations
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

Status Operators::Join(const string& result,           // Name of the output relation 
                       const int projCnt,              // Number of attributes in the projection
    	               const attrInfo projNames[],     // List of projection attributes
    	               const attrInfo* attr1,          // Left attr in the join predicate
    	               const Operator op,              // Predicate operator
    	               const attrInfo* attr2)          // Right attr in the join predicate
{
	Status status;
	AttrDesc left_attr;
	AttrDesc right_attr;
	AttrDesc* projList = NULL;
	int record_length;
	bool equi_join;
	bool indices_exist;
	
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

		// Get attribute info for left
		status = attrCat->getInfo(attr1->relName, attr1->attrName, left_attr);
		if(status != OK) throw status;
		
		// Get attribute info for right
		status = attrCat->getInfo(attr2->relName, attr2->attrName, right_attr);
		if(status != OK) throw status;


		equi_join = (op == EQ) ? true : false;
		indices_exist = (left_attr.indexed || right_attr.indexed) ? true : false;
		
		if(equi_join) {
			if(indices_exist) {
				status = INL(result, projCnt, projList, left_attr, op, right_attr, record_length);
			} else {
				status = SMJ(result, projCnt, projList, left_attr, op, right_attr, record_length);
			}
		} else {
			status = SNL(result, projCnt, projList, left_attr, op, right_attr, record_length);
		}
		
		if(status != OK) throw status;
		
		// no exceptions thrown, so status is OK
		status = OK;
	} catch (Status s) {
		status = s;
	}
	
	// Free memory
	if(projList) delete[] projList;

	return status;
}

// Function to compare two record based on the predicate. Returns 0 if the two attributes 
// are equal, a negative number if the left (attrDesc1) attribute is less that the right 
// attribute, otherwise this function returns a positive number.
int Operators::matchRec(const Record& outerRec,     // Left record
                        const Record& innerRec,     // Right record
                        const AttrDesc& attrDesc1,  // Left attribute in the predicate
                        const AttrDesc& attrDesc2)  // Right attribute in the predicate
{
    int tmpInt1, tmpInt2;
    double tmpFloat1, tmpFloat2, floatDiff;

    // Compare the attribute values using memcpy to avoid byte alignment issues
    switch(attrDesc1.attrType)
    {
        case INTEGER:
            memcpy(&tmpInt1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof(int));
            memcpy(&tmpInt2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof(int));
            return tmpInt1 - tmpInt2;

        case DOUBLE:
            memcpy(&tmpFloat1, (char *) outerRec.data + attrDesc1.attrOffset, sizeof(double));
            memcpy(&tmpFloat2, (char *) innerRec.data + attrDesc2.attrOffset, sizeof(double));
            floatDiff = tmpFloat1 - tmpFloat2;
            return (fabs(floatDiff) < DOUBLEERROR) ? 0 : (floatDiff < 0?floor(floatDiff):ceil(floatDiff));

        case STRING:
            return strncmp(
                (char *) outerRec.data + attrDesc1.attrOffset, 
                (char *) innerRec.data + attrDesc2.attrOffset, 
                MAX(attrDesc1.attrLen, attrDesc2.attrLen));
    }

    return 0;
}
