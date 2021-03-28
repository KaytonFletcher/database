#include "WriteOut.h"

void doWork(WriteOutInfo* info) {
  Record buffer;
  while(info->inPipe.Remove(&buffer)) {
    buffer.WriteOut(&info->mySchema, info->outFile);
  }
}

void WriteOut::Run(Pipe &inPipe, FILE *outFile, Schema &mySchema) {
  this->info = new WriteOutInfo{inPipe, outFile, mySchema, this->runLength};
  this->worker = std::thread(&doWork, info);
}

void WriteOut::WaitUntilDone() {
  worker.join();
  delete this->info;
  this->info = nullptr;
}

void WriteOut::Use_n_Pages(int n) { this->runLength = n; }