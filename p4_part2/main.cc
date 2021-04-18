
#include <iostream>

#include "../ParseTree.h"

using namespace std;

extern struct TableList *tables;
extern struct AndList *whereList;
extern struct NameList *groupingAtts;
extern struct NameList *attsToSelect;
extern int distinctAtts;
extern int distinctFunc;

extern "C" {
int yyparse(void); // defined in y.tab.c
}

int main() {
  yyparse();

  while (tables != nullptr) {
    std::cout << "Table name: " << tables->tableName << std::endl;
    std::cout << "Table alias: " << tables->aliasAs << std::endl;

    tables = tables->next;
  }

  while (whereList) {
    struct OrList *myOr = whereList->left;
    while (myOr) {
      std::cout << "Or Code: " << myOr->left->code << std::endl;
      std::cout << "Or Left Code: " << myOr->left->left->code << std::endl;
      std::cout << "Or Right Code: " << myOr->left->right->code << std::endl;

      myOr = myOr->rightOr;
    }

    whereList = whereList->rightAnd;
  }
}
