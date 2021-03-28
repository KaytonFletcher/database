#pragma once

#include "RelOp.h"

struct WriteOutInfo {
  Pipe &inPipe;
  FILE *outFile;
  Schema &mySchema;
  int runLength;
};

class WriteOut : public RelationalOp {
private:
  WriteOutInfo *info = nullptr;
  int runLength = 1;

public:
  void Run(Pipe &inPipe, FILE *outFile, Schema &mySchema);
  void WaitUntilDone();
  void Use_n_Pages(int n);
};