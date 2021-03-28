#pragma once

#include "RelOp.h"

struct JoinInfo {
  Pipe &inPipeL;
  Pipe &inPipeR;
  Pipe &outPipe;
  CNF &selOp;
  Record &literal;
  int runLength;
  int leftAtts;
  int rightAtts;
};

class Join : public RelationalOp {

private:
  JoinInfo *info = nullptr;
  int runLength = 1;

public:
  void Run(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp,
           Record &literal, int leftAtts, int rightAtts);
  void WaitUntilDone();
  void Use_n_Pages(int n);
};