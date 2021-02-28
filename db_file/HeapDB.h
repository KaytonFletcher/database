#pragma once

#include <stdexcept>

#include "../db_core/File.h"
#include "../db_core/Record.h"

#include "./InternalDBFile.h"

class HeapDB : public InternalDBFile {
  int currPage = 0;
  int indexInPage = 0;
  int numPages = 0;

  // when a page has records that have not yet been written to the database
  bool pageDirty = false;

  // current record in file
  Record *record = nullptr;

  // Single-page buffer for reading and writing from file
  Page *pageBuffer = nullptr;

public:
  HeapDB();

  void Create(File *file, void *startup);
  void Open(File* file);
  int Close();

  void Load(Schema &myschema, const char *loadpath);

  void MoveFirst();
  void Add(Record &addme);
  int GetNext(Record &fetchme);
  int GetNext(Record &fetchme, CNF &cnf, Record &literal);
};