#pragma once

#include "RelOp.h"

struct CountInfo {
  Pipe &inPipe;
  Pipe &outPipe;
  OrderMaker &groupAtts;
  int totalAtts;
  int runLength;
};

class Count : public RelationalOp {
private:
  CountInfo *info = nullptr;
  int runLength = 1;

public:
  void Run(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, int totalAtts);
  void WaitUntilDone();
  void Use_n_Pages(int n);
};