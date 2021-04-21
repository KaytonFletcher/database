#include "DBFile.h"

DBFile::DBFile() : isOpen(false), db(nullptr) {}

DBFile::~DBFile() { delete db; }

int DBFile::Create(const char *f_path, fType f_type, void *startup) {
  if (this->isOpen) {
    db->Close();
    delete db;
    db = nullptr;
  }

  std::string filePath(f_path);

  File *file = new File;

  try {
    file->Open(0, const_cast<char *>(filePath.c_str()));
  } catch (std::runtime_error &) {
    return 0;
  }

  if (f_type == heap) {
    db = new HeapDB();
    std::ofstream metaFile(filePath + ".meta");

    if (metaFile.good()) {
      metaFile.write("heap\n", 6);
      metaFile.close();
    } else {
      return 0;
    }
  } else if (f_type == sorted) {
    db = new SortedDB();
    std::ofstream metaFile(filePath + ".meta");

    if (metaFile.good()) {
      StartupInfo *startupInfo = static_cast<StartupInfo *>(startup);

      metaFile << "sorted\n" << startupInfo->l << "\n" << *startupInfo->o;
    } else {
      return 0;
    }
  } else {
    return 0;
  }

  this->isOpen = true;
  db->Create(file, filePath, startup);

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

  File *file = new File;

  try {
    file->Open(1, const_cast<char *>(filePath.c_str()));
  } catch (std::runtime_error &) {
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

  this->isOpen = true;
  db->Open(file, filePath, metaFile);
  metaFile.close();
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
