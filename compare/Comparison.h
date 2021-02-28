#ifndef COMPARISON_H
#define COMPARISON_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../db_core/Defs.h"
#include "../db_core/Record.h"

// This stores an individual comparison that is part of a CNF
class Comparison {

  friend class ComparisonEngine;
  friend class CNF;

  Target operand1;
  int whichAtt1;
  Target operand2;
  int whichAtt2;

  Type attType;

  CompOperator op;

public:
  Comparison();

  // copy constructor
  Comparison(const Comparison &copyMe);

  // print to the screen
  void Print();
};

#endif
