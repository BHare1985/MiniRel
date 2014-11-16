/**************************************
 * DO NOT IN ANY WAY MODIFY THIS FILE *
 **************************************/

#ifndef DB_H
#define DB_H

#include <sys/types.h>
#include <string>
#include <functional>
#include <map>
#include <iostream>
#include "error.h"

using namespace std;

// Define if debug output wanted
// #define DEBUGIO
// #define DEBUGFREE

// Forward declarations
class DB;
class Page;

// Class definition for open files
class File {
public:
  friend class DB;

  // Allocate a new page
  const Status allocatePage(int& pageNo);

  // Release space for a page
  const Status disposePage(const int pageNo);

  // Read page from file
  const Status readPage(const int pageNo, Page* pagePtr) const;

  // Write page to file
  const Status writePage(const int pageNo, const Page* pagePtr);

  // Returns pageNo of first page
  const Status getFirstPage(int& pageNo) const; 

  bool operator == (const File & other) const 
  {
    return fileName == other.fileName;
  }

  bool operator < (const File & other) const
  {
    return fileName < other.fileName;
  }

  void printName() const
  {
    cout << "File: " << fileName << endl;
  }

private: 
  File(const string & fname);
  ~File();

  static const Status create(const string & fileName);
  static const Status destroy(const string & fileName);

  const Status open();
  const Status close();

  // Check if the page is on the free list
  const Status onFreeList(const int pageNo, bool& onFL) const; 

  // Internal file read
  const Status intread(const int pageNo, Page* pagePtr) const;

  // Internal file write
  const Status intwrite(const int pageNo, const Page* pagePtr);

#ifdef DEBUGFREE
  // List free pages
  void listFree();
#endif

  string fileName;    // The name of the file
  int openCnt;        // # times file has been opened
  int unixFile;       // Unix file stream for file
};


class BufMgr;
extern BufMgr* bufMgr;


class DB {
public:
  DB();   // Initialize open file table
  ~DB();  // Clean up any remaining open files

  // Create a new file
  const Status createFile(const string & fileName) const;

  // Destroy a file and release all space
  const Status destroyFile(const string & fileName) const;

  const Status openFile(const string & fileName, File* & file);
  const Status closeFile(File* file);

 private:
  typedef map<string, File* > OpenFileMap;
  OpenFileMap openFiles;    // List of open files
};


// Structure of DB (header) page
typedef struct {
  int nextFree;       // Page # of next page on free list
  int firstPage;      // Page # of first page in file
  int numPages;       // Total # of pages in file
} DBPage;

#endif
