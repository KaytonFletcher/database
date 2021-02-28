#pragma once

#include <cmath>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "compare/CNF.h"
#include "db_core/BigQ.h"
#include "db_core/Pipe.h"
#include "db_core/Record.h"
#include "db_core/Relation.h"
#include "db_file/DBFile.h"

extern "C" {
int yyparse(void); // defined in y.tab.c
}

extern struct AndList *final;

typedef struct {
  Pipe *pipe;
  OrderMaker *order;
  bool print;
  bool write;
} testutil;

class Test {
public:
  Test()
      : supplier("supplier"), partsupp("partsupp"), part("part"),
        nation("nation"), customer("customer"), orders("orders"),
        region("region"), lineitem("lineitem") {

    std::cout
        << " \n** IMPORTANT: MAKE SURE THE INFORMATION BELOW IS CORRECT **\n";
    std::cout << " catalog location: \t" << catalog_path << std::endl;
    std::cout << " tpch files dir: \t" << tpch_dir << std::endl;
    std::cout << " heap files dir: \t" << dbfile_dir << std::endl;
    std::cout << " \n\n";
  }

  const std::string supplier = "supplier";
  const std::string partsupp = "partsupp";
  const std::string part = "part";
  const std::string nation = "nation";
  const std::string customer = "customer";
  const std::string orders = "orders";
  const std::string region = "region";
  const std::string lineitem = "lineitem";

  // This file is used to parse the database schemas
  inline static const std::string catalog_path = "catalog";

  // dir where dbgen tpch files (extension *.tbl) can be found
  inline static const std::string dbfile_dir = "";

  inline static const std::string tpch_dir = "./p1/tables/";

  static Relation *rel;

  Relation *relations[8] = { // nation
      new Relation(nation.c_str(),
                   new Schema(catalog_path.c_str(), nation.c_str()),
                   dbfile_dir.c_str()),
      // region
      new Relation(region.c_str(),
                   new Schema(catalog_path.c_str(), region.c_str()),
                   dbfile_dir.c_str()),
      // customer
      new Relation(customer.c_str(),
                   new Schema(catalog_path.c_str(), customer.c_str()),
                   dbfile_dir.c_str()),
      // part
      new Relation(part.c_str(), new Schema(catalog_path.c_str(), part.c_str()),
                   dbfile_dir.c_str()),
      // partsupplier
      new Relation(partsupp.c_str(),
                   new Schema(catalog_path.c_str(), partsupp.c_str()),
                   dbfile_dir.c_str()),
      // orders
      new Relation(orders.c_str(),
                   new Schema(catalog_path.c_str(), orders.c_str()),
                   dbfile_dir.c_str()),
      // lineitem
      new Relation(lineitem.c_str(),
                   new Schema(catalog_path.c_str(), lineitem.c_str()),
                   dbfile_dir.c_str()),

      new Relation(supplier.c_str(),
                   new Schema(catalog_path.c_str(), supplier.c_str()),
                   dbfile_dir.c_str())};

  void cleanup() {
    for (int i = 0; i < 8; i++) {
      delete relations[i];
    }
  }
};
