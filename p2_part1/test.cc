#include "../test.h"

void *producer(void *arg) {

  Pipe *myPipe = (Pipe *)arg;

  Record temp;
  int counter = 0;

  DBFile dbfile;
  if (!dbfile.Open(Test::rel->path())) {
    std::cout << "FAILED TO OPEN DATABASE" << std::endl;
  }
  std::cout << " producer: opened DBFile " << Test::rel->path() << std::endl;
  dbfile.MoveFirst();

  while (dbfile.GetNext(temp) == 1) {
    counter += 1;
    if (counter % 100000 == 0) {
      std::cerr << " producer: " << counter << std::endl;
    }
    myPipe->Insert(&temp);
  }

  dbfile.Close();
  myPipe->ShutDown();

  std::cout << " producer: inserted " << counter << " recs into the pipe\n";
  return NULL;
}

void *consumer(void *arg) {

  testutil *t = (testutil *)arg;

  ComparisonEngine ceng;

  DBFile dbfile;
  char outfile[100];

  if (t->write) {
    sprintf(outfile, "%s.bigq", Test::rel->path());
    dbfile.Create(outfile, heap, NULL);
  }

  int err = 0;
  int i = 0;

  Record rec[2];
  Record *last = NULL, *prev = NULL;

  while (t->pipe->Remove(&rec[i % 2])) {
    prev = last;
    last = &rec[i % 2];

    if (prev && last) {

      if (ceng.Compare(prev, last, t->order) == 1) {
        std::cout << "Index of error: " << i << std::endl;

        std::cout << "PREV" << std::endl;
        prev->Print(Test::rel->schema());
        std::cout << "LAST" << std::endl;
        last->Print(Test::rel->schema());
        std::cout << "\n\n\n";
        err++;
      }
      if (t->write) {
        dbfile.Add(*prev);
      }
    }
    if (t->print) {
      last->Print(Test::rel->schema());
    }
    i++;
  }

  std::cout << " consumer: removed " << i << " recs from the pipe\n";

  if (t->write) {
    if (last) {
      dbfile.Add(*last);
    }
    std::cerr << " consumer: recs removed written out as heap DBFile at " << outfile
         << std::endl;
    dbfile.Close();
  }
  std::cerr << " consumer: " << (i - err) << " recs out of " << i
       << " recs in sorted order \n";
  if (err) {
    std::cerr << " consumer: " << err << " recs failed sorted order test \n" << std::endl;
  }
  return NULL;
}

void test1(int option, int runlen) {

  // sort order for records
  OrderMaker sortorder;
  Test::rel->get_sort_order(sortorder);

  int buffsz = 100; // pipe cache size
  Pipe input(buffsz);
  Pipe output(buffsz);

  // thread to dump data into the input pipe (for BigQ's consumption)
  pthread_t thread1;
  pthread_create(&thread1, NULL, producer, (void *)&input);

  // thread to read sorted data from output pipe (dumped by BigQ)
  pthread_t thread2;
  testutil tutil = {&output, &sortorder, false, false};
  if (option == 2) {
    tutil.print = true;
  } else if (option == 3) {
    tutil.write = true;
  }
  pthread_create(&thread2, NULL, consumer, (void *)&tutil);

  BigQ bq(input, output, sortorder, runlen);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
}

int main(int argc, char *argv[]) {
  Test testProgram;

  int tindx = 0;
  while (tindx < 1 || tindx > 3) {
    std::cout << " select test option: \n";
    std::cout << " \t 1. sort \n";
    std::cout << " \t 2. sort + display \n";
    std::cout << " \t 3. sort + write \n\t ";
    std::cin >> tindx;
  }

  int findx = 0;
  while (findx < 1 || findx > 7) {
    std::cout << "\n select dbfile to use: \n";
    std::cout << "\t 1. nation \n";
    std::cout << "\t 2. region \n";
    std::cout << "\t 3. customer \n";
    std::cout << "\t 4. part \n";
    std::cout << "\t 5. partsupp \n";
    std::cout << "\t 6. orders \n";
    std::cout << "\t 7. lineitem \n \t ";
    std::cin >> findx;
  }
  Test::rel = testProgram.relations[findx - 1];

  int runlen;
  std::cout << "\t\n specify runlength:\n\t ";
  std::cin >> runlen;

  test1(tindx, runlen);

  testProgram.cleanup();
}
