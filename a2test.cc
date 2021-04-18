#include "test.h"

int add_data(FILE *src) {
  DBFile dbfile;
  dbfile.Open(DBTest::rel->path());
  Record temp;

  int recsAdded = 0;
  while (temp.SuckNextRecord(DBTest::rel->schema(), src)) {
    dbfile.Add(temp);
    recsAdded++;
  }

  dbfile.Close();
  return recsAdded;
}

// create a sorted dbfile interactively
void createSortedDB() {

  OrderMaker o;
  DBTest::rel->get_sort_order(o);

  std::cin.clear();
  clearerr(stdin);

  int runlen = 0;
  while (runlen < 1) {
    std::cout << "\t\n specify runlength:\n\t ";
    std::cin >> runlen;
  }
  struct {
    OrderMaker *o;
    int l;
  } startup = {&o, runlen};

  DBFile dbfile;
  std::cout << "\n output to dbfile : " << DBTest::rel->path() << std::endl;
  dbfile.Create(DBTest::rel->path(), sorted, &startup);
  dbfile.Close();

  char tbl_path[100];
  sprintf(tbl_path, "%s%s.tbl", DBTest::tpch_dir.c_str(), DBTest::rel->name());
  std::cout << " input from file : " << tbl_path << std::endl;

  FILE *tblfile = fopen(tbl_path, "r");

  int total = add_data(tblfile);

  std::cout << "\n create finished.. " << total << " recs inserted\n";
  fclose(tblfile);
}

// created unsorted heap based database
void createHeapDB() {
  DBFile dbfile;
  std::cout << " DBFile will be created at " << DBTest::rel->path()
            << std::endl;
  dbfile.Create(DBTest::rel->path(), heap, NULL);

  char tbl_path[100]; // construct path of the tpch flat text file
  sprintf(tbl_path, "%s%s.tbl", DBTest::tpch_dir.c_str(), DBTest::rel->name());
  std::cout << " tpch file will be loaded from " << tbl_path << std::endl;

  dbfile.Load(*(DBTest::rel->schema()), tbl_path);
  dbfile.Close();
}

int main(int argc, char *argv[]) {

  std::unordered_map<int, std::string> choiceToRelName = {
      {1, "nation"},   {2, "region"},   {3, "customer"}, {4, "part"},
      {5, "partsupp"}, {6, "supplier"}, {7, "orders"},   {8, "lineitem"}};

  DBTest testProgram;

  void (*test_ptr[])() = {&createSortedDB, &createHeapDB};
  void (*test)();

  int tindx = 0;
  while (tindx < 1 || tindx > 2) {
    std::cout << " select test option: \n";
    std::cout << " \t 1. create sorted dbfile\n";
    std::cout << " \t 2. create heap dbfile\n";
    std::cin >> tindx;
  }

  int findx = 0;
  while (findx < 1 || findx > 8) {
    std::cout << "\n select table: \n";
    std::cout << "\t 1. nation \n";
    std::cout << "\t 2. region \n";
    std::cout << "\t 3. customer \n";
    std::cout << "\t 4. part \n";
    std::cout << "\t 5. partsupp \n";
    std::cout << "\t 6. supplier \n";
    std::cout << "\t 7. orders \n";
    std::cout << "\t 8. lineitem \n \t ";
    std::cin >> findx;
  }
  std::string relName;

  auto itr = testProgram.relations.find(choiceToRelName[findx]);
  if (itr != testProgram.relations.end()) {
    DBTest::rel = itr->second;
  } else {
    std::cout << "Failed to find relation with name: " << choiceToRelName[findx]
              << std::endl;
    exit(1);
  }

  test = test_ptr[tindx - 1];
  test();

  testProgram.cleanup();
  std::cout << "\n\n";
}
