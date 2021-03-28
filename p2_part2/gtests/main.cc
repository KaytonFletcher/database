#include "../db_file/SortedDB.h"
#include "../test.h"
#include "gtest/gtest.h"

// TEST(CreateLiteral, Broken) {
//   DBTest testProgram;

//   CNF cnf;
//   Record literal;
//   DBTest::rel = testProgram.relations[0];
//   DBTest::rel->get_cnf(cnf, literal);

//   OrderMaker order;
//   // sorting on nations n_regionkey
//   order.numAtts = 1;
//   order.whichAtts[0] = 2;
//   order.whichTypes[0] = Int;

//   DBFile file;
//   file.Open("nation.bin");
//   Record rec;
//   file.GetNext(rec);
//   ComparisonEngine ce;

//   OrderMaker query;
//   cnf.BuildQuery(query, order);
//   std::cout << ce.Compare(&rec, &literal, &cnf, &order) << std::endl;
//   // std::cout << ce.Compare(&rec, &literal, &order) << std::endl;
//   // std::cout << ce.Compare(&rec, &literal, &cnf) << std::endl;
//   ASSERT_EQ(0, 1);
// }

// TEST(CreateSorted, FirstCreate) {
//   DBTest testProgram;
//   DBFile file;
//   StartupInfo startup;
//   startup.l = 3;
//   OrderMaker order;
//   // sorting on nations n_regionkey
//   order.numAtts = 1;
//   order.whichAtts[0] = 2;
//   order.whichTypes[0] = Int;
//   startup.o = &order;

//   DBTest::rel = testProgram.relations[0];

//   file.Create("test.db", sorted, &startup);

//   file.Close();

//   file.Open("test.db");
// }

// // THIS TEST ONLY SUCCEEDS WHEN SUPPLIED THE USER INPUT "(n_regionkey = 3)"
TEST(CNFQuery, SimpleQuery) {
  DBTest testProgram;
  DBFile file;
  StartupInfo startup;
  CNF cnf;
  Record literal;

  startup.l = 3;

  // sorting on nations n_regionkey
  OrderMaker order;
  order.numAtts = 1;
  order.whichAtts[0] = 2;
  order.whichTypes[0] = Int;
  startup.o = &order;

  DBTest::rel = testProgram.relations[0];

  DBTest::rel->get_cnf(cnf, literal);

  file.Create("test.db", sorted, &startup);

  char tbl_path[100];
  sprintf(tbl_path, "%s%s.tbl", DBTest::tpch_dir.c_str(), DBTest::rel->name());
  std::cout << " input from file : " << tbl_path << std::endl;

  // Preloads DBFile with table entries from nation.tbl
  FILE *tblfile = fopen(tbl_path, "r");
  Record temp;
  while (temp.SuckNextRecord(DBTest::rel->schema(), tblfile)) {
    file.Add(temp);
  }

  Record getme;
  int numRecs = 0;
  int res = file.GetNext(getme, cnf, literal);
  ASSERT_EQ(res, 1);

  while (res == 1) {
    numRecs++;
    getme.Print(DBTest::rel->schema());
    res = file.GetNext(getme, cnf, literal);
  }

  ASSERT_EQ(numRecs, 5);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
