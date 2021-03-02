#pragma once

#include <stdexcept>

#include "../db_core/BigQ.h"
#include "../db_core/Defs.h"
#include "../db_core/File.h"
#include "../db_core/Pipe.h"
#include "../db_core/Record.h"
#include "../db_core/Schema.h"

#include "./InternalDBFile.h"

struct StartupInfo {
  OrderMaker *o;
  int l;
};

class SortedDB : public InternalDBFile {
  int currPage = 0;

  std::string fileName;

  int runLength = 0;

  Pipe* inPipe;
  Pipe* outPipe;

  BigQ* sorter;
 
  // provided to define how the database records are sorted
  OrderMaker order;

  enum Mode { Writing, Reading };

  Mode mode = Reading;

  // current record in file
  Record *record = nullptr;

  // Single-page buffer for reading and writing from file
  Page *pageBuffer = nullptr;

  void merge();

public:
  SortedDB();
  ~SortedDB();

  void Create(File *file, std::string& fileName, void *startup);
  void Open(File *file, std::string& fileName, std::ifstream &meta);
  int Close();

  void Load(Schema &myschema, const char *loadpath);

  void MoveFirst();
  void Add(Record &addme);
  int GetNext(Record &fetchme);
  int GetNext(Record &fetchme, CNF &cnf, Record &literal);
};