#ifndef ORDERMAKER_H
#define ORDERMAKER_H

#include "../db_core/Schema.h"
#include <istream>

// This structure encapsulates a sort order for records
class OrderMaker {

public:
  int numAtts;
  int whichAtts[MAX_ANDS];
  Type whichTypes[MAX_ANDS];

  // creates an empty OrdermMaker
  OrderMaker();

  // create an OrderMaker that can be used to sort records
  // based upon ALL of their attributes
  OrderMaker(Schema *schema);

  // print to the screen
  void Print();

  friend std::istream &operator>>(std::istream &is, OrderMaker &o); 

  friend std::ostream &operator<<(std::ostream &os, OrderMaker &o); 
};

#endif