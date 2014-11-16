#ifndef INDEX_H
#define INDEX_H

#include "heapfile.h"

extern DB db;

const int DIRSIZE = PAGESIZE - MAXNAMESIZE - 3*sizeof(int) - sizeof(Datatype);
const int UNIQUE  = 1;
const int NONUNIQUE = 0;


// #define BTREE_INDEX
#ifdef BTREE_INDEX 

   #include "btree.h"

   // Typedef BTreeIndex to the type Index, which allows us to switch 
   // between using a hash  or a btree index
   typedef BTreeIndex Index; 

/* Use hash index for EECS 484 Project 4 */
#else      


struct iHeaderPage {
    char          fileName[MAXNAMESIZE];  // name of file
    int           offset;                 // byte offset of the indexed attribute 
    int           length;                 // length of the attribute
    Datatype      type;                   // datatype of the attribute
    int           depth;                  // depth of the directory
    int           unique;                 // enforce uniqueness on inserts
    short         dir[1];         
    char          data[DIRSIZE - 1];
};


struct Bucket {
  short depth;
  short slotCnt;
  char  data[PAGESIZE - 2*sizeof(short)];
};

// If index does not exist, calling the constructor is equivalent to creating 
// the index. Otherwise, the constructor will load existing index for usage.
class Index {
public:
  Index(const string& name,      // name of the relation being indexed
  	const int offset,            // offset of the attribute being indexed 
  	const int length,            // length of the attribute being indexed
  	const Datatype type,         // type of the attribute being indexed
  	const int unique,            // =1 if the index should only allow unique entries
  	Status& status);             // return error codes

  ~Index();

  // Insert an entry into the index. value should point to the index key (attribute)
  const Status insertEntry(const void* value, RID rid);

  // Delete an entry from the index. value should point to the index key (attribute)
  const Status deleteEntry(const void* value, const RID& rid);
  
  // Initiate an indexed scan
  const Status startScan(const void* value);

  // Return next entry from the current run of indexed scan
  const Status scanNext(RID& outRid); 

  // End current indexed scan
  const Status endScan();

#ifdef DEBUGIND
  void printDir();
  void printBucs();
#endif

private:
  File*         file;
  iHeaderPage*  headerPage;      
  int           headerPageNo;     
  int           dirSize;        // size of the directory
  int           numSlots;       // maximum number of slots in a page
  int           recSize;
  
  int           curOffset;      // offset of the next entry to be scanned 
  Bucket*       curBuc;         // pointer to pinned page in buffer pool
  int           curPageNo;      // page number of pinned page
  const void*   curValue;       // comparison value of filter
      
  const Status matchRec(const Bucket* bucket, const void* value, const int offset);
  const Status hashIndex(const void* value, int& hashvalue);
};


#endif  
/* BTREE_INDEX */

#endif 
