#include "Count.h"

void insertGroup(CountInfo &info, Record &groupRecord, int numInGroup,
                 int *toKeep) {

  Record resLiteral;
  resLiteral.ConstructLiteral(numInGroup, 0.0, true);

  Record result;

  result.MergeRecords(&resLiteral, &groupRecord, 1, info.totalAtts, toKeep,
                      info.groupAtts.numAtts + 1, 1);

  info.outPipe.Insert(&result);
}

void doWork(CountInfo *info) {
  ComparisonEngine ce;
  Pipe tempPipe(100);
  BigQ sorter(info->inPipe, tempPipe, info->groupAtts, info->runLength);

  Record buffer;
  Record recordToCompare;

  int *toKeep = new int[info->groupAtts.numAtts + 1];
  toKeep[0] = 0;
  for (int i = 0; i < info->groupAtts.numAtts; i++) {
    toKeep[i + 1] = info->groupAtts.whichAtts[i];
  }

  if (!tempPipe.Remove(&recordToCompare)) {
    info->outPipe.ShutDown();
    return;
  }

  int numInGroup = 0;
  while (tempPipe.Remove(&buffer)) {

    // this implies we are in the same group, since the records are sorted by
    // attribute
    if (ce.Compare(&buffer, &recordToCompare, &info->groupAtts) == 0) {
      numInGroup++;
    } else {

      insertGroup(*info, recordToCompare, numInGroup, toKeep);
      numInGroup = 0;
      // "buffer" becomes the start of the new group
      recordToCompare = buffer;
    }
  }

  // WE INSERT LAST RECORD HERE INTO SUM AND MAKE ONE LAST GROUP TUPLE
  // add recordToCompare to Sum

  numInGroup++;
  insertGroup(*info, recordToCompare, numInGroup, toKeep);

  info->outPipe.ShutDown();
}

void Count::Run(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts,
                int totalAtts) {
  this->info =
      new CountInfo{inPipe, outPipe, groupAtts, totalAtts, this->runLength};
  this->worker = std::thread(&doWork, this->info);
}

void Count::WaitUntilDone() {
  worker.join();
  delete info;
  info = nullptr;
}

void Count::Use_n_Pages(int n) { this->runLength = n; }