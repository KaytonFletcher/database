
#ifndef SCHEMA_H
#define SCHEMA_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Defs.h"

struct Attribute {

  char *name = nullptr;
  Type myType;

  ~Attribute() {
    // free is used because strdup requires it to deallocate memory
    free(name);
    name = nullptr;
  }
};

class Schema {

  // gives the attributes in the schema
  int numAtts;
  Attribute *myAtts;

  // gives the physical location of the binary file storing the relation
  const char *fileName;

public:
  // gets the set of attributes, but be careful with this, since it leads
  // to aliasing!!!
  Attribute *GetAtts();

  // returns the number of attributes
  int GetNumAtts();

  // this finds the position of the specified attribute in the schema
  // returns a -1 if the attribute is not present in the schema
  int Find(const char *attName);

  // this finds the type of the given attribute
  Type FindType(const char *attName);

  // this reads the specification for the schema in from a file
  Schema(const char *fName, const char *relName);

  // this constructs a sort order structure that can be used to
  // place a lexicographic ordering on the records using this type of schema
  // int GetSortOrder(OrderMaker &order);

  ~Schema();
};

#endif
