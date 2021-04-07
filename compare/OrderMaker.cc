#include "OrderMaker.h"

OrderMaker ::OrderMaker() { numAtts = 0; }

OrderMaker ::OrderMaker(Schema *schema) {
  numAtts = 0;

  int n = schema->GetNumAtts();
  Attribute *atts = schema->GetAtts();

  for (int i = 0; i < n; i++) {
    if (atts[i].myType == Int) {
      whichAtts[numAtts] = i;
      whichTypes[numAtts] = Int;
      numAtts++;
    }
  }

  // now add in the doubles
  for (int i = 0; i < n; i++) {
    if (atts[i].myType == Double) {
      whichAtts[numAtts] = i;
      whichTypes[numAtts] = Double;
      numAtts++;
    }
  }

  // and finally the strings
  for (int i = 0; i < n; i++) {
    if (atts[i].myType == String) {
      whichAtts[numAtts] = i;
      whichTypes[numAtts] = String;
      numAtts++;
    }
  }
}

void OrderMaker ::Print() {
  printf("NumAtts = %5d\n", numAtts);
  for (int i = 0; i < numAtts; i++) {
    printf("%3d: %5d ", i, whichAtts[i]);
    if (whichTypes[i] == Int)
      printf("Int\n");
    else if (whichTypes[i] == Double)
      printf("Double\n");
    else
      printf("String\n");
  }
}

std::istream &operator>>(std::istream &is, OrderMaker &o) {
  is >> o.numAtts;

  for (int i = 0; i < o.numAtts; i++) {
    is >> o.whichAtts[i];
    int temp;

    is >> temp;

    o.whichTypes[i] = static_cast<Type>(temp);
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, OrderMaker &o) {
  os << o.numAtts;

  for (int i = 0; i < o.numAtts; i++) {
    os << " " << o.whichAtts[i] << " ";
    os << o.whichTypes[i];
  }
  return os;
}
