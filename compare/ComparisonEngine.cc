#include "ComparisonEngine.h"
#include "OrderMaker.h"

// returns a -1, 0, or 1 depending upon whether left is less then, equal to, or
// greater than right, depending upon the OrderMaker
int ComparisonEngine ::Compare(Record *left, Record *right,
                               OrderMaker *orderUs) const {

  char *val1, *val2;

  char *left_bits = left->GetBits();
  char *right_bits = right->GetBits();

  // std::cout << "NUM ATTS: " << orderUs->numAtts << std::endl;
  // std::cout << "WHICH ATTS: " << orderUs->whichAtts[0] << std::endl;

  for (int i = 0; i < orderUs->numAtts; i++) {
    val1 = left_bits + ((int *)left_bits)[orderUs->whichAtts[i] + 1];
    val2 = right_bits + ((int *)right_bits)[orderUs->whichAtts[i] + 1];

    // these are used to store the two operands, depending on their type
    int val1Int, val2Int;
    double val1Double, val2Double;

    // now check the type and do the comparison
    switch (orderUs->whichTypes[i]) {

    // first case: we are dealing with integers
    case Int:

      // cast the two bit strings to ints
      val1Int = *((int *)val1);
      val2Int = *((int *)val2);

      // std::cout << "Val 1: " << val1Int << std::endl;
      // std::cout << "Val 2: " << val2Int << std::endl;

      // and do the comparison
      if (val1Int < val2Int)
        return -1;
      else if (val1Int > val2Int)
        return 1;

      break;

    // second case: dealing with doubles
    case Double:

      // cast the two bit strings to doubles
      val1Double = *((double *)val1);
      val2Double = *((double *)val2);

      // and do the comparison
      if (val1Double < val2Double)
        return -1;
      else if (val1Double > val2Double)
        return 1;

      break;

    // last case: dealing with strings
    default:
      int sc = strcmp(val1, val2);
      if (sc != 0)
        return sc;

      break;
    }
  }

  return 0;
}

// returns a -1, 0, or 1 depending upon whether left is less then, equal to, or
// greater than right, depending upon the OrderMakers that are passed in.  This
// one is used for joins, where you have to compare records *across* two input
// tables
int ComparisonEngine ::Compare(Record *left, OrderMaker *order_left,
                               Record *right, OrderMaker *order_right) {

  char *val1, *val2;

  char *left_bits = left->GetBits();
  char *right_bits = right->GetBits();

  for (int i = 0; i < order_left->numAtts; i++) {
    val1 = left_bits + ((int *)left_bits)[order_left->whichAtts[i] + 1];
    val2 = right_bits + ((int *)right_bits)[order_right->whichAtts[i] + 1];

    // these are used to store the two operands, depending on their type
    int val1Int, val2Int;
    double val1Double, val2Double;

    // now check the type and do the comparison
    switch (order_left->whichTypes[i]) {

    // first case: we are dealing with integers
    case Int:

      // cast the two bit strings to ints
      val1Int = *((int *)val1);
      val2Int = *((int *)val2);

      // and do the comparison
      if (val1Int < val2Int)
        return -1;
      else if (val1Int > val2Int)
        return 1;

      break;

    // second case: dealing with doubles
    case Double:

      // cast the two bit strings to doubles
      val1Double = *((double *)val1);
      val2Double = *((double *)val2);

      // and do the comparison
      if (val1Double < val2Double)
        return -1;
      else if (val1Double > val2Double)
        return 1;

      break;

    // last case: dealing with strings
    default:
      int sc = strcmp(val1, val2);
      if (sc != 0)
        return sc;

      break;
    }
  }

  return 0;
}

// Here we apply a CNF to a single record; this function either returns true or
// false dpending upon wheter or not the CNF expression accepts the record
int ComparisonEngine ::Compare(Record *left, Record *literal,
                               CNF *myComparison) {

  for (int i = 0; i < myComparison->numAnds; i++) {

    for (int j = 0; j < myComparison->orLens[i]; j++) {

      // this returns a 0 if the comparison did not eval to true
      int result = Run(left, literal, &myComparison->orList[i][j]);

      if (result != 0) {
        break;
      }

      // if we made it through all of the comparisons without a hit, return a 0
      if (j == myComparison->orLens[i] - 1) {
        return 0;
      }
    }

    // this ends the for-loop for the OR-list... if we made it this far, then
    // we'll go on to the next disjunction in the overall CNF expression
  }

  // this ends the for-loop for the AND-list... if we made it this far, then we
  // know that we have satisfied the overall CNF expression
  return 1;
}

