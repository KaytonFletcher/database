#include "SortedDB.h"

SortedDB::SortedDB() {}

void SortedDB::Create(File *file, void *startup) {
  this->file = file;

  // static_cast<type>(expression)
}
void SortedDB::Open(File *file) {}
int SortedDB::Close() { return 0; }

void SortedDB::Load(Schema &myschema, const char *loadpath) {}

void SortedDB::MoveFirst() {}
void SortedDB::Add(Record &addme) {}

int SortedDB::GetNext(Record &fetchme) { return 0; }
int SortedDB::GetNext(Record &fetchme, CNF &cnf, Record &literal) { return 0; }