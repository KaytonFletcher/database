#ifndef CNF_H
#define CNF_H

#include "Comparison.h"
#include "OrderMaker.h"
#include "Record.h"
#include "Schema.h"

// This structure stores a CNF expression that is to be evaluated
// during query execution

class CNF {

  friend class ComparisonEngine;

  Comparison orList[MAX_ANDS][MAX_ORS];

  int orLens[MAX_ANDS];
  int numAnds;

public:
  // this returns an instance of the OrderMaker class that
  // allows the CNF to be implemented using a sort-based
  // algorithm such as a sort-merge join.  Returns a 0 if and
  // only if it is impossible to determine an acceptable ordering
  // for the given comparison
  int GetSortOrders(OrderMaker &left, OrderMaker &right);

  // this returns an instance of the OrderMaker class that
  // allows the CNF to be implemented using a sort-based
  // algorithm such as a sort-merge join.  Returns a 0 if and
  // only if it is impossible to determine an acceptable ordering
  // for the given comparison
  int GetSortOrders(OrderMaker &order_maker);

  // print the comparison structure to the screen
  void Print();

  // this takes a parse tree for a CNF and converts it into a 2-D
  // matrix storing the same CNF expression.  This function is applicable
  // specifically to the case where there are two relations involved
  void GrowFromParseTree(struct AndList *parseTree, Schema *leftSchema,
                         Schema *rightSchema, Record &literal);

  // version of the same function, except that it is used in the case of
  // a relational selection over a single relation so only one schema is used
  void GrowFromParseTree(struct AndList *parseTree, Schema *mySchema,
                         Record &literal);
};

#endif