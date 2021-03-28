#include "../test.h"
#include <unordered_map>

using namespace std;

// load from a tpch file
void test1() {

  DBFile dbfile;
  cout << " DBFile will be created at " << DBTest::rel->path() << endl;
  dbfile.Create(DBTest::rel->path(), heap, NULL);

  char tbl_path[100]; // construct path of the tpch flat text file
  sprintf(tbl_path, "%s%s.tbl", DBTest::tpch_dir.c_str(), DBTest::rel->name());
  cout << " tpch file will be loaded from " << tbl_path << endl;

  dbfile.Load(*(DBTest::rel->schema()), tbl_path);
  dbfile.Close();
}

// sequential scan of a DBfile
void test2() {

  DBFile dbfile;
  dbfile.Open(DBTest::rel->path());
  dbfile.MoveFirst();

  Record temp;

  int counter = 0;
  while (dbfile.GetNext(temp) == 1) {
    counter += 1;
    temp.Print(DBTest::rel->schema());
    if (counter % 10000 == 0) {
      cout << counter << "\n";
    }
  }
  cout << " scanned " << counter << " recs \n";
  dbfile.Close();
}

// scan of a DBfile and apply a filter predicate
void test3() {

  cout << " Filter with CNF for : " << DBTest::rel->name() << "\n";

  CNF cnf;
  Record literal;
  DBTest::rel->get_cnf(cnf, literal);

  DBFile dbfile;
  dbfile.Open(DBTest::rel->path());
  dbfile.MoveFirst();

  Record temp;

  int counter = 0;
  while (dbfile.GetNext(temp, cnf, literal) == 1) {
    counter += 1;
    temp.Print(DBTest::rel->schema());
    if (counter % 10000 == 0) {
      cout << counter << "\n";
    }
  }
  cout << " selected " << counter << " recs \n";
  dbfile.Close();
}

int main() {
  std::unordered_map<int, std::string> choiceToRelName = {
      {1, "nation"},   {2, "region"},   {3, "customer"}, {4, "part"},
      {5, "partsupp"}, {6, "supplier"}, {7, "orders"},   {8, "lineitem"}};

  DBTest testProgram;

  void (*test)();

  void (*test_ptr[])() = {&test1, &test2, &test3};

  int tindx = 0;
  while (tindx < 1 || tindx > 3) {
    cout << " select test: \n";
    cout << " \t 1. load file \n";
    cout << " \t 2. scan \n";
    cout << " \t 3. scan & filter \n \t ";
    cin >> tindx;
  }

  int findx = 0;
  while (findx < 1 || findx > 8) {
    cout << "\n select table: \n";
    cout << "\t 1. nation \n";
    cout << "\t 2. region \n";
    cout << "\t 3. customer \n";
    cout << "\t 4. part \n";
    cout << "\t 5. partsupp \n";
    cout << "\t 6. supplier \n";
    cout << "\t 7. orders \n";
    cout << "\t 8. lineitem \n \t ";
    cin >> findx;
  }

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
}
