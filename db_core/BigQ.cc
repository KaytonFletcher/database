#include "BigQ.h"

void BigQ::merge(std::vector<RunInfo> &runInfo) {
  auto compare = [this](std::pair<int, Record> lhs,
                        std::pair<int, Record> rhs) {
    return ce.Compare(&lhs.second, &rhs.second, &sortorder) > 0 ? true : false;
  };

  std::cout << "NUM RUNS: " << runInfo.size() << std::endl;

  std::priority_queue<std::pair<int, Record>,
                      std::vector<std::pair<int, Record>>, decltype(compare)>
      queue(compare);

  // construct priority queue over sorted runs and dump sorted data
  // into the out pipe
  Page *runPages = new Page[runInfo.size()];

  // Gets the initial first page from each run to be put in runPages
  // ex. 3 runs with 4 pages each
  // [0,1,2,3], [4,5,6,7], [8,9,10,11]
  // will grab [0, 4, 8] first
  for (uint j = 0; j < runInfo.size(); j++) {
    if (runInfo[j].numPagesLeft) {
      file.GetPage(&runPages[j], runInfo[j].currIndex);
      runInfo[j].currIndex++;
      runInfo[j].numPagesLeft--;
    } else {
      std::cout << "THIS IS BAD" << std::endl;
    }
  }

  // Fills queue with initial pages first records
  for (uint i = 0; i < runInfo.size(); i++) {
    Record temp;
    if (runPages[i].GetFirst(&temp)) {
      queue.push(std::make_pair(i, temp));
    } else {
      std::cout << "THIS IS ALSO BAD" << std::endl;
    }
  }

  while (!queue.empty()) {
    Record temp = queue.top().second;
    int index = queue.top().first;
    out.Insert(&temp);
    queue.pop();

    Record temp2;
    if (runPages[index].GetFirst(&temp2)) {
      queue.push(std::make_pair(index, temp2));
    } else if (runInfo[index].numPagesLeft) {
      file.GetPage(&runPages[index], runInfo[index].currIndex);

      runPages[index].GetFirst(&temp2);
      queue.push(std::make_pair(index, temp2));
      runInfo[index].currIndex++;
      runInfo[index].numPagesLeft--;
    }
  }

  delete[] runPages;

  // finally shut down the out pipe
  out.ShutDown();
}

void BigQ::readAndSort() {

  int numRecordsInRun = 0;
  int numPagesinRun = 0;
  int totalPages = 0;
  Record record;
  char fName[13] = "sortfile.bin";
  file.Open(0, fName);

  Page *pages = new Page[this->runlen];
  std::vector<RunInfo> runInfo;

  bool pageDirty = false;

  // read data from in pipe sort them into runlen pages
  while (this->in.Remove(&record)) {

    pageDirty = true;

    if (!pages[numPagesinRun].Append(&record)) {
      pageDirty = false;
      numPagesinRun++;
      totalPages++;

      // all pages have been filled for run
      if (numPagesinRun == this->runlen) {
        std::vector<Record> records;

        for (int i = 0; i < runlen; i++) {
          Record temp;
          while (pages[i].GetFirst(&temp)) {
            records.push_back(temp);
          }
        }

        std::sort(records.begin(), records.end(), [this](Record r1, Record r2) {
          return (ce.Compare(&r1, &r2, &sortorder) < 0) ? true : false;
        });

        uint recordIndex = 0;
        for (int i = runlen; i > 0; i--) {
          Page temp;

          while (recordIndex < records.size() &&
                 temp.Append(&records[recordIndex])) {
            numRecordsInRun++;
            recordIndex++;
          }

          file.AddPage(&temp, totalPages - i);
        }

        runInfo.push_back(RunInfo(numPagesinRun, totalPages - numPagesinRun));

        std::cout << "Num Records in run: " << numRecordsInRun << std::endl;
        numRecordsInRun = 0;

        // Reset data structures for next run
        numPagesinRun = 0;
        delete[] pages;
        pages = new Page[runlen];

        // Chance for left over records, must be added to next run
        while (recordIndex < records.size()) {
          numRecordsInRun++;

          pages[numPagesinRun].Append(&records[recordIndex]);
          recordIndex++;
        }
      }

      // If last page was full, we must add the record that failed to append
      pages[numPagesinRun].Append(&record);
      numRecordsInRun++;
    }
  }

  std::vector<Record> records;

  if (pageDirty) {
    totalPages++;
  }

  int numPages = pageDirty ? numPagesinRun + 1 : numPagesinRun;
  if (numPages > 0) {
    runInfo.push_back(RunInfo(numPages, totalPages - numPages));
  }

  // gets remaining pages from vector (if less than runlength)
  for (int i = 0; i < numPages; i++) {
    Record temp;
    while (pages[i].GetFirst(&temp)) {
      records.push_back(temp);
    }
  }

  std::sort(records.begin(), records.end(), [this](Record r1, Record r2) {
    return (ce.Compare(&r1, &r2, &sortorder) < 0) ? true : false;
  });

  uint recordIndex = 0;

  for (int i = numPages; i > 0; i--) {
    Page temp;

    while (recordIndex < records.size() && temp.Append(&records[recordIndex])) {
      recordIndex++;
    }

    file.AddPage(&temp, totalPages - i);
  }

  if (recordIndex < records.size()) {
    Page temp;
    while (recordIndex < records.size()) {

      temp.Append(&records[recordIndex]);
      recordIndex++;
    }

    file.AddPage(&temp, totalPages);
    totalPages++;

    runInfo.push_back(RunInfo(1, totalPages - 1));
  }

  delete[] pages;

  merge(runInfo);
}

BigQ ::BigQ(Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen)
    : in(in), out(out), sortorder(sortorder), runlen(runlen) {
  this->worker = new std::thread(&BigQ::readAndSort, this);
}

BigQ::~BigQ() {
  worker->join();
  delete worker;
}
