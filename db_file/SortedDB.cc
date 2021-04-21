#include "SortedDB.h"

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

void SortedDB::Load(Schema &schema, const char *loadpath) {
  Record temp;

  FILE *tableFile = fopen(loadpath, "r");

  while (temp.SuckNextRecord(&schema, tableFile) == 1) {
    Add(temp);
  }
  fclose(tableFile);
}

void SortedDB::MoveFirst() {
  if (this->mode == Writing) {
    this->merge();
    this->mode = Reading;
  }
  this->currPage = 0;
}

void SortedDB::Add(Record &addme) {
  if (mode != Writing) {
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
    mode = Reading;
  }

  // if there are no pages, there are no records to get
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
int SortedDB::GetNext(Record &fetchme, CNF &cnf, Record &literal) {
  if (this->mode == Writing) {
    // std::cout << "MERGING" << std::endl;
    this->merge();
  }

  // if there are no pages, there are no records to get
  if (this->file->GetLength() == 0) {
    return 0;
  }

  if (this->pageBuffer == nullptr) {
    this->pageBuffer = new Page();
    this->file->GetPage(pageBuffer, 0);
    this->currPage = 0;
  }

  if (this->mode != Querying) {
    cnf.BuildQuery(query, order);
  }

  this->mode = Querying;

  // do binary search
  if (query.numAtts > 0) {
    if (binarySearch(fetchme, literal, cnf)) {

      // sanity check
      if (comp.Compare(&fetchme, &literal, &cnf, &query) != 0) {
        std::cout << "THIS SHOULD NEVER HAPPEN" << std::endl;
        return 0;
      }
      if (comp.Compare(&fetchme, &literal, &cnf) != 0) {
        return 1;
      }
      while (true) {
        int numCheckedInPage = 0;
        while (pageBuffer->GetFirst(&fetchme)) {
          numCheckedInPage++;
          if (comp.Compare(&fetchme, &literal, &cnf, &query) != 0) {
            return 0;
          }
          if (comp.Compare(&fetchme, &literal, &cnf) != 0) {
            return 1;
          }
        }

        currPage++;

        if (currPage < this->file->GetLength() - 1) {
          delete this->pageBuffer;
          this->pageBuffer = new Page();
          this->file->GetPage(this->pageBuffer, currPage);
        } else {
          break;
        }
      }

    } else {
      // std::cout << "Bineary search failed" << std::endl;
      return 0;
    }

  } else { // normal linear scan

    // std::cout << "Doing linear search" << std::endl;

    if (this->GetNext(fetchme) == 0) {
      return 0;
    }
    while (this->comp.Compare(&fetchme, &literal, &cnf) == 0) {
      if (this->GetNext(fetchme) == 0) {
        return 0;
      }
    }

    return 1;
  }

  return 0;
}

SortedDB::~SortedDB() {
  if (pageBuffer != nullptr) {
    delete pageBuffer;
    pageBuffer = nullptr;
  }
}

bool SortedDB::binarySearch(Record &recFound, Record &literal, CNF &cnf) {
  // sanity check that we have pages
  if (this->file->GetLength() <= 0) {
    return false;
  }
  if (this->currPage >= this->file->GetLength() - 1) {
    return false;
  }
  int left = this->currPage;
  int right = this->file->GetLength() - 2;

  int numWrong = 0;

  while (left <= right) {
    int pageMidpoint = (left + right) / 2; // naturally floors

    bool notEmpty = false;
    Page *temp = new Page();

    if (this->currPage == pageMidpoint) {
      notEmpty = pageBuffer->GetFirst(&recFound);
    } else {
      file->GetPage(temp, pageMidpoint);
      notEmpty = temp->GetFirst(&recFound);
    }

    while (notEmpty) {
      // compares next record to literal record (based on cnf)
      int comparisonResult =
          this->comp.Compare(&recFound, &literal, &cnf, &query);
      if (comparisonResult > 0) {
        right--;
        numWrong++;
        break;
      } else if (comparisonResult == 0) {
        // Are we replacing current page or did we find record in curr page?
        if (currPage == pageMidpoint) {
          delete temp;
        } else {
          currPage = pageMidpoint;
          delete pageBuffer;
          pageBuffer = temp;
        }

        return true;
      }
      numWrong++;
      // std::cout << "NUMWRONG: " << numWrong << std::endl;

      if (this->currPage == pageMidpoint) {
        // std::cout << "Getting from current page 2" << std::endl;

        notEmpty = pageBuffer->GetFirst(&recFound);
      } else {
        notEmpty = temp->GetFirst(&recFound);
      }
    }

    delete temp;

    if (!notEmpty) {
      left++;
    }
  }

  return false;
}

void SortedDB::merge() {
  inPipe->ShutDown();

  if (this->file->GetLength() == 0) {
    Page page;
    Record record;
    int recs = 0;
    while (outPipe->Remove(&record)) {
      recs++;
      if (!page.Append(&record)) {
        // std::cout << "Adding page at index: " << this->file->GetLength()
        //           << std::endl;

        file->AddPage(&page, this->file->GetLength());
        page.EmptyItOut();
        page.Append(&record);
      }
    }

    // std::cout << "Adding page at index: " << this->file->GetLength()
    //           << std::endl;
    // std::cout << "Added this num of recs: " << recs << std::endl;
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
      // std::cout << "Current page in file merging: " << i << std::endl;

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
            if (this->comp.Compare(&fileRec, &pipeRec, &order) < 0) {
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

    // std::cout << "Current length of file: " << file->GetLength() <<
    // std::endl; std::cout << "New number of records in file: " <<
    // records.size()
    //           << std::endl;
    Page temp;

    for (uint i = 0; i < records.size(); i++) {
      if (!temp.Append(&records[i])) {

        // std::cout << "Adding page at index: " << this->file->GetLength()
        //           << std::endl;
        this->file->AddPage(&temp, this->file->GetLength());
        temp.EmptyItOut();
        i--;
      }
    }

    this->file->AddPage(&temp, this->file->GetLength());
    // std::cout << "Length after adding pages: " << file->GetLength()
    //           << std::endl;
  }

  // Done using current bigq to place sorted records
  this->outPipe->ShutDown();

  // this resets the page buffer to begin reading
  this->currPage = 0;
  delete this->pageBuffer;
  this->pageBuffer = new Page();
  this->file->GetPage(pageBuffer, 0);

  // deletes pipes and sorter, to be created on next write (call to GetNext)
  this->cleanup();
}

void SortedDB::cleanup() {
  delete this->inPipe;
  this->inPipe = nullptr;
  delete this->outPipe;
  this->outPipe = nullptr;

  delete this->sorter;
  sorter = nullptr;
}