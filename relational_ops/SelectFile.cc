#include "SelectFile.h"

void doWork(SelectFileInfo *info) {
  int tempSkip = info->numToSkip;
  Record buffer;
  while (info->inFile.GetNext(buffer, info->selection, info->literal)) {
    if (!tempSkip) {
      info->outPipe.Insert(&buffer);
      tempSkip = info->numToSkip;
    } else {
      tempSkip--;
    }
  }
  info->outPipe.ShutDown();
}

void SelectFile::Run(DBFile &inFile, Pipe &outPipe, CNF &selOp,
                     Record &literal) {
  this->info = new SelectFileInfo{inFile, outPipe, selOp, literal, 0};

  this->worker = std::thread(&doWork, info);
}
void SelectFile::Run(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal,
                     int numToSkip) {
  this->info = new SelectFileInfo{inFile, outPipe, selOp, literal, numToSkip};

  this->worker = std::thread(&doWork, info);
}

void SelectFile::WaitUntilDone() {
  worker.join();
  delete this->info;
  this->info = nullptr;
}

void SelectFile::Use_n_Pages(int runlen) {}