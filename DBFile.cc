#include "DBFile.h"


// stub file .. replace it with your own DBFile.cc

DBFile::DBFile() { this->file = new File(); }

DBFile::~DBFile() { 
  delete this->file;
}

int DBFile::Create(const char *f_path, fType f_type, void *startup) {
  if (f_type != heap) {
    return 0;
  }
  size_t len = strlen(f_path) + 1;
  char *f_path_copy = new char[len]();
  strncpy(f_path_copy, f_path, len);

  this->file->Open(0, f_path_copy);

  delete[] f_path_copy;
  return 1;
}

void DBFile::Load(Schema &f_schema, const char *loadpath) {


}

int DBFile::Open(const char *f_path) {
  size_t len = strlen(f_path) + 1;
  char *f_path_copy = new char[len]();
  strncpy(f_path_copy, f_path, len);
  this->file->Open(1, f_path_copy);
  delete[] f_path_copy;
  return 1;
}

void DBFile::MoveFirst() {
  this->file->GetPage
}

int DBFile::Close() {
  this->file->Close();
  return 1;
}

void DBFile::Add(Record &rec) {}

int DBFile::GetNext(Record &fetchme) {}

int DBFile::GetNext(Record &fetchme, CNF &cnf, Record &literal) {}
