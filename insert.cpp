#include "catalog.h"
#include "query.h"
#include "index.h"

#include <stdio.h>
#include <string.h>
#include "utility.h"


/*
 * Inserts a record into the specified relation
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */


/*

// Description of an attribute. This structure is produced by the SQL
// parser and is used in two cases: In the "create relation" command 
// to send to the catalogs the name and type of the parsed attribute,
// and in the "insert into" command to send to the insert command
// the value of the attribute (using the attrValue field).
typedef struct {
  char relName[MAXNAME];                // relation name
  char attrName[MAXNAME];               // attribute name
  int  attrType;                        // INTEGER, DOUBLE, or STRING
  int  attrLen;                         // length of attribute in bytes

  // Pointer to binary value (used by the parser for insert into statements)
  // In some versions of the SQL parser, attrValue is also used to hold the 
  // default value specified during the create table command
  void *attrValue;

} attrInfo; 



This function inserts a tuple with the given attribute values (in attrList) into the specified relation. The
type attrInfo is defined in the file catalog.h. The value of the attribute is in attrList[I].attrValue, and the
name of the attribute is in attrList[I].attrame. For the SQL INTEGER (DOUBLE) data type, attrValue
is a pointer to an integer (double). Similarly for an attribute of type STRING (SQL CHAR type),
attrValue points to a character string.
The attrList array may not list the attributes in the same order as they appear in the relation, so you may
have to rearrange the attribute values before inserting the tuple into the relation. If no value is specified
for an attribute in attrList, you should reject the insertion. (In a real database system, missing attribute
values are implemented using NULLs.)
In addition to inserting the tuple, this operator must also update all the hash indices on the relation. Use
the Index::insertEntry to insert an entry into the index. Look up the system catalogs to find out
information about the relation and attributes. (See Section “Getting Started” for more details.) 

You can use attrCat->getRelInfo(const string &r.ame, int &attrCnt, AttrDesc *&attrs) to retrieve
information about the attributes of the “stars” relation. This will tell you the offset, data type, and the size
of each attribute for packing into the record. In this case, we should expect to find that “starid” has offset
0 and length 4. “real_name” has offset 4 and length 20, “plays” has offset 24 and length 12, and “soapid”
has offset 36 and length “4”. The total length of the record becomes 40. 


*/

Status Updates::Insert(const string& relation,      // Name of the relation
                       const int attrCnt,           // Number of attributes specified in INSERT statement
                       const attrInfo attrList[])   // Value of attributes specified in INSERT statement
{

	Status status;
	AttrDesc *attrs;
	int rel_attrCnt;

	// get attribute data
	if ((status = attrCat->getRelInfo(relation, rel_attrCnt, attrs)) != OK) {
		return status;
	}

	// If the relational attr count isn't equal to attr specified
	if(rel_attrCnt != attrCnt) {
		return ATTRTYPEMISMATCH;
	}
	

/*
	// open data file

	HeapFileScan *hfile = new HeapFileScan(relation, status);
	if (!hfile)
	return INSUFMEM;
	if (status != OK)
	return status;

	// cout << "Relation name: " << relation << endl << endl;

	int i;
	for(i = 0; i < attrCnt; i++) {
	printf("%-*.*s%c ", attrWidth[i], attrWidth[i],
	   attrs[i].attrName, (attrs[i].indexed ? '*' : ' '));
	}
	printf("\n");

	for(i = 0; i < attrCnt; i++) {
	for(int j = 0; j < attrWidth[i]; j++)
	  putchar('-');
	printf("  ");
	}
	printf("\n");

	if ((status = hfile->startScan(0, 0, INTEGER, NULL, EQ)) != OK)
	return status;

	Record rec;
	RID rid;

	int records = 0;
	while((status = hfile->scanNext(rid)) == OK) {
	if ((status = hfile->getRecord(rid, rec)) != OK)
	  return status;
	if (records < 100) UT_printRec(attrCnt, attrs, attrWidth, rec);
	//UT_printRec(attrCnt, attrs, attrWidth, rec);
	records++;
	}
	if (status != FILEEOF)
	return status;

	cout << endl << "Number of records: " << records << endl;

	delete [] attrWidth;
	delete [] attrs;

	// close scan and data file

	if ((status = hfile->endScan()) != OK)
	return status;
	delete hfile;
*/

	return OK;
}
