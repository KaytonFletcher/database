#include "DBFile.h"
#include <stdexcept>

DBFile::DBFile()
    : currPage(0), indexInPage(0), numPages(0), pageDirty(false),
      file(new File()) {}

DBFile::~DBFile() { delete this->file; }

int DBFile::Create(const char *f_path, fType f_type, void *startup) {
  if (f_type != heap) {
    return 0;
  }
  size_t len = strlen(f_path) + 1;
  char *f_path_copy = new char[len]();
  strncpy(f_path_copy, f_path, len);

  try{
  this->file->Open(0, f_path_copy);
  } catch(std::runtime_error) {
     delete[] f_path_copy;
    return 0;
  }

  delete[] f_path_copy;
  return 1;
}

void DBFile::Load(Schema &f_schema, const char *loadpath) {

  Record temp;

  FILE *tableFile = fopen(loadpath, "r");

  while (temp.SuckNextRecord(&f_schema, tableFile) == 1) {
    Add(temp);
  }
  fclose(tableFile);
}

int DBFile::Open(const char *f_path) {
  size_t len = strlen(f_path) + 1;
  char *f_path_copy = new char[len]();
  strncpy(f_path_copy, f_path, len);
  this->file->Open(1, f_path_copy);
  delete[] f_path_copy;

  this->numPages = this->file->GetLength();
  return 1;
}

void DBFile::MoveFirst() {
  this->currPage = 0;
  this->indexInPage = 0;
}

int DBFile::Close() {
  if (this->pageDirty) {
    this->file->AddPage(this->pageBuffer, numPages++);
    delete this->pageBuffer;
    this->pageBuffer = nullptr;
  }
  this->file->Close();
  return 1;
}

void DBFile::Add(Record &rec) {
  if (pageDirty) {
    int ret = this->pageBuffer->Append(&rec);

    // if page was full, we write it
    if (ret == 0) {
      // adds page at numPages and increments numPages
      this->file->AddPage(this->pageBuffer, numPages++);
      // TODO: Maybe delete page here
      delete this->pageBuffer;
      this->pageBuffer = nullptr;
      this->pageDirty = false;
      Add(rec);
    }
  } else {
    this->pageBuffer = new Page();

    int ret = this->pageBuffer->Append(&rec);
    this->pageDirty = true;

    if (ret == 0) {
      std::cout << "THIS SHOULD NEVER HAPPEN" << std::endl;
    }
  }
}

int DBFile::GetNext(Record &fetchMe) {
  if (this->pageDirty) {
    // adds page at numPages and increments numPages
    this->file->AddPage(this->pageBuffer, numPages++);
    // TODO: Maybe delete page here
    delete this->pageBuffer;
    this->pageBuffer = nullptr;
    this->pageDirty = false;
  }

  if (this->pageBuffer == nullptr) {
    this->pageBuffer = new Page();
    // retreive page and go through it until correct record is pulled
    this->file->GetPage(this->pageBuffer, this->currPage);
  }

  int ret = 0;

  ret = this->pageBuffer->GetFirst(&fetchMe);

  // if final record pulled failed, we move on to the next page,
  // unless we are at the last page
  if (ret == 0) {
    delete this->pageBuffer;
    this->pageBuffer = nullptr;

    if (this->currPage == this->numPages - 2) {
      return 0;
    } else {
      currPage++;
      indexInPage = 0;
      // recursively determine if next page will have a valid record
      return GetNext(fetchMe);
    }
  }

  return 1;
}

int DBFile::GetNext(Record &fetchme, CNF &cnf, Record &literal) {

  if (GetNext(fetchme) == 0) {
    return 0;
  }
  while (comp.Compare(&fetchme, &literal, &cnf) == 0) {
    if (GetNext(fetchme) == 0) {
      return 0;
    }
  }

  return 1;
}
