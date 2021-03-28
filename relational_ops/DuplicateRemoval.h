#pragma once
#include "RelOp.h"

struct DuplicateRemovalInfo {
  Pipe &inPipe;
  Pipe &outPipe;
  Schema &schema;
  int runLength;
};

class DuplicateRemoval : public RelationalOp {

private:
  DuplicateRemovalInfo *info = nullptr;

  int runLength = 1;

public:
  void Run(Pipe &inPipe, Pipe &outPipe, Schema &mySchema);

  void WaitUntilDone();
  void Use_n_Pages(int n);
};