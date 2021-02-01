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