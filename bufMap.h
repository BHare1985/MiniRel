#ifndef BUFMAP_H
#define BUFMAP_H

#include "db.h"

// Define if debug output wanted
// #define DEBUGBUF

// Class for locating buffer frame (used by BufMap)
struct FileAndPage
{
  File*	file; // Pointer to a file object
  int	pageNo; // Page number within a file
  
  FileAndPage(File *f, int p) 
  {
    file = f;
    pageNo = p;
  }

  bool  operator == (const FileAndPage & other) const
  {
    return *file == *(other.file) && pageNo == other.pageNo;
  }

  bool  operator < (const FileAndPage & other) const
  {
    if (*file == *(other.file)) 
      return pageNo < other.pageNo;
    else 
      return *file < *(other.file);
  }

};


// Class to keep track of pages in the buffer pool
class BufMap { 
public:   
  // Insert entry into buffer pool, mapping (file,pageNo) to frameNo.
  // Return OK or BUFMAPERROR if an error occured.
  const Status insert(File* file, const int pageNo, const unsigned int frameNo); 

  // Check if (file,pageNo) is currently in the buffer pool and return frameNo
  // if found. Otherwise return BUFMAPNOTFOUND.
  const Status lookup(File* file, const int pageNo, unsigned int & frameNo) const; 

  // Delete entry (file,pageNo) from buffer pool.
  // Return OK if pate was found. Else return BUFMAPNOTFOUND.
  const Status remove(File* file, const int pageNo); 

private:
  typedef map<FileAndPage, unsigned int> BufHashTable;
  BufHashTable hashTable;  // Hash table for locating frame
};

#endif // BUFMAP_H
