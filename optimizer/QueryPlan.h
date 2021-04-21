#pragma once

#include <unordered_map>

#include "../optionals/Relation.h"
#include "../statistics/StatsHelper.h"
#include "../tpch/Generator.h"
#include "Node.h"
#include "SelectFileNode.h"

extern struct FuncOperator *finalFunction;
extern struct TableList *tables;
extern struct AndList *whereList;
extern struct NameList *groupingAtts;
extern struct NameList *attsToSelect;
extern int distinctAtts;
extern int distinctFunc;

class QueryPlan {
private:
  void Print(Node *node) {
    if (!node)
      return;

    this->Print(node->GetLeft());
    this->Print(node->GetRight());

    node->Print();
  }

  Node *root;

  std::unordered_map<std::string, Schema *> schemas;
  std::unordered_map<std::string, std::string> aliases;

public:
  QueryPlan() { TPCH::ParseConfigFile(schemas); }

  void GenerateSelectFiles() {
    int numTables = 0;

    while (tables) {
      aliases.insert({tables->aliasAs, tables->tableName});
      Node *node = new SelectFileNode(*this->schemas.at(tables->tableName),
                                      ++numTables, tables->aliasAs);

      // std::cout << "Table name: " << tables->tableName << std::endl;
      // std::cout << "Table alias: " << tables->aliasAs << std::endl;

      node->Print();

      tables = tables->next;
    }
  }

  void GenerateSelectPipes() {
    while (whereList) {
      struct OrList *orList = whereList->left;
      while (orList) {

        std::string attrName;
        std::string aliasRelName;
        orList->left.

            orList = orList->rightOr;
      }
      whereList = whereList->rightAnd;
    }
  }
  void Print() { this->Print(this->root); }
};