#include "Sum.h"

void doWork(SumInfo *info) {
  Record buffer;
  int intResult = 0;
  double doubleResult = 0;

  bool isInt = false;

  while (info->inPipe.Remove(&buffer)) {
    int tempInt = 0;
    double tempDouble = 0;

    if (info->function.Apply(buffer, tempInt, tempDouble) == Int) {
      isInt = true;
      intResult += tempInt;
    } else {
      doubleResult += tempDouble;
    }
  }

  Record result;
  result.ConstructLiteral(intResult, doubleResult, isInt);

  info->outPipe.Insert(&result);
  info->outPipe.ShutDown();
}

void Sum::Run(Pipe &inPipe, Pipe &outPipe, Function &computation) {

  this->info = new SumInfo{inPipe, outPipe, computation, this->runLength};
  this->worker = std::thread(&doWork, info);
}

void Sum::WaitUntilDone() {
  worker.join();
  delete this->info;
  this->info = nullptr;
}

void Sum::Use_n_Pages(int n) { this->runLength = n; }