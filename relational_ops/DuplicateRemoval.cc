#include "DuplicateRemoval.h"

void generateOrderFromSchema(Schema &schema, OrderMaker &order) {
  Attribute *atts = schema.GetAtts();

  for (int i = 0; i < schema.GetNumAtts(); i++) {
    order.numAtts++;
    order.whichAtts[i] = i;
    order.whichTypes[i] = atts[i].myType;
  }
}

void doWork(DuplicateRemovalInfo *info) {
  ComparisonEngine ce;
  OrderMaker order;
  generateOrderFromSchema(info->schema, order);

  Pipe intermediatePipe(100);

  BigQ sorter(info->inPipe, intermediatePipe, order, info->runLength);

  Record firstBuffer;
  Record secondBuffer;

  bool firstDirty = false;
  bool empty = false;

  while (!empty) {
    if (!firstDirty) {
      if (intermediatePipe.Remove(&firstBuffer)) {
        firstDirty = true;
      } else {
        break;
      }
    }

    while (!empty) {
      if (intermediatePipe.Remove(&secondBuffer)) {

        // if it does equal 0, we throw out the secondBuffer record
        if (ce.Compare(&firstBuffer, &secondBuffer, &order) != 0) {
          info->outPipe.Insert(&firstBuffer);
          firstBuffer = secondBuffer;
          break;
        }
      } else {
        empty = true;
        info->outPipe.Insert(&firstBuffer);
      }
    }
  }

  info->outPipe.ShutDown();
}

void DuplicateRemoval::Run(Pipe &inPipe, Pipe &outPipe, Schema &mySchema) {
  this->info =
      new DuplicateRemovalInfo{inPipe, outPipe, mySchema, this->runLength};

  this->worker = std::thread(&doWork, info);
}

void DuplicateRemoval::WaitUntilDone() {
  worker.join();
  delete this->info;
  this->info = nullptr;
}

void DuplicateRemoval::Use_n_Pages(int n) { this->runLength = n; }