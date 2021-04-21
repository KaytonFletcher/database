#include "Print.h"

extern struct FuncOperator *finalFunction;
extern struct TableList *tables;
extern struct AndList *whereList;
extern struct NameList *groupingAtts;
extern struct NameList *attsToSelect;
extern int distinctAtts;
extern int distinctFunc;

std::string getCode(int code) {
  switch (code) {
  case 1:
    return "Double";
  case 2:
    return "Int";
  case 3:
    return "Name";
  case 4:
    return "String";
  case 5:
    return "Less Than";
  case 6:
    return "Greater Than";
  case 7:
    return "Equals";
  default:
    return "Not Found";
  }
}

void printFunc(FuncOperator *func, bool left, bool right) {
  if (!func)
    return;

  if (left)
    printFunc(func->leftOperator, true, false);

  std::cout << "bruh" << func->code << std::endl;

  if (func->code) {
    std::cout << "Function code: " << (char)func->code << std::endl;
  } else {
    if (func->leftOperand) {
      std::cout << "Left Code: " << getCode(func->leftOperand->code)
                << std::endl;
    } else {
      std::cout << "WHYYYY" << std::endl;
    }
  }

  if (right)
    printFunc(func->right, false, true);
}

void PrintParsedSQL() {

  while (tables != nullptr) {
    std::cout << "Table name: " << tables->tableName << std::endl;
    std::cout << "Table alias: " << tables->aliasAs << std::endl;

    tables = tables->next;
  }

  while (whereList) {
    struct OrList *myOr = whereList->left;
    while (myOr) {
      std::cout << "-- Or --" << std::endl;
      std::cout << "Left: " << getCode(myOr->left->left->code) << " "
                << getCode(myOr->left->code)
                << " Right: " << getCode(myOr->left->right->code) << std::endl;

      myOr = myOr->rightOr;
    }

    whereList = whereList->rightAnd;
  }

  if (distinctAtts) {
    std::cout << "-- Only Distinct Attributes --" << std::endl;
  } else {
    std::cout << "-- All Attributes --" << std::endl;
  }

  while (attsToSelect) {
    std::cout << "Attributes to Select: " << attsToSelect->name << std::endl;
    attsToSelect = attsToSelect->next;
  }

  while (groupingAtts) {
    std::cout << "Group By Attribute: " << groupingAtts->name << std::endl;
    groupingAtts = groupingAtts->next;
  }

  if (distinctFunc) {
    std::cout << "-- Select Distinct --" << std::endl;
  } else {
    std::cout << "-- Select All --" << std::endl;
  }

  printFunc(finalFunction, true, true);
}
