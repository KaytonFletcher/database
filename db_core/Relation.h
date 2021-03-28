#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../compare/CNF.h"
#include "../db_file/DBFile.h"
#include "../relational_ops/SelectFile.h"
#include "Function.h"
#include "Schema.h"

extern struct AndList *final;
extern struct FuncOperator *finalfunc;
extern FILE *yyin;

extern "C" {
int yyparse(void);                // defined in y.tab.c
int yyfuncparse(void);            // defined in yyfunc.tab.c
void init_lexical_parser(char *); // defined in lex.yy.c (from Lexer.l)
void close_lexical_parser();      // defined in lex.yy.c
void init_lexical_parser_func(
    char *);                      // defined in lex.yyfunc.c (from Lexerfunc.l)
void close_lexical_parser_func(); // defined in lex.yyfunc.c
}

class Relation {

private:
  const char *rname;
  const char *prefix;
  char rpath[100];
  Schema *rschema;

public:
  CNF cnf;
  SelectFile sf;
  DBFile df;
  Record literal;
  Function function;
  Pipe pipe;

  Relation(const char *_name, Schema *_schema, const char *_prefix)
      : rname(_name), prefix(_prefix), rschema(_schema), pipe(0) {
    sprintf(rpath, "%s%s.bin", prefix, rname);
  }

  Relation(const char *_name, Schema *_schema, const char *_prefix,
           int numPages)
      : rname(_name), prefix(_prefix), rschema(_schema), pipe(numPages) {
    sprintf(rpath, "%s%s.bin", prefix, rname);
  }

  const char *name() { return rname; }
  const char *path() { return rpath; }
  Schema *schema() { return rschema; }

  void info();

  void get_cnf(CNF &cnf_pred, Record &literal);

  void get_cnf(char *input, CNF &cnf_pred, Record &literal);

  void get_cnf(char *input, Function &fn_pred);

  void get_cnf(char *input) {
    init_lexical_parser(input);
    if (yyparse() != 0) {
      std::cout << " Error: can't parse your CNF " << input << std::endl;
      exit(1);
    }
    cnf.GrowFromParseTree(final, schema(),
                               literal); // constructs CNF predicate
    close_lexical_parser();
  }

  void get_file_cnf(const char *fpath, CNF &cnf_pred, Record &literal);

  void get_sort_order(OrderMaker &sortorder);

  ~Relation() { delete rschema; }
};
