#pragma once

#include "../compare/OrderMaker.h"
#include "../db_core/Function.h"
#include "Node.h"

class GroupByNode : Node {

  OrderMaker groupAtts;
  int totalAtts;
  Function computeMe;
  int runLength;

public:
  GroupByNode();

  void Print() const {}
};