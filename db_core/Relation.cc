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