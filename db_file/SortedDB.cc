#include "SortedDB.h"
#include <iterator>
#include <sstream>

SortedDB::SortedDB() : currPage(0) {}

void SortedDB::Create(File *file, std::string &fileName, void *startup) {
  this->file = file;
  this->fileName = fileName;

  StartupInfo *startupInfo = static_cast<StartupInfo *>(startup);
  this->runLength = startupInfo->l;
  this->order = *startupInfo->o;

  // std::cout << "-- SORTED DB CREATED --" << std::endl;
  // std::cout << "Run Length: " << runLength << std::endl;
  // order.Print();
}

void SortedDB::Open(File *file, std::string &fileName,
                    std::ifstream &metaFile) {
  this->fileName = fileName;
  this->file = file;

  std::string temp;
  std::getline(metaFile, temp);
  runLength = stoi(temp);
  OrderMaker tempOrder;

  metaFile >> this->order;

  // std::cout << "-- SORTED DB OPENED --" << std::endl;
  // std::cout << "Run Length: " << runLength << std::endl;
  // this->order.Print();
}
int SortedDB::Close() {
  if (this->mode == Writing) {
    this->merge();
  }

  if (pageBuffer != nullptr) {
    delete pageBuffer;
    pageBuffer = nullptr;
  }
  this->file->Close();
  return 1;
}

void SortedDB::Load(Schema &myschema, const char *loadpath) {}

void SortedDB::MoveFirst() { this->currPage = 0; }

void SortedDB::merge() {
  inPipe->ShutDown();

  if (this->file->GetLength() == 0) {
    Page page;
    Record record;
    int recs = 0;
    while (outPipe->Remove(&record)) {
      recs++;
      if (!page.Append(&record)) {
        std::cout << "Adding page at index: " << this->file->GetLength()
                  << std::endl;

        file->AddPage(&page, this->file->GetLength());
        page.EmptyItOut();
        page.Append(&record);
      }
    }

    std::cout << "Adding page at index: " << this->file->GetLength()
              << std::endl;
    std::cout << "Added this num of recs: " << recs << std::endl;
    file->AddPage(&page, this->file->GetLength());

  } else {

    Page pageToGet;
    Record pipeRec;
    Record fileRec;
    bool pipeEmpty = false;
    bool getNewFromPipe = false;

    std::vector<Record> records;

    if (!outPipe->Remove(&pipeRec)) {
      pipeEmpty = true;
      std::cout << "THIS SHOULD NEVER HAPPEN" << std::endl;
    }

    for (int i = 0; i < this->file->GetLength() - 1; i++) {
      std::cout << "Current page in file merging: " << i << std::endl;

      file->GetPage(&pageToGet, i);

      while (pageToGet.GetFirst(&fileRec)) {

        while (!pipeEmpty) {

          // This gets a new record from outPipe when the last one
          // was added to the file instead of the record from the file
          if (getNewFromPipe) {
            if (!outPipe->Remove(&pipeRec)) {
              pipeEmpty = true;
            }
          }

          if (!pipeEmpty) {
            if (this->comp.Compare(&fileRec, &pipeRec, &order) > 0) {
              getNewFromPipe = true;
              records.push_back(pipeRec);
            } else {
              getNewFromPipe = false;
              records.push_back(fileRec);
              break;
            }
          }
        }

        if (pipeEmpty) {
          records.push_back(fileRec);
        }
      }
    }

    if (!getNewFromPipe) {
      records.push_back(pipeRec);
    }

    while (outPipe->Remove(&pipeRec)) {
      records.push_back(pipeRec);
    }

    this->file->Open(0, const_cast<char *>(this->fileName.c_str()));

    std::cout << "Current length of file: " << file->GetLength() << std::endl;
    std::cout << "New number of records in file: " << records.size()
              << std::endl;
    Page temp;

    for (int i = 0; i < records.size(); i++) {
      if (!temp.Append(&records[i])) {

        std::cout << "Adding page at index: " << this->file->GetLength()
                  << std::endl;
        this->file->AddPage(&temp, this->file->GetLength());
        temp.EmptyItOut();
        i--;
      }
    }

    this->file->AddPage(&temp, this->file->GetLength());
    std::cout << "Length after adding pages: " << file->GetLength()
              << std::endl;
  }

  delete this->inPipe;
  this->inPipe = nullptr;
  this->outPipe->ShutDown();
  delete this->outPipe;
  this->outPipe = nullptr;

  delete this->sorter;
  sorter = nullptr;
  this->mode = Reading;
  this->currPage = 0;
  delete this->pageBuffer;
  this->pageBuffer = new Page();
  this->file->GetPage(pageBuffer, 0);
  delete this->sorter;
  this->sorter = nullptr;
}

void SortedDB::Add(Record &addme) {
  if (mode == Reading) {
    this->inPipe = new Pipe(100);
    this->outPipe = new Pipe(100);
    this->sorter = new BigQ(*inPipe, *outPipe, order, runLength);
    this->mode = Writing;
  }

  inPipe->Insert(&addme);
}

int SortedDB::GetNext(Record &fetchme) {
  if (mode == Writing) {
    this->merge();
  }
  if (this->file->GetLength() == 0) {
    return 0;
  }

  if (this->pageBuffer == nullptr) {
    this->pageBuffer = new Page();
    this->file->GetPage(pageBuffer, 0);
    this->currPage = 0;
  }

  // std::cout << "Number of pages: " << file->GetLength() << std::endl;
  // std::cout << "Current page: " << currPage << std::endl;

  if (!this->pageBuffer->GetFirst(&fetchme)) {
    // std::cout << "Failed to get first record" << std::endl;
    if (currPage + 2 == this->file->GetLength()) {
      return 0;
    }
    currPage++;
    delete this->pageBuffer;
    this->pageBuffer = new Page();
    this->file->GetPage(this->pageBuffer, currPage);
    this->GetNext(fetchme);
  }

  return 1;
}
int SortedDB::GetNext(Record &fetchme, CNF &cnf, Record &literal) { return 0; }

SortedDB::~SortedDB() {
  if (pageBuffer != nullptr) {
    delete pageBuffer;
    pageBuffer = nullptr;
  }
}