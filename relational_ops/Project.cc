#include "Project.h"

void doWork(ProjectInfo *info) {
  Record buffer;
  while (info->inPipe.Remove(&buffer)) {
    buffer.Project(info->keepMe, info->numAttsOutput, info->numAttsInput);
    info->outPipe.Insert(&buffer);
  }

  info->outPipe.ShutDown();
}

void Project::Run(Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput,
                  int numAttsOutput) {

  this->info =
      new ProjectInfo{inPipe, outPipe, keepMe, numAttsInput, numAttsOutput};
  this->worker = std::thread(&doWork, this->info);
}

void Project::WaitUntilDone() {
  worker.join();
  delete info;
  info = nullptr;
}

void Project::Use_n_Pages(int runlen) {}