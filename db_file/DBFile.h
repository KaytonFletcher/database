#pragma once

#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>

#include "../db_core/Defs.h"
#include "../db_core/File.h"
#include "../db_core/Record.h"
#include "../db_core/Schema.h"

#include "HeapDB.h"
#include "InternalDBFile.h"
#include "SortedDB.h"

typedef enum { heap, sorted, tree } fType;

class DBFile {
private:
  bool isOpen = false;
  InternalDBFile *db = nullptr;

public:
  DBFile();
  ~DBFile();

  int Create(const char *fpath, fType file_type, void *startup);
  int Open(const char *fpath);
  int Close();

  void Load(Schema &myschema, const char *loadpath);

  void MoveFirst();
  void Add(Record &addme);
  int GetNext(Record &fetchme);
  int GetNext(Record &fetchme, CNF &cnf, Record &literal);
};
