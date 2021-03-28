#include "GroupBy.h"

void insertGroup(GroupByInfo &info, Record &groupRecord, bool isInt,
                 int intResult, double doubleResult, int *toKeep) {

  Record resLiteral;
  resLiteral.ConstructLiteral(intResult, doubleResult, isInt);

  Record result;

  result.MergeRecords(&resLiteral, &groupRecord, 1, info.totalAtts, toKeep,
                      info.groupAtts.numAtts + 1, 1);

  info.outPipe.Insert(&result);
}

void doWork(GroupByInfo *info) {
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

  int intResult = 0;
  double doubleResult = 0.0;
  bool isInt = true;
  int cnt = 0;
  while (tempPipe.Remove(&buffer)) {
    int tempInt = 0;
    double tempDouble = 0.0;

    // this implies we are in the same group, since the records are sorted by
    // attribute
    if (ce.Compare(&buffer, &recordToCompare, &info->groupAtts) == 0) {
      isInt = (info->computeMe.Apply(buffer, tempInt, tempDouble) == Int);
      isInt ? intResult += tempInt : doubleResult += tempDouble;
    } else {

      // add recordToCompare to Sum
      isInt =
          (info->computeMe.Apply(recordToCompare, tempInt, tempDouble) == Int);

      isInt ? intResult += tempInt : doubleResult += tempDouble;

      insertGroup(*info, recordToCompare, isInt, intResult, doubleResult,
                  toKeep);

      intResult = 0;
      doubleResult = 0.0;

      // "buffer" becomes the start of the new group
      recordToCompare = buffer;
      cnt++;
    }
  }

  int tempInt = 0;
  double tempDouble = 0.0;
  // WE INSERT LAST RECORD HERE INTO SUM AND MAKE ONE LAST GROUP TUPLE
  // add recordToCompare to Sum
  info->computeMe.Apply(recordToCompare, tempInt, tempDouble);
  isInt ? intResult += tempInt : doubleResult += tempDouble;
  insertGroup(*info, recordToCompare, isInt, intResult, doubleResult, toKeep);

  info->outPipe.ShutDown();
}

void GroupBy::Run(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts,
                  int totalAtts, Function &computeMe) {
  this->info = new GroupByInfo{inPipe,    outPipe,   groupAtts,
                               totalAtts, computeMe, this->runLength};
  this->worker = std::thread(&doWork, this->info);
}

void GroupBy::WaitUntilDone() {
  worker.join();
  delete info;
  info = nullptr;
}

void GroupBy::Use_n_Pages(int n) { this->runLength = n; }