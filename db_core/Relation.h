#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../compare/CNF.h"
#include "../db_file/DBFile.h"
#include "Schema.h"

extern "C" {
int yyparse(void); // defined in y.tab.c
}

extern struct AndList *final;

class Relation {

private:
  const char *rname;
  const char *prefix;
  char rpath[100];
  Schema *rschema;

public:
  Relation(const char *_name, Schema *_schema, const char *_prefix)
      : rname(_name), prefix(_prefix), rschema(_schema) {
    sprintf(rpath, "%s%s.bin", prefix, rname);
  }
  const char *name() { return rname; }
  const char *path() { return rpath; }
  Schema *schema() { return rschema; }

  void info();

  void get_cnf(CNF &cnf_pred, Record &literal);

  void get_sort_order(OrderMaker &sortorder);

  ~Relation() { delete rschema; }
};
