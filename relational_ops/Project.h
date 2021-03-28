#pragma once
#include "RelOp.h"

struct ProjectInfo {
  Pipe &inPipe;
  Pipe &outPipe;
  int *keepMe;
  int numAttsInput;
  int numAttsOutput;
};

class Project : public RelationalOp {

private:
  ProjectInfo *info = nullptr;

public:
  void Run(Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput,
           int numAttsOutput);
  void WaitUntilDone();
  void Use_n_Pages(int n);
};
