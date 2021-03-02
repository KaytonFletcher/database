#include "../test.h"

void test1();
void test2();
void test3();

int add_data(FILE *src, int numrecs, int &res) {
  DBFile dbfile;
  dbfile.Open(DBTest::rel->path());
  Record temp;

  int proc = 0;
  int xx = 20000;
  res = temp.SuckNextRecord(DBTest::rel->schema(), src);

  while (res && proc < numrecs) {
    proc++;
    dbfile.Add(temp);
    if (proc == xx)
      std::cerr << "\t ";
    if (proc % xx == 0)
      std::cerr << ".";

    res = temp.SuckNextRecord(DBTest::rel->schema(), src);
  }

  dbfile.Close();
  return proc;
}

// create a dbfile interactively
void test1() {

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

  srand48(time(NULL));

  int proc = 1, res = 1, tot = 0;
  while (proc && res) {
    int x = 0;
    while (x < 1 || x > 3) {
      std::cout << "\n select option for : " << DBTest::rel->path()
                << std::endl;
      std::cout << " \t 1. add a few (1 to 1k recs)\n";
      std::cout << " \t 2. add a lot (1k to 1e+06 recs) \n";
      std::cout << " \t 3. run some query \n \t ";
      std::cin >> x;
    }
    if (x < 3) {
      proc =
          add_data(tblfile, lrand48() % (int)pow(1e3, x) + (x - 1) * 1000, res);
      tot += proc;
      if (proc)
        std::cout << "\n\t added " << proc << " recs..so far " << tot
                  << std::endl;
    } else {
      test3();
    }
  }
  std::cout << "\n create finished.. " << tot << " recs inserted\n";
  fclose(tblfile);
}

// sequential scan of a DBfile
void test2() {

  std::cout << " scan : " << DBTest::rel->path() << "\n";
  DBFile dbfile;
  dbfile.Open(DBTest::rel->path());
  dbfile.MoveFirst();

  Record temp;

  int cnt = 0;
  std::cerr << "\t";
  while (dbfile.GetNext(temp) && ++cnt) {
    temp.Print(DBTest::rel->schema());
    if (cnt % 10000) {
      std::cerr << ".";
    }
  }
  std::cout << "\n scanned " << cnt << " recs \n";
  dbfile.Close();
}

void test3() {

  CNF cnf;
  Record literal;
  DBTest::rel->get_cnf(cnf, literal);

  DBFile dbfile;
  dbfile.Open(DBTest::rel->path());
  dbfile.MoveFirst();

  Record temp;

  int cnt = 0;
  std::cerr << "\t";
  while (dbfile.GetNext(temp, cnf, literal) && ++cnt) {
    temp.Print(DBTest::rel->schema());
    if (cnt % 10000 == 0) {
      std::cerr << ".";
    }
  }
  std::cout << "\n query over " << DBTest::rel->path() << " returned " << cnt
            << " recs\n";
  dbfile.Close();
}

int main(int argc, char *argv[]) {

  DBTest testProgram;

  void (*test_ptr[])() = {&test1, &test2, &test3};
  void (*test)();

  int tindx = 0;
  while (tindx < 1 || tindx > 3) {
    std::cout << " select test option: \n";
    std::cout << " \t 1. create sorted dbfile\n";
    std::cout << " \t 2. scan a dbfile\n";
    std::cout << " \t 3. run some query \n \t ";
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
  DBTest::rel = testProgram.relations[findx - 1];

  test = test_ptr[tindx - 1];
  test();

  testProgram.cleanup();
  std::cout << "\n\n";
}
