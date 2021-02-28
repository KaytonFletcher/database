#pragma once

#include "../db_core/Defs.h"
#include "../db_core/File.h"
#include "../db_core/Record.h"
#include "../db_core/Schema.h"

#include "../compare/ComparisonEngine.h"

class InternalDBFile {
protected:
  // File that is acting as heap-based database
  File *file = nullptr;

  ComparisonEngine comp;

public:
  virtual void Create(File* file, void *startup) = 0;
  virtual void Open(File* file) = 0;
  virtual int Close() = 0;

  virtual void Load(Schema &myschema, const char *loadpath) = 0;

  virtual void MoveFirst() = 0;
  virtual void Add(Record &addme) = 0;
  virtual int GetNext(Record &fetchme) = 0;
  virtual int GetNext(Record &fetchme, CNF &cnf, Record &literal) = 0;

  virtual ~InternalDBFile();
};