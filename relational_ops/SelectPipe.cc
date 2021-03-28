#include "SelectPipe.h"

void SelectPipe::doWork() {
  Record buffer;
  ComparisonEngine ce;

  while (inPipe.Remove(&buffer)) {
    if (ce.Compare(&buffer, &this->literal, &this->selection)) {
      outPipe.Insert(&buffer);
    }
  }
}

void SelectPipe::Run(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal) {
  this->inPipe = inPipe;
  this->outPipe = outPipe;
  this->selection = selOp;
  this->literal = literal;

  this->worker = std::thread(&SelectPipe::doWork, this);
}

void SelectPipe::WaitUntilDone() {
  worker.join();
}
void SelectPipe::Use_n_Pages(int n) {}