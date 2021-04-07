#pragma once

#include <cmath>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unordered_map>

#include "compare/CNF.h"
#include "db_core/BigQ.h"
#include "db_core/Function.h"
#include "db_core/Pipe.h"
#include "db_core/Record.h"
#include "optionals/Relation.h"

#include "db_file/DBFile.h"

#include "relational_ops/DuplicateRemoval.h"
#include "relational_ops/GroupBy.h"
#include "relational_ops/Join.h"
#include "relational_ops/Project.h"
#include "relational_ops/RelOp.h"
#include "relational_ops/SelectFile.h"
#include "relational_ops/SelectPipe.h"
#include "relational_ops/Sum.h"
#include "relational_ops/WriteOut.h"

extern struct AndList *final;
extern struct FuncOperator *finalfunc;
extern FILE *yyin;

typedef struct {
  Pipe *pipe;
  OrderMaker *order;
  bool print;
  bool write;
} testutil;

class DBTest {
public:
  DBTest() {

    FILE *fp = fopen(settings.c_str(), "r");
    if (fp) {
      char *mem = (char *)malloc(80 * 3);
      char *temp_catalog_path = &mem[0];
      char *temp_dbfile_dir = &mem[80];
      char *temp_tpch_dir = &mem[160];
      char line[80];
      fgets(line, 80, fp);
      sscanf(line, "%s\n", temp_catalog_path);
      fgets(line, 80, fp);
      sscanf(line, "%s\n", temp_dbfile_dir);
      fgets(line, 80, fp);
      sscanf(line, "%s\n", temp_tpch_dir);
      fclose(fp);
      if (!(temp_catalog_path && temp_dbfile_dir && temp_tpch_dir)) {
        std::cerr << " Test settings file 'test.cat' not in correct format.\n";
        free(mem);
        exit(1);
      } else {
        catalog_path = temp_catalog_path;
        dbfile_dir = temp_dbfile_dir;
        tpch_dir = temp_tpch_dir;
        free(mem);
      }
    } else {
      std::cerr << " Test settings files 'test.cat' missing \n";
      exit(1);
    }
    std::cout
        << " \n** IMPORTANT: MAKE SURE THE INFORMATION BELOW IS CORRECT **\n";
    std::cout << " catalog location: \t" << catalog_path << std::endl;
    std::cout << " tpch files dir: \t" << tpch_dir << std::endl;
    std::cout << " heap files dir: \t" << dbfile_dir << std::endl;
    std::cout << " \n\n";

    relations = {
        {"nation",
         new Relation(nation.c_str(),
                      new Schema(catalog_path.c_str(), nation.c_str()),
                      dbfile_dir.c_str(), pipesz)},
        {"region",
         new Relation(region.c_str(),
                      new Schema(catalog_path.c_str(), region.c_str()),
                      dbfile_dir.c_str(), pipesz)},
        {"customer",
         new Relation(customer.c_str(),
                      new Schema(catalog_path.c_str(), customer.c_str()),
                      dbfile_dir.c_str(), pipesz)},
        {"part", new Relation(part.c_str(),
                              new Schema(catalog_path.c_str(), part.c_str()),
                              dbfile_dir.c_str(), pipesz)},
        {"partsupp",
         new Relation(partsupp.c_str(),
                      new Schema(catalog_path.c_str(), partsupp.c_str()),
                      dbfile_dir.c_str(), pipesz)},
        {"supplier",
         new Relation(supplier.c_str(),
                      new Schema(catalog_path.c_str(), supplier.c_str()),
                      dbfile_dir.c_str(), pipesz)},
        {"orders",
         new Relation(orders.c_str(),
                      new Schema(catalog_path.c_str(), orders.c_str()),
                      dbfile_dir.c_str(), pipesz)},
        {"lineitem",
         new Relation(lineitem.c_str(),
                      new Schema(catalog_path.c_str(), lineitem.c_str()),
                      dbfile_dir.c_str(), pipesz)}};
  }

  int pipesz = 100; // buffer sz allowed for each pipe

  inline static const std::string settings = "test.cat";

  const std::string nation = "nation";
  const std::string region = "region";
  const std::string customer = "customer";
  const std::string part = "part";
  const std::string partsupp = "partsupp";
  const std::string supplier = "supplier";
  const std::string orders = "orders";
  const std::string lineitem = "lineitem";

  // This file is used to parse the database schemas
  inline static std::string catalog_path = "catalog";

  // dir where .bin files created by DBFile class can be found or created
  inline static std::string dbfile_dir = "";

  // dir where dbgen tpch files (extension *.tbl) can be found
  inline static std::string tpch_dir = "./p1/tables/";

  inline static Relation *rel = nullptr;

  std::unordered_map<std::string, Relation *> relations;

  void cleanup() {
    for (auto tuple : relations) {
      delete tuple.second;
    }
  }
};
