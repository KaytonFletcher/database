#ifndef BIGQ_H
#define BIGQ_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

#include "File.h"
#include "Pipe.h"
#include "Record.h"

using namespace std;

struct RunInfo {
  int numPagesLeft;
  int currIndex;
  int startingIndex;

  RunInfo(int numPages, int startingIndex)
      : currIndex(startingIndex), startingIndex(startingIndex),
        numPagesLeft(numPages) {}
};
class BigQ {
  Pipe &in;
  Pipe &out;
  OrderMaker &sortorder;
  int runlen;
  ComparisonEngine ce;
  File file;

  std::thread *worker;

  void merge(std::vector<RunInfo> &runIndices);
  void readAndSort();

public:
  BigQ(Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
  ~BigQ();
};

#endif
