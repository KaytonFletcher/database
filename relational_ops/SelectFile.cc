#include "SelectFile.h"

void doWork(SelectFileInfo *info) {
  Record buffer;
  while (info->inFile.GetNext(buffer, info->selection, info->literal)) {
    info->outPipe.Insert(&buffer);
  }
  info->outPipe.ShutDown();
}

void SelectFile::Run(DBFile &inFile, Pipe &outPipe, CNF &selOp,
                     Record &literal) {
  this->info = new SelectFileInfo{inFile, outPipe, selOp, literal};

  this->worker = std::thread(&doWork, info);
}

void SelectFile::WaitUntilDone() {
  worker.join();

  delete this->info;
  this->info = nullptr;
}

void SelectFile::Use_n_Pages(int runlen) {}