#pragma once
#include "RelOp.h"

struct SumInfo {
  Pipe &inPipe;
  Pipe &outPipe;
  Function &function;
  int runLength;
};

class Sum : public RelationalOp {

private:
  SumInfo* info = nullptr;
  int runLength = 1;

public:
  void Run(Pipe &inPipe, Pipe &outPipe, Function &computeMe);

  void WaitUntilDone();
  void Use_n_Pages(int n);
};