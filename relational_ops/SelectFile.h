#pragma once
#include "RelOp.h"

struct SelectFileInfo {
  DBFile &inFile;
  Pipe &outPipe;
  CNF &selection;
  Record &literal;
  int numToSkip;
};

class SelectFile : public RelationalOp {
private:
  SelectFileInfo *info;

public:
  void Run(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal);
  void Run(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal, int numToSkip);

  void WaitUntilDone();
  void Use_n_Pages(int n);
};
