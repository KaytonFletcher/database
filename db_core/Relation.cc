#include "Relation.h"

void Relation::info() {
  std::cout << " relation info\n";
  std::cout << "\t name: " << name() << std::endl;
  std::cout << "\t path: " << path() << std::endl;
}

void Relation::get_cnf(CNF &cnf_pred, Record &literal) {
  std::cout << " Enter CNF predicate (when done press ctrl-D):\n\t";
  if (yyparse() != 0) {
    std::cout << "Can't parse your CNF.\n";
    exit(1);
  }
  cnf_pred.GrowFromParseTree(final, schema(),
                             literal); // constructs CNF predicate
}

void Relation::get_cnf(char *input, CNF &cnf_pred, Record &literal) {
  init_lexical_parser(input);
  if (yyparse() != 0) {
    std::cout << " Error: can't parse your CNF.\n";
    exit(1);
  }
  cnf_pred.GrowFromParseTree(final, schema(),
                             literal); // constructs CNF predicate
  close_lexical_parser();
}

void Relation::get_cnf(char *input, Function &fn_pred) {
  init_lexical_parser_func(input);
  if (yyfuncparse() != 0) {
    std::cout << " Error: can't parse your CNF.\n";
    exit(1);
  }
  fn_pred.GrowFromParseTree(finalfunc,
                            *(schema())); // constructs CNF predicate
  close_lexical_parser_func();
}

void Relation::get_file_cnf(const char *fpath, CNF &cnf_pred, Record &literal) {
  yyin = fopen(fpath, "r");
  if (yyin == NULL) {
    std::cout << " Error: can't open file " << fpath << " for parsing \n";
    exit(1);
  }
  if (yyparse() != 0) {
    std::cout << " Error: can't parse your CNF.\n";
    exit(1);
  }
  cnf_pred.GrowFromParseTree(final, schema(),
                             literal); // constructs CNF predicate
  // cnf_pred.GrowFromParseTree (final, l_schema (), r_schema (), literal); //
  // constructs CNF predicate over two relations l_schema is the left reln's
  // schema r the right's
  // cnf_pred.Print ();
}

void Relation::get_sort_order(OrderMaker &sortorder) {
  std::cout << "\n specify sort ordering (when done press ctrl-D):\n\t ";
  if (yyparse() != 0) {
    std::cout << "Can't parse your sort CNF.\n";
    exit(1);
  }
  std::cout << " \n";
  Record literal;
  CNF sort_pred;
  sort_pred.GrowFromParseTree(final, schema(),
                              literal); // constructs CNF predicate

  sort_pred.GetSortOrders(sortorder);
}