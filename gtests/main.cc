#include "../db_file/SortedDB.h"
#include "../test.h"
#include "gtest/gtest.h"

TEST(CreateSorted, FirstCreate) {
  DBTest testProgram;
  DBFile file;
  StartupInfo startup;
  startup.l = 3;
  OrderMaker order;
  
  DBTest::rel = testProgram.relations[0];
  
  DBTest::rel->get_sort_order(order);
  startup.o = &order;

  file.Create("test.db", sorted, &startup);
  
  file.Close();

  file.Open("test.db");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
