#ifndef HEAPFILE_H
#define HEAPFILE_H

#include "buf.h"
#include "page.h"
#include "datatypes.h"

extern DB db;

// Define if debug output wanted
// #define DEBUGREL

const unsigned MAXNAMESIZE = 50;

struct HeaderPage {
  char fileName[MAXNAMESIZE];   // name of file
  int firstPage;                // pageNo of first data page in file
  int	lastPage;                 // pageNo of last data page in file
  int	pageCnt;                  // number of pages
  int	recCnt;                   // record count
};


// If a heapfile does not exist, calling the constructor is equivalent to 
// creating the heapfile. Otherwise, the constructor will load existing heapfile for usage.
class HeapFile {
protected:
  File* file;             // pointer to underlying DB File object
  HeaderPage* headerPage;	// pointer to header page in buffer pool
  int headerPageNo;	      // page number of header page

public:
  HeapFile(const string& name, Status& returnStatus);
  ~HeapFile();

  // Return number of records in file
  const int getRecCnt() const;

  // Insert record into file
  const Status insertRecord(const Record& rec, RID& outRid); 

  // Delete record from file
  const Status deleteRecord(const RID& rid);
};


class HeapFileScan : public HeapFile {
public:

  // Initiate a filtered scan such that calling scanNext() will only 
  // return the next record that satisfies the predicate 
  HeapFileScan(const string& name,
    const int offset, 
    const int length,  
    const Datatype type, 
    const char* filter, 
    const Operator op,
    Status& status);

  // Initiate an unfiltered scan
  HeapFileScan(const string& name, Status& status);

  ~HeapFileScan();

  // Start a scan (with an optional predicate)
  const Status startScan(const int offset, 
			 const int length,  
			 const Datatype type, 
			 const char* filter, 
			 const Operator op);

  // End the scan
  const Status endScan();

  // Set a marker to the current record
  Status setMarker(); 

  // Move the scan back to the marked position and retrieve the Record that was marked
  Status gotoMarker(RID& rid, Record& rec); 

  // Retrieve record identified by RID
  const Status getRecord(const RID& rid, Record& rec);

  // Marks current page of scan dirty
  const Status markDirty(const RID& rid);

  // Get the next record ID
  const Status scanNext(RID& outRid);

  // Get next record ID and the record object
  const Status scanNext(RID& outRid, Record& rec);

  // Retrieve an arbitrary record from a file. If record is not on the 
  // currently pinned page, the current page is unpinned and the required page 
  // is read into the buffer pool and pinned
  const Status getRandomRecord(const RID& rid, Record& rec);

private:
  RID curRec;        // rid of last record returned
  Page* curPage;	   // pointer to pinned page in buffer pool
  int curPageNo;	   // page number of pinned page
  bool dirtyFlag;	   // true if page has been updated
  int offset;        // byte offset of filter attribute
  int length;        // length of filter attribute
  Datatype type;     // datatype of filter attribute
  const char* filter;// comparison value of filter
  Operator op;       // comparison operator of filter
  RID mark;          // last marked spot (RID) in the file.

  const bool matchRec(const Record& rec) const;
};

#endif
