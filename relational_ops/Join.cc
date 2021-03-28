#include "Join.h"

void doWork(JoinInfo *info) {
  ComparisonEngine ce;

  OrderMaker orderLeft;
  OrderMaker orderRight;

  info->selOp.GetSortOrders(orderLeft, orderRight);

  int totalAtts = info->leftAtts + info->rightAtts;

  int *toKeep = new int[totalAtts];
  int j = 0;
  for (int i = 0; i < totalAtts; i++) {
    if (i < info->leftAtts) {
      toKeep[i] = i;
    } else {
      toKeep[i] = j;
      j++;
    }
  }

  // then we have an equality check that makes this a valid sort-merge join
  if (orderLeft.numAtts > 0) {
    const int BUFFER_SIZE = 100;

    Pipe outPipeL(BUFFER_SIZE);
    Pipe outPipeR(BUFFER_SIZE);

    BigQ leftBigQ(info->inPipeL, outPipeL, orderLeft, info->runLength);
    BigQ rightBigQ(info->inPipeR, outPipeR, orderRight, info->runLength);

    Record leftBuffer;
    Record rightBuffer;

    bool leftDirty = false;
    bool rightDirty = false;

    bool leftEmpty = false;
    bool rightEmpty = false;

    while (!leftEmpty && !rightEmpty) {
      if (!leftDirty) {
        if (outPipeL.Remove(&leftBuffer)) {
          leftDirty = true;
        } else {
          leftEmpty = true;
          break;
        }
      }

      if (!rightDirty) {
        if (outPipeR.Remove(&rightBuffer)) {
          rightDirty = true;
        } else {
          rightEmpty = true;
          break;
        }
      }

      int comp = ce.Compare(&leftBuffer, &orderLeft, &rightBuffer, &orderRight);
      if (comp == 0) {
        Record res;
        res.MergeRecords(&leftBuffer, &rightBuffer, info->leftAtts,
                         info->rightAtts, toKeep, totalAtts, info->leftAtts);
        info->outPipe.Insert(&res);
      }

      if (comp >= 0) {
        rightDirty = false;
      } else {
        leftDirty = false;
      }
    }
    if (!leftEmpty) {
      while (outPipeL.Remove(&leftBuffer)) {
      }
    }

    if (!rightEmpty) {
      std::cout << "more in right!" << std::endl;
      while (outPipeR.Remove(&rightBuffer)) {
      }
    }
  }

  delete[] toKeep;
  info->outPipe.ShutDown();
}

void Join::Run(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp,
               Record &literal, int leftAtts, int rightAtts) {
  this->info = new JoinInfo{inPipeL, inPipeR,         outPipe,  selOp,
                            literal, this->runLength, leftAtts, rightAtts};

  this->worker = std::thread(&doWork, this->info);
}

void Join::WaitUntilDone() {
  worker.join();  
  delete this->info;
  this->info = nullptr;
}

void Join::Use_n_Pages(int n) { this->runLength = n; }