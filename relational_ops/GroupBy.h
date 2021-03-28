#pragma once

#include "RelOp.h"

struct GroupByInfo {
  Pipe &inPipe;
  Pipe &outPipe;
  OrderMaker &groupAtts;
  int totalAtts;
  Function &computeMe;
  int runLength;
};

class GroupBy : public RelationalOp {
private:
  GroupByInfo *info = nullptr;
  int runLength = 1;

public:
  void Run(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, int totalAtts,
           Function &computeMe);
  void WaitUntilDone();
  void Use_n_Pages(int n);
};