#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../ParseTree.h"
#include "../db_core/Pipe.h"
#include "../db_file/DBFile.h"
#include "AttributeStat.h"
#include "Histogram.h"
#include "RelationStat.h"
#include "StatsHelper.h"

extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char *);
extern "C" int yyparse(void);
extern struct AndList *final;

class Statistics {
private:
  std::vector<RelationStat> relations;
  std::unordered_map<std::string, int> relationIndicies;

  // maps attribute name to the relation names that have the attribute
  std::unordered_map<std::string, std::unordered_set<std::string>> attributes;
  std::unordered_map<std::string, std::unordered_set<std::string>> joins;

  void validateRelNames(char **relNames, int numToJoin,
                        struct AndList *parseTree,
                        std::vector<std::unordered_set<std::string> *> &groups);

  void fetchRecordsForHistogram(std::string &dbFileName, std::string &attrName,
                                std::string &relationName, Pipe &outPipe) {
    DBFile dbfile;
    dbfile.Open((this->dbfile_dir + dbFileName).c_str());
    Pipe inPipe(100);

    std::string cnfStr = "(" + attrName + ")";
    yy_scan_string(cnfStr.c_str());
    yyparse();

    OrderMaker order;

    CNF cnf;
    Record literal;

    Schema schema(catalog_path.c_str(), relationName.c_str());

    cnf.GrowFromParseTree(final, &schema, literal);
    cnf.GetSortOrders(order);

    BigQ sorter(inPipe, outPipe, order, 512);
  }

  inline static const std::string settings = "test.cat";

  // This file is used to parse the database schemas
  inline static std::string catalog_path = "catalog";

  // dir where .bin files created by DBFile class can be found or created
  inline static std::string dbfile_dir = "";

  // dir where dbgen tpch files (extension *.tbl) can be found
  inline static std::string tpch_dir = "./";

public:
  Statistics();
  Statistics(Statistics &copyMe); // Performs deep copy
  ~Statistics();
  int NumRelations() { return this->relations.size(); }
  int NumAttributes() { return this->attributes.size(); }
  int NumRelationsWithAttribute(const std::string &attr) {
    if (this->attributes.find(attr) != this->attributes.end()) {
      return this->attributes[attr].size();
    } else {
      return 0;
    }
  }

  void AddRel(char *relName, int numTuples);
  void AddAtt(char *relName, char *attName, int numDistincts);
  void CopyRel(char *oldName, char *newName);

  void Read(char *fromWhere);
  void Write(char *fromWhere);

  void Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
  double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);

  friend std::istream &operator>>(std::istream &is, Statistics &o);
  friend std::ostream &operator<<(std::ostream &os, Statistics &o);
};