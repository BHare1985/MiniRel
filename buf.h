#ifndef BUF_H
#define BUF_H

#include "db.h"
#include "bufMap.h"

// Define if debug output wanted
// #define DEBUGBUF

// Forward declaration
class BufMgr;

// Class for maintaining information about buffer pool frame
class BufDesc {
public:
    friend class BufMgr;

private:
  File* file;   // Pointer to file object
  int pageNo;   // Page within file
  int pinCnt;   // Number of times this page has been pinned
  bool dirty;   // True if dirty;  false otherwise
  bool valid;   // True if page is valid
  bool refbit;  // Has this buffer frame been referenced recently?

  // Initialize buffer frame for a new user
  void Clear() 
  {
    pinCnt = 0;
    file = NULL;
    pageNo = -1;
    dirty = refbit = false;
    valid = false;
  };

  void Set(File* filePtr, int pageNum) 
  { 
    file = filePtr;
    pageNo = pageNum;
    pinCnt = 1;
    dirty = false;
    refbit = true;
    valid = true;
  }

  BufDesc() { Clear(); }
};


struct BufStats {
  unsigned accesses;    // Total number of accesses to buffer pool
  unsigned diskreads;   // Number of pages read from disk 
  unsigned diskwrites;  // Number of pages written back to disk

  void clear() { accesses = diskreads = diskwrites = 0; }
  BufStats() { clear(); }
};

inline ostream & operator << (ostream & os, const BufStats & stats)
{
  os << "accesses = " << stats.accesses
     << ", disk reads = " << stats.diskreads
     << ", disk writes = " << stats.diskwrites << endl;

  return os;
}


class BufMgr {
public:
  Page *bufPool; // Actual buffer pool

  BufMgr(const unsigned int bufs);
  ~BufMgr();

  const Status readPage(File* file, const int PageNo, Page*& page);
  const Status unPinPage(File* file, const int PageNo, const bool dirty);

  // Allocates a new, empty page
  const Status allocPage(File* file, int& PageNo, Page*& page); 
                         
  // Write out all dirty pages of the file
  const Status flushFile(File* file); 

  // Dispose a page in file
  const Status disposePage(File* file, const int PageNo); 

  // Return the number of pages that are currently unpinned
  unsigned numUnpinnedPages();

  // Print the buffer pool contents
  void printSelf(void); 

  // Get buffer pool usage
  const BufStats & getBufStats() const { return bufStats; }

  void clearBufStats() { bufStats.clear(); }

private:
  unsigned int clockHand;   // Clock hand for clock algorithm
  BufMap bufMap;            // Mapping of (File, page) to frame
  BufDesc *bufTable;        // Vector of status info, 1 per page
  unsigned int numBufs;     // Number of pages in buffer pool
  BufStats bufStats;        // Statistics about buffer pool usage

  // Allocate a free frame
  const Status allocBuf(unsigned int & frame);

  void advanceClock() { clockHand = (clockHand + 1) % numBufs; }
};

#endif
