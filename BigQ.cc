#include "BigQ.h"
#include "ComparisonEngine.h"
#include <queue>
#include <utility>

struct thread_data {

  thread_data(Pipe &in, Pipe &out, OrderMaker &orderMaker, int runlen)
      : in(in), out(out), sortorder(orderMaker), runlen(runlen) {}
  Pipe in;
  Pipe out;
  OrderMaker sortorder;
  int runlen;
};

void merge(thread_data *thread_data, ComparisonEngine &ce,
           std::vector<int> &runIndices, File &file) {

  auto compare = [thread_data, ce](std::pair<int, Record> lhs,
                                   std::pair<int, Record> rhs) {
    return ce.Compare(&lhs.second, &rhs.second, &thread_data->sortorder);
  };

  // get one page from each run
  for (int i = 0; i < thread_data->runlen; i++) {

    std::vector<Page> runPages(runIndices.size(), Page());
    std::priority_queue<std::pair<int, Record>,
                        std::vector<std::pair<int, Record>>, decltype(compare)>
        queue(compare);

    // Getting the next page from each run and putting it in runPages
    for (int j = 0; j < runIndices.size(); j++) {
      file.GetPage(&runPages[j], runIndices[j] + i);
    }

    // Fills queue with initial pages first records
    for (int i = 0; i < runPages.size(); i++) {
      Record temp;
      if (runPages[i].GetFirst(&temp)) {
        queue.push(std::make_pair(i, temp));
      }
    }

    while (!queue.empty()) {
      Record temp = queue.top().second;
      int index = queue.top().first;
      thread_data->out.Insert(&temp);
      queue.pop();

      Record temp2;
      if (runPages[index].GetFirst(&temp2)) {
        queue.push(std::make_pair(index, temp));
      }
    }
  }
}

void *readAndSort(void *data) {
  struct thread_data *thread_data;
  thread_data = (struct thread_data *)data;

  int numPages = 0;
  int recordsWritten = 0;
  int pageIndex = 0;

  Record record;
  File file;
  ComparisonEngine ce;

  char fName[13] = "sortfile.bin";
  file.Open(0, fName);
  std::vector<Page> pages(thread_data->runlen, Page());

  std::vector<int> runIndices;

  while (thread_data->in.Remove(&record)) {
    if (!pages[pageIndex].Append(&record)) {

      numPages++;
      if (numPages == thread_data->runlen) {
        std::vector<Record> records;
        for (auto page : pages) {
          Record temp;
          while (page.GetFirst(&temp)) {
            records.push_back(temp);
          }
        }

        std::sort(records.begin(), records.end(),
                  [thread_data, ce](Record r1, Record r2) {
                    return ce.Compare(&r1, &r2, &thread_data->sortorder);
                  });

        for (int i = recordsWritten; i < recordsWritten + records.size(); i++) {
          Page temp;
          if (!temp.Append(&records[i])) {
            file.AddPage(&temp, i);
          }
        }
        runIndices.push_back(recordsWritten);
        recordsWritten += records.size();

        pages.clear();
      }
    }
  }

  std::vector<Record> records;
  for (auto page : pages) {
    Record temp;
    while (page.GetFirst(&temp)) {
      records.push_back(temp);
    }
  }

  std::sort(records.begin(), records.end(),
            [thread_data, ce](Record r1, Record r2) {
              return ce.Compare(&r1, &r2, &thread_data->sortorder);
            });

  for (int i = recordsWritten; i < recordsWritten + records.size(); i++) {
    Page temp;
    if (!temp.Append(&records[i])) {
      file.AddPage(&temp, i);
    }
  }
  runIndices.push_back(recordsWritten);
  // recordsWritten += records.size();

  merge(thread_data, ce, runIndices, file);

  pthread_exit(NULL);
}

BigQ ::BigQ(Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen)
    : in(in), out(out), sortorder(sortorder), runlen(runlen) {
  // thread_data data(in, out, sortorder, runlen);

  this->worker = new std::thread(readAndSort, (void *)&data);
  // read data from in pipe sort them into runlen pages

  // construct priority queue over sorted runs and dump sorted data
  // into the out pipe

  // finally shut down the out pipe
  out.ShutDown();
}

BigQ::~BigQ() { delete worker; }
