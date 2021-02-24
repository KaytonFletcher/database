#include "../BigQ.h"
#include "../test.h"
#include "gtest/gtest.h"

TEST(BigQConstructor, FirstCreate) {
  setup();
  rel = n;
  int buffsz = 100; // pipe cache size
  Pipe input(buffsz);
  Pipe output(buffsz);

  Record temp;
  int counter = 0;

  DBFile dbfile;
  if (!dbfile.Open("./heap_dbs/nation.bin")) {
    std::cout << "FAILED TO OPEN DATABASE" << std::endl;
  }

  dbfile.MoveFirst();

  while (dbfile.GetNext(temp) == 1) {
    input.Insert(&temp);
  }

  dbfile.Close();
  input.ShutDown();

  OrderMaker sortorder;
  rel->get_sort_order(sortorder);

  BigQ bq(input, output, sortorder, 1);

  int i = 0;

  Record rec[2];
  Record *last = NULL, *prev = NULL;

  ComparisonEngine ce;

  while (output.Remove(&rec[i % 2])) {
    prev = last;
    last = &rec[i % 2];

    if (prev && last) {
      int comp = ce.Compare(prev, last, &sortorder);
      ASSERT_EQ(comp, 0);
    }

    i++;
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
