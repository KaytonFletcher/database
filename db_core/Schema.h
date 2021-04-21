
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

  Attribute() = default;
  Attribute(const Attribute &other) {

    if (other.name != nullptr) {
      this->name = strdup(other.name);
    }

    this->myType = other.myType;
  }

  Attribute &operator=(const Attribute &other) {
    if (this->name != nullptr) {
      free(name);
    }

    if (other.name != nullptr) {
      this->name = strdup(other.name);
    }

    this->myType = other.myType;
    return *this;
  }

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

  void Print(const std::string& aliasRelName) const;

  // this reads the specification for the schema in from a file
  Schema(const char *fName, const char *relName);

  // this composes a schema instance in-memory
  Schema(const char *fName, int num_atts, Attribute **atts);

  Schema(const Schema &other) {
    this->numAtts = other.numAtts;
    this->fileName = strdup(other.fileName);

    this->myAtts = new Attribute[this->numAtts];
    for (int i = 0; i < this->numAtts; i++) {
      this->myAtts[i] = other.myAtts[i];
    }
  }

  Schema &operator=(const Schema &other) {
    delete[] this->myAtts;
    free((void *)fileName);

    this->numAtts = other.numAtts;
    this->fileName = strdup(other.fileName);

    this->myAtts = new Attribute[this->numAtts];
    for (int i = 0; i < this->numAtts; i++) {
      this->myAtts[i] = other.myAtts[i];
    }
    return *this;
  }

  // this constructs a sort order structure that can be used to
  // place a lexicographic ordering on the records using this type of schema
  // int GetSortOrder(OrderMaker &order);

  ~Schema();
};

#endif
