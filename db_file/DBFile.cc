#include "DBFile.h"
#include <fstream>

DBFile::DBFile() : isOpen(false), db(nullptr){};

DBFile::~DBFile() { delete db; }

int DBFile::Create(const char *f_path, fType f_type, void *startup) {
  if (this->isOpen) {
    db->Close();
    delete db;
    db = nullptr;
  }

  std::string filePath(f_path);

  // size_t len = strlen(f_path) + 1;
  // char *f_path_copy = new char[len]();
  // strncpy(f_path_copy, f_path, len);

  File *file = new File;

  try {
    file->Open(0, const_cast<char *>(filePath.c_str()));
  } catch (std::runtime_error&) {
    // delete[] f_path_copy;
    return 0;
  }

  // delete[] f_path_copy;

  if (f_type == heap) {
    db = new HeapDB();
    std::ofstream metaFile(filePath + ".meta");

    if (metaFile.good()) {
      metaFile.write("heap", 4);
    } else {
      return 0;
    }
  } else if (f_type == sorted) {
    db = new SortedDB();
    std::ofstream metaFile(filePath + ".meta");

    if (metaFile.good()) {
      metaFile.write("sorted", 6);
    } else {
      return 0;
    }
  } else {
    return 0;
  }

  this->isOpen = true;
  db->Create(file, startup);

  return 1;
}

void DBFile::Load(Schema &f_schema, const char *loadpath) {
  db->Load(f_schema, loadpath);
}

int DBFile::Open(const char *f_path) {
  if (this->isOpen) {
    db->Close();
    delete db;
    db = nullptr;
  }

  std::string filePath(f_path);

  // size_t len = strlen(f_path) + 1;
  // char *f_path_copy = new char[len]();
  // strncpy(f_path_copy, f_path, len);

  File *file = new File;

  try {
    file->Open(1, const_cast<char *>(filePath.c_str()));
  } catch (std::runtime_error&) {
    // delete[] f_path_copy;
    return 0;
  }

  std::ifstream metaFile(filePath + ".meta");
  if (metaFile.good()) {
    std::string type;
    std::getline(metaFile, type);
    if (type == "heap") {
      db = new HeapDB();
    } else if (type == "sorted") {
      db = new SortedDB();
    } else {
      return 0;
    }
  } else {
    return 0;
  }

  // delete[] f_path_copy;

  this->isOpen = true;
  db->Open(file);

  return 1;
}

void DBFile::MoveFirst() { db->MoveFirst(); }

int DBFile::Close() {
  this->isOpen = false;
  return db->Close();
}

void DBFile::Add(Record &rec) { db->Add(rec); }

int DBFile::GetNext(Record &fetchMe) { return db->GetNext(fetchMe); }

int DBFile::GetNext(Record &fetchme, CNF &cnf, Record &literal) {
  return db->GetNext(fetchme, cnf, literal);
}
