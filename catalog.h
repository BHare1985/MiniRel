#ifndef CATALOG_H
#define CATALOG_H

#include "datatypes.h"
#include "heapfile.h"
// #include "index.h"

// Define if debug output wanted
// #define DEBUGCAT

#define RELCATNAME   "relcat"           // Name of relation catalog
#define ATTRCATNAME  "attrcat"          // Name of attribute catalog
#define RELNAME      "relname"          // Name of indexed field in rel/attrcat
#define MAXNAME      32                 // Length of relName, attrName
#define MAXSTRINGLEN 255                // Max length of string attribute


// -------- These enums are defined in datatypes.h -------------------------
// Data types that minirel understands:
// enum Datatype { INTEGER = 0, DOUBLE = 1, STRING = 2 };
//
// Operations that are supported in predicates in minirel:
// enum Operator { LT, LTE, EQ, GTE, GT, NE, NOTSET };
// -------------------------------------------------------------------------


// Schema of relation catalog:
//   relation name   : char(32)           <-- lookup key
//   attribute count : integer(4)
//   index count     : integer(4)
typedef struct {
  char relName[MAXNAME];                // relation name
  int attrCnt;                          // number of attributes
  int indexCnt;                         // number of indexed attrs
} RelDesc;


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


// Class that represents the Relation Catalog. This class inherits from 
// HeapFileScan because it uses its mechanism to store catalog tuples
class RelCatalog : public HeapFileScan {
 public:
  // Open relation catalog
  RelCatalog(Status &status);

  // Get a RelDesc object that describes a relation (rName)
  const Status getInfo(const string& rName, RelDesc& record);

  // Add tuple to catalog
  const Status addInfo(RelDesc& record);

  // Remove tuple from catalog
  const Status removeInfo(const string& rName);

  // Create a new relation
  const Status createRel(const string& rName, const int attrCnt, const attrInfo attrList[]);

  // Destroy a relation
  const Status destroyRel(const string& rName);

  // Add index to a relation
  const Status addIndex(const string& rName, const string& attrName);

  // Remove index from a relation
  const Status dropIndex(const string& rName, const string& attrName);

  // Print catalog information (relation may be NULL)
  const Status help(const string& rName);

  ~RelCatalog();
};


// Schema of attribute catalog:
//   relation name    : char(32)           <-- lookup key
//   attribute name   : char(32)
//   attribute number : integer(4)
//   attribute type   : integer(4)  (type is Datatype actually)
//   attribute size   : integer(4)
//   index flag       : integer(4)
typedef struct {
  char relName[MAXNAME];                // relation name
  char attrName[MAXNAME];               // attribute name
  int attrOffset;                       // attribute offset
  int attrType;                         // attribute type
  int attrLen;                          // attribute length
  int indexed;                          // TRUE if indexed
} AttrDesc;


// Class that represents the Attribute Catalog. This class inherits from 
// HeapFileScan because it uses its mechanism to store catalog tuples
class AttrCatalog : public HeapFileScan {
public:
  friend class RelCatalog;

  // Open attribute catalog
  AttrCatalog(Status &status);

  // Get an AttrDesc object that describes an attribute (attrName) in a relation (rName)
  const Status getInfo(const string& rName, const string& attrName, AttrDesc &record);

  // Add tuple to catalog
  const Status addInfo(AttrDesc& record);

  // Remove tuple from catalog
  const Status removeInfo(const string& rName, const string& attrName);

  // Get a list of AttrDesc objects that describe all attributes of a relation (rName)
  // Also return the number of attributes in this relation (attrCnt)
  const Status getRelInfo(const string& rName, int &attrCnt, AttrDesc *&attrs);

  // Delete all attributes of a relation
  const Status dropRelation(const string& rName);

  // Add index to a relation
  const Status addIndex(const string& rName, const string& attrName);

  // Remove index from a relation
  const Status dropIndex(const string& rName, const string& attrName);

  ~AttrCatalog();
};

// Extern variables that are instantianted in the main program. These variables 
// represent a global instance of RelCatalog, AttrCatalog and Error 
// that will be used by all parts of the program
extern RelCatalog  *relCat;   // Pointer to the relational catalog object
extern AttrCatalog *attrCat;  // Pointer to the attribute catalog object
extern Error error;           // The system error manager

#endif