// this is just like the last one, except that it deals with a pair of records
int ComparisonEngine ::Compare(Record *left, Record *right, Record *literal,
                               CNF *myComparison) {

  for (int i = 0; i < myComparison->numAnds; i++) {

    for (int j = 0; j < myComparison->orLens[i]; j++) {

      // this returns a 0 if the comparison did not eval to true
      int result = Run(left, right, literal, &myComparison->orList[i][j]);

      if (result != 0) {
        break;
      }

      // if we made it through all of the comparisons without a hit, return a 0
      if (j == myComparison->orLens[i] - 1) {
        return 0;
      }
    }

    // this ends the for-loop for the OR-list... if we made it this far, then
    // we'll go on to the next disjunction in the overall CNF expression
  }

  // this ends the for-loop for the AND-list... if we made it this far, then we
  // know that we have satisfied the overall CNF expression
  return 1;
}

int ComparisonEngine ::Compare(Record *left, Record *literal, CNF *comp,
                               OrderMaker *query) {
  char *left_bits = left->GetBits();
  char *lit_bits = literal->GetBits();

  for (int i = 0; i < query->numAtts; i++) {

    for (int j = 0; j < comp->numAnds; j++) {
      if (comp->orLens[j] == 1 && comp->orList[j][0].op == Equals) {
        Comparison &comparison = comp->orList[j][0];
        char *val1, *val2;

        if (comparison.operand1 == Left &&
            comparison.whichAtt1 == query->whichAtts[i]) {
          val1 = left_bits + ((int *)left_bits)[comparison.whichAtt1 + 1];
          val2 = lit_bits + ((int *)lit_bits)[comparison.whichAtt2 + 1];
          int res = helperComp(val1, val2, &comparison);
          if (res != 0) {
            return res;
          }

          break;
        }

        if (comparison.operand2 == Left &&
            comparison.whichAtt2 == query->whichAtts[i]) {

          val1 = left_bits + ((int *)left_bits)[comparison.whichAtt2 + 1];
          val2 = lit_bits + ((int *)lit_bits)[comparison.whichAtt1 + 1];

          int res = helperComp(val2, val1, &comparison);
          if (res != 0) {
            return res;
          }

          break;
        }
      }

      if (j == comp->numAnds - 1) {
        std::cout << "THIS SHOULD NEVER HAPPEN!" << std::endl;
        exit(1);
      }
    }
  }

  return 0;
}

int ComparisonEngine::helperComp(char *val1, char *val2, Comparison *c) {
  int val1Int, val2Int, tempResult;
  double val1Double, val2Double;

  // now check the type and the comparison operation
  switch (c->attType) {

  // first case: we are dealing with integers
  case Int:

    if (val1Int < val2Int) {
      return -1;
    } else {
      return (val1Int < val2Int);
    }
    break;

  // second case: dealing with doubles
  case Double:
    val1Double = *((double *)val1);
    val2Double = *((double *)val2);

    if (val1Double < val2Double) {
      return -1;
    } else {
      return (val1Double < val2Double);
    }
    break;

  // final case: dealing with strings
  default:

    // so check the operation type in order to actually do the comparison
    return strcmp(val1, val2);
    break;
  }
}

