#include "HeapDB.h"
#include <fstream>

HeapDB::HeapDB() : currPage(0), numPages(0), pageDirty(false) {}

void HeapDB::Create(File *file, std::string &fileName, void *startup) {
  this->file = file;
}

void HeapDB::Load(Schema &f_schema, const char *loadpath) {

  Record temp;

  FILE *tableFile = fopen(loadpath, "r");

  while (temp.SuckNextRecord(&f_schema, tableFile) == 1) {
    Add(temp);
  }
  fclose(tableFile);
}

void HeapDB::Open(File *file, std::string &fileName, std::ifstream &meta) {
  this->file = file;
  this->numPages = this->file->GetLength();
}

void HeapDB::MoveFirst() { this->currPage = 0; }

int HeapDB::Close() {
  if (this->pageDirty) {
    this->file->AddPage(this->pageBuffer, numPages++);
    delete this->pageBuffer;
    this->pageBuffer = nullptr;
  }
  this->file->Close();
  return 1;
}

void HeapDB::Add(Record &rec) {
  if (pageDirty) {
    int ret = this->pageBuffer->Append(&rec);

    // if page was full, we write it
    if (ret == 0) {
      // adds page at numPages and increments numPages
      this->file->AddPage(this->pageBuffer, numPages++);

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

int HeapDB::GetNext(Record &fetchMe) {
  if (this->pageDirty) {
    // adds page at numPages and increments numPages
    this->file->AddPage(this->pageBuffer, numPages++);

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
      // recursively determine if next page will have a valid record
      return GetNext(fetchMe);
    }
  }

  return 1;
}

int HeapDB::GetNext(Record &fetchme, CNF &cnf, Record &literal) {

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
