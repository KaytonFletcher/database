#ifndef BIGQ_H
#define BIGQ_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

#include "File.h"
#include "Pipe.h"
#include "Record.h"

using namespace std;

class BigQ {

  Pipe &in;
  Pipe &out;
  OrderMaker &sortorder;
  int runlen;
  ComparisonEngine ce;
  File file;
  std::thread *worker;

public:
  BigQ(Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
  ~BigQ();
};

#endif
