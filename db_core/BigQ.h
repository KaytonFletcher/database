#ifndef BIGQ_H
#define BIGQ_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "../compare/ComparisonEngine.h"
#include "File.h"
#include "Pipe.h"
#include "Record.h"

struct RunInfo {
  int numPagesLeft;
  int currIndex;

  RunInfo(int numPages, int startingIndex)
      : numPagesLeft(numPages), currIndex(startingIndex) {}
};
class BigQ {
  static inline int numSorters = 0;
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
