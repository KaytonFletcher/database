#pragma once
#include "RelOp.h"

class SelectPipe : public RelationalOp {
private:
  Record literal;

  Pipe inPipe;
  Pipe outPipe;

  CNF selection;

  void doWork();

public:
  SelectPipe() : inPipe(100), outPipe(100){};

  void Run(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);
  void WaitUntilDone();
  void Use_n_Pages(int n);
};