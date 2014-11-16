#ifndef SORT_H
#define SORT_H

#include <vector>
#include "heapfile.h"

// Define if debug output wanted
// #define DEBUGSORT


// SORTREC is an in-memory sort record that qsort(3) sorts.
// The sort attribute as well as the associated RID are
// stored in the record. The RID is used for fetching the
// full record when it is needed.

typedef struct {
  RID rid;                              // record id of current record
  char* field;                          // pointer to field
  int length;                           // length of field
} SORTREC;


class SortedFile {
public:
  SortedFile(const string& fileName,      // name of the relation
	           int offset,                  // offset into the attribute to perform sort
	           int length,                  // length of the attribute
             Datatype type,               // type of the attribute
	           int maxItems,                // how many tuples that to sort at one time
             Status& status);              

  // Fetch next record in sort order
  Status next(Record & rec);

  // Record a position in sort sequence      
  Status setMark();                     

  // Go to last recorded spot
  Status gotoMark();                    

  // Destroy temporary structures / files
  ~SortedFile();

private:
  Status sortFile();                    // split source file into sub-runs
  Status generateRun(int numItems);     // generate one sub-run of file
  Status startScans();                  // start a scan on each sorted run

  typedef struct {
    string name;                        // name of run file
    HeapFileScan* file;                 // ptr to sorted run of file
    int valid;                          // TRUE if recPtr has a record
    Record rec;
    RID rid;                            // RID of current record of run
    RID mark;                           // last marked spot (RID) in file
  } RUN;

  std::vector<RUN> runs;                // holds info about each sub-run

  HeapFileScan* file;                   // source file to sort
  string fileName;                      // name of source file to sort
  Datatype type;                        // type of sort attribute
  int offset;                           // offset of sort attribute
  int length;                           // length of sort attribute

  SORTREC *buffer;                      // in-memory sort buffer
  int maxItems;                         // max. # of items/tuples in buffer
  int numItems;                         // current # of items in buffer
};

#endif