// This is an internal function used by the comparison engine
int ComparisonEngine ::Run(Record *left, Record *literal, Comparison *c) {

  char *val1, *val2;

  char *left_bits = left->GetBits();
  char *lit_bits = literal->GetBits();

  // first get a pointer to the first value to compare
  if (c->operand1 == Left) {
    // std::cout << "Which atts attribute: " << c->whichAtt1 << std::endl;
    val1 = left_bits + ((int *)left_bits)[c->whichAtt1 + 1];
  } else {
    val1 = lit_bits + ((int *)lit_bits)[c->whichAtt1 + 1];
  }

  // next get a pointer to the second value to compare
  if (c->operand2 == Left) {
    val2 = left_bits + ((int *)left_bits)[c->whichAtt2 + 1];
  } else {
    // std::cout << "WHICHATTS RUN: " << c->whichAtt2 << std::endl;
    val2 = lit_bits + ((int *)lit_bits)[c->whichAtt2 + 1];
  }

  int val1Int, val2Int, tempResult;
  double val1Double, val2Double;

  // now check the type and the comparison operation
  switch (c->attType) {

  // first case: we are dealing with integers
  case Int:

    val1Int = *((int *)val1);
    val2Int = *((int *)val2);

    // std::cout << "Val 1 Run: " << val1Int << std::endl;
    // std::cout << "Val 2 Run: " << val2Int << std::endl;

    // and check the operation type in order to actually do the comparison
    switch (c->op) {

    case LessThan:
      return (val1Int < val2Int);
      break;

    case GreaterThan:
      return (val1Int > val2Int);
      break;

    default:
      return (val1Int == val2Int);
      break;
    }
    break;

  // second case: dealing with doubles
  case Double:
    val1Double = *((double *)val1);
    val2Double = *((double *)val2);

    // and check the operation type in order to actually do the comparison
    switch (c->op) {

    case LessThan:
      return (val1Double < val2Double);
      break;

    case GreaterThan:
      return (val1Double > val2Double);
      break;

    default:
      return (val1Double == val2Double);
      break;
    }
    break;

  // final case: dealing with strings
  default:

    // so check the operation type in order to actually do the comparison
    tempResult = strcmp(val1, val2);
    switch (c->op) {

    case LessThan:
      return tempResult < 0;
      break;

    case GreaterThan:
      return tempResult > 0;
      break;

    default:
      return tempResult == 0;
      break;
    }
    break;
  }
}

// This is an internal function used by the comparison engine
int ComparisonEngine ::Run(Record *left, Record *right, Record *literal,
                           Comparison *c) {

  char *val1, *val2;

  char *left_bits = left->GetBits();
  char *right_bits = right->GetBits();
  char *lit_bits = literal->GetBits();

  // first get a pointer to the first value to compare
  switch (c->operand1) {

  case Left:
    val1 = left_bits + ((int *)left_bits)[c->whichAtt1 + 1];
    break;

  case Right:
    val1 = right_bits + ((int *)right_bits)[c->whichAtt1 + 1];
    break;

  default:
    val1 = lit_bits + ((int *)lit_bits)[c->whichAtt1 + 1];
    break;
  }

  // next get a pointer to the second value to compare
  switch (c->operand2) {

  case Left:
    val2 = left_bits + ((int *)left_bits)[c->whichAtt2 + 1];
    break;

  case Right:
    val2 = right_bits + ((int *)right_bits)[c->whichAtt2 + 1];
    break;

  default:
    val2 = lit_bits + ((int *)lit_bits)[c->whichAtt2 + 1];
    break;
  }

  int val1Int, val2Int, tempResult;
  double val1Double, val2Double;

  // now check the type and the comparison operation
  switch (c->attType) {

  // first case: we are dealing with integers
  case Int:

    val1Int = *((int *)val1);
    val2Int = *((int *)val2);

    // and check the operation type in order to actually do the comparison
    switch (c->op) {

    case LessThan:
      return (val1Int < val2Int);
      break;

    case GreaterThan:
      return (val1Int > val2Int);
      break;

    default:
      return (val1Int == val2Int);
      break;
    }
    break;

  // second case: dealing with doubles
  case Double:
    val1Double = *((double *)val1);
    val2Double = *((double *)val2);

    // and check the operation type in order to actually do the comparison
    switch (c->op) {

    case LessThan:
      return (val1Double < val2Double);
      break;

    case GreaterThan:
      return (val1Double > val2Double);
      break;

    default:
      return (val1Double == val2Double);
      break;
    }
    break;

  // final case: dealing with strings
  default:

    // so check the operation type in order to actually do the comparison
    tempResult = strcmp(val1, val2);
    switch (c->op) {

    case LessThan:
      return tempResult < 0;
      break;

    case GreaterThan:
      return tempResult > 0;
      break;

    default:
      return tempResult == 0;
      break;
    }
    break;
  }
}
