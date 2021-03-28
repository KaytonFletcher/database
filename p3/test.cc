#include "../test.h"

void get_cnf(char *input, Schema *left, CNF &cnf_pred, Record &literal) {
  init_lexical_parser(input);
  if (yyparse() != 0) {
    std::cout << " Error: can't parse your CNF " << input << std::endl;
    exit(1);
  }
  cnf_pred.GrowFromParseTree(final, left, literal); // constructs CNF predicate
  close_lexical_parser();
}

void get_cnf(char *input, Schema *left, Schema *right, CNF &cnf_pred,
             Record &literal) {
  init_lexical_parser(input);
  if (yyparse() != 0) {
    std::cout << " Error: can't parse your CNF " << input << std::endl;
    exit(1);
  }
  cnf_pred.GrowFromParseTree(final, left, right,
                             literal); // constructs CNF predicate
  close_lexical_parser();
}

void get_cnf(char *input, Schema *left, Function &fn_pred) {
  init_lexical_parser_func(input);
  if (yyfuncparse() != 0) {
    std::cout << " Error: can't parse your arithmetic expr. " << input
              << std::endl;
    exit(1);
  }
  fn_pred.GrowFromParseTree(finalfunc, *left); // constructs CNF predicate
  close_lexical_parser_func();
}

Attribute IA = {strdup("int"), Int};
Attribute SA = {strdup("string"), String};
Attribute DA = {strdup("double"), Double};

int clear_pipe(Pipe &in_pipe, Schema *schema, bool print) {
  Record rec;
  int cnt = 0;
  while (in_pipe.Remove(&rec)) {
    if (print) {
      rec.Print(schema);
    }
    cnt++;
  }
  return cnt;
}

int clear_pipe(Pipe &in_pipe, Schema *schema, Function &func, bool print) {
  Record rec;
  int cnt = 0;
  double sum = 0;
  while (in_pipe.Remove(&rec)) {
    if (print) {
      rec.Print(schema);
    }
    int ival = 0;
    double dval = 0;
    func.Apply(rec, ival, dval);
    sum += (ival + dval);
    cnt++;
  }
  std::cout << " Sum: " << sum << std::endl;
  return cnt;
}
int pipesz = 100; // buffer sz allowed for each pipe
int buffsz = 100; // pages of memory allowed for operations

int pAtts = 9;
int psAtts = 5;
int liAtts = 16;
int oAtts = 9;
int sAtts = 7;
int cAtts = 8;
int nAtts = 4;
int rAtts = 3;

void initSortFile(char *predicateString, int numPages, DBTest &test,
                  const char *relationName) {
  auto relItr = test.relations.find(relationName);
  if (relItr == test.relations.end()) {
    std::cout << "Failed to find relation with name: " << relationName
              << std::endl;
    exit(1);
  }

  Relation *rel = relItr->second;
  rel->df.Open(rel->path());
  get_cnf(predicateString, rel->schema(), rel->cnf, rel->literal);
  rel->sf.Use_n_Pages(numPages);
  free(predicateString);
}

// select * from partsupp where ps_supplycost <1.03
// expected output: 21 records
void q1() {
  DBTest test;
  const char *partsuppName = "partsupp";

  char *pred_ps = strdup("(ps_supplycost < 1.03)");
  initSortFile(pred_ps, 100, test, partsuppName);

  auto relItr = test.relations.find(partsuppName);
  if (relItr == test.relations.end()) {
    std::cout << "Failed to find relation with name: " << partsuppName
              << std::endl;
    exit(1);
  }

  Relation *rel = relItr->second;

  rel->sf.Run(rel->df, rel->pipe, rel->cnf, rel->literal);
  rel->sf.WaitUntilDone();

  int cnt = clear_pipe(rel->pipe, rel->schema(), true);
  std::cout << "\n\n query1 returned " << cnt << " records \n";

  rel->df.Close();
  test.cleanup();
}

// select p_partkey(0), p_name(1), p_retailprice(7) from part where
// (p_retailprice > 931.01) AND (p_retailprice < 931.3); expected output: 12
// records
void q2() {
  DBTest test;
  const char *partName = "part";

  char *pred_p = strdup("(p_retailprice > 931.01) AND (p_retailprice < 931.3)");
  initSortFile(pred_p, 100, test, "part");

  Project P_p;
  Pipe _out(pipesz);
  int keepMe[] = {0, 1, 7};
  int numAttsIn = pAtts;
  int numAttsOut = 3;
  P_p.Use_n_Pages(buffsz);

  auto relItr = test.relations.find(partName);
  if (relItr == test.relations.end()) {
    std::cout << "Failed to find relation with name: " << partName << std::endl;
    exit(1);
  }

  Relation *rel = relItr->second;

  rel->sf.Run(rel->df, rel->pipe, rel->cnf, rel->literal);

  P_p.Run(rel->pipe, _out, keepMe, numAttsIn, numAttsOut);

  rel->sf.WaitUntilDone();
  P_p.WaitUntilDone();

  Attribute *att3[] = {&IA, &SA, &DA};
  Schema out_sch("out_sch", numAttsOut, att3);
  int cnt = clear_pipe(_out, &out_sch, true);

  std::cout << "\n\n query2 returned " << cnt << " records \n";

  rel->df.Close();
  test.cleanup();
}

// select sum (s_acctbal + (s_acctbal * 1.05)) from supplier;
// expected output: 9.24623e+07
void q3() {
  DBTest test;
  char *pred_s = strdup("(s_suppkey = s_suppkey)");

  const char *supplierName = "supplier";
  initSortFile(pred_s, 100, test, supplierName);

  Sum T;
  // _s (input pipe)
  Pipe _out(1);
  Function func;
  char *str_sum = strdup("(s_acctbal + (s_acctbal * 1.05))");
  get_cnf(str_sum, test.relations[supplierName]->schema(), func);
  free(str_sum);
  func.Print();
  T.Use_n_Pages(1);

  auto relItr = test.relations.find(supplierName);
  if (relItr == test.relations.end()) {
    std::cout << "Failed to find relation with name: " << supplierName
              << std::endl;
    exit(1);
  }

  Relation *rel = relItr->second;

  rel->sf.Run(rel->df, rel->pipe, rel->cnf, rel->literal);

  T.Run(rel->pipe, _out, func);

  rel->sf.WaitUntilDone();
  T.WaitUntilDone();

  Attribute *attList[] = {&DA};
  Schema out_sch("out_sch", 1, attList);
  int cnt = clear_pipe(_out, &out_sch, true);

  std::cout << "\n\n query3 returned " << cnt << " records \n";

  rel->df.Close();
  test.cleanup();
}

// select sum (ps_supplycost) from supplier, partsupp
// where s_suppkey = ps_suppkey;
// expected output: 4.00406e+08
void q4() {
  DBTest test;
  std::cout << " query4 \n";

  const char *supplierName = "supplier";
  const char *partsuppName = "partsupp";

  char *pred_s = strdup("(s_suppkey = s_suppkey)");
  initSortFile(pred_s, 100, test, supplierName);

  auto relItr = test.relations.find(supplierName);
  if (relItr == test.relations.end()) {
    std::cout << "Failed to find relation with name: " << supplierName
              << std::endl;
    exit(1);
  }

  Relation *sRel = relItr->second;

  sRel->sf.Run(sRel->df, sRel->pipe, sRel->cnf,
               sRel->literal); // 10k recs qualified

  char *pred_ps = strdup("(ps_suppkey = ps_suppkey)");
  initSortFile(pred_ps, 100, test, partsuppName);

  auto relItr2 = test.relations.find(partsuppName);
  if (relItr2 == test.relations.end()) {
    std::cout << "Failed to find relation with name: " << partsuppName
              << std::endl;
    exit(1);
  }

  Relation *psRel = relItr2->second;

  psRel->sf.Run(psRel->df, psRel->pipe, psRel->cnf,
                psRel->literal); // 161 recs qualified

  Join J;
  // left _s
  // right _ps

  CNF joinCNF;
  Record joinLiteral;
  Pipe joinOutPipe(pipesz);

  get_cnf(strdup("(s_suppkey = ps_suppkey)"), sRel->schema(), psRel->schema(),
          joinCNF, joinLiteral);

  int outAtts = sAtts + psAtts;
  Attribute ps_supplycost = {strdup("ps_supplycost"), Double};
  Attribute *joinatt[] = {
      &IA, &SA, &SA, &IA, &SA, &DA, &SA, &IA, &IA, &IA, &ps_supplycost, &SA};
  Schema join_sch("join_sch", outAtts, joinatt);
  J.Run(sRel->pipe, psRel->pipe, joinOutPipe, joinCNF, joinLiteral, sAtts,
        psAtts);

  Sum T;
  // _s (input pipe)
  Pipe _out(1);
  Function func;
  char *str_sum = strdup("(ps_supplycost)");
  get_cnf(str_sum, &join_sch, func);

  func.Print();
  T.Use_n_Pages(1);

  T.Run(joinOutPipe, _out, func);

  J.WaitUntilDone();
  T.WaitUntilDone();

  psRel->sf.WaitUntilDone();
  sRel->sf.WaitUntilDone();
  Attribute *attList[] = {&DA};
  Schema sum_sch("sum_sch", 1, attList);
  int cnt = clear_pipe(_out, &sum_sch, true);
  std::cout << " query4 returned " << cnt << " recs \n";
  test.cleanup();
}

// select distinct ps_suppkey from partsupp where ps_supplycost < 100.11;
// expected output: 9996 rows
void q5() {
  DBTest test;
  const char *partsuppName = "partsupp";

  char *pred_ps = strdup("(ps_supplycost < 100.11)");
  initSortFile(pred_ps, 100, test, partsuppName);

  Project P_ps;
  Pipe __ps(pipesz);
  int keepMe[] = {1};
  int numAttsIn = psAtts;
  int numAttsOut = 1;
  P_ps.Use_n_Pages(buffsz);

  DuplicateRemoval D;
  // inpipe = __ps
  Pipe ___ps(pipesz);

  Attribute *attList[] = {&IA};
  Schema __ps_sch("__ps", 1, attList);

  WriteOut W;
  // inpipe = ___ps
  const char *fwpath = "ps.w.tmp";
  FILE *writefile = fopen(fwpath, "w");

  auto relItr = test.relations.find(partsuppName);
  if (relItr == test.relations.end()) {
    std::cout << "Failed to find relation with name: " << partsuppName
              << std::endl;
    exit(1);
  }

  Relation *rel = relItr->second;

  rel->sf.Run(rel->df, rel->pipe, rel->cnf, rel->literal);

  P_ps.Run(rel->pipe, __ps, keepMe, numAttsIn, numAttsOut);
  D.Run(__ps, ___ps, __ps_sch);
  W.Run(___ps, writefile, __ps_sch);

  rel->sf.WaitUntilDone();
  P_ps.WaitUntilDone();
  D.WaitUntilDone();
  W.WaitUntilDone();

  std::cout << " query5 finished..output written to file " << fwpath << "\n";
  test.cleanup();
}

// select sum (ps_supplycost) from supplier, partsupp
// where s_suppkey = ps_suppkey groupby s_nationkey;
// expected output: 25 rows
void q6() {
  DBTest test;
  std::cout << " query6 \n";

  const char *supplierName = "supplier";
  const char *partsuppName = "partsupp";

  char *pred_s = strdup("(s_suppkey = s_suppkey)");
  initSortFile(pred_s, 100, test, supplierName);

  auto relItr = test.relations.find(supplierName);
  if (relItr == test.relations.end()) {
    std::cout << "Failed to find relation with name: " << supplierName
              << std::endl;
    exit(1);
  }

  Relation *sRel = relItr->second;

  sRel->sf.Run(sRel->df, sRel->pipe, sRel->cnf,
               sRel->literal); // 10k recs qualified

  char *pred_ps = strdup("(ps_suppkey = ps_suppkey)");
  initSortFile(pred_ps, 100, test, partsuppName);

  auto relItr2 = test.relations.find(partsuppName);
  if (relItr2 == test.relations.end()) {
    std::cout << "Failed to find relation with name: " << partsuppName
              << std::endl;
    exit(1);
  }

  Relation *psRel = relItr2->second;

  psRel->sf.Run(psRel->df, psRel->pipe, psRel->cnf,
                psRel->literal); // 161 recs qualified

  Join J;
  // left _s
  // right _ps
  Pipe _s_ps(pipesz);
  CNF cnf_p_ps;
  Record lit_p_ps;
  get_cnf(strdup("(s_suppkey = ps_suppkey)"), sRel->schema(), psRel->schema(),
          cnf_p_ps, lit_p_ps);

  int outAtts = sAtts + psAtts;
  Attribute s_nationkey = {strdup("s_nationkey"), Int};
  Attribute ps_supplycost = {strdup("ps_supplycost"), Double};
  Attribute *joinatt[] = {&IA, &SA, &SA, &s_nationkey,   &SA, &DA, &SA,
                          &IA, &IA, &IA, &ps_supplycost, &SA};
  Schema join_sch("join_sch", outAtts, joinatt);

  GroupBy G;

  // _s (input pipe)
  Pipe _out(1);
  Function func;
  char *str_sum = strdup("(ps_supplycost)");
  get_cnf(str_sum, &join_sch, func);
  func.Print();

  OrderMaker grp_order;

  grp_order.numAtts = 1;
  grp_order.whichAtts[0] = 3;
  grp_order.whichTypes[0] = Int;

  G.Use_n_Pages(1);

  J.Run(sRel->pipe, psRel->pipe, _s_ps, cnf_p_ps, lit_p_ps, sAtts, psAtts);

  G.Run(_s_ps, _out, grp_order, outAtts, func);

  Attribute *attList[] = {&DA};
  Schema sum_sch("sum_sch", 1, attList);
  int cnt = clear_pipe(_out, &sum_sch, true);
  std::cout << " query6 returned sum for " << cnt
            << " groups (expected 25 groups)\n";

  G.WaitUntilDone();
  J.WaitUntilDone();

  sRel->sf.WaitUntilDone();
  psRel->sf.WaitUntilDone();

  test.cleanup();
}

void q7() {
  /*
  select sum(ps_supplycost)
  from part, supplier, partsupp
  where p_partkey = ps_partkey and
  s_suppkey = ps_suppkey and
  s_acctbal > 2500;

  ANSWER: 274251601.96 (5.91 sec)

  possible plan:
          SF(s_acctbal > 2500) => _s
          SF(p_partkey = p_partkey) => _p
          SF(ps_partkey = ps_partkey) => _ps
          On records from pipes _p and _ps:
                  J(p_partkey = ps_partkey) => _p_ps
          On _s and _p_ps:
                  J(s_suppkey = ps_suppkey) => _s_p_ps
          On _s_p_ps:
                  S(s_supplycost) => __s_p_ps
          On __s_p_ps:
                  W(__s_p_ps)

  Legend:
  SF : select all records that satisfy some simple cnf expr over recs from
  in_file SP: same as SF but recs come from in_pipe J: select all records (from
  left_pipe x right_pipe) that satisfy a cnf expression P: project some atts
  from in-pipe T: apply some aggregate function G: same as T but do it over each
  group identified by ordermaker D: stuff only distinct records into the
  out_pipe discarding duplicates W: write out records from in_pipe to a file
  using out_schema
  */
  std::cout << " TBA\n";
}

void q8() {
  /*
  select l_orderkey, l_partkey, l_suppkey
  from lineitem
  where l_returnflag = 'R' and l_discount < 0.04 or
  l_returnflag = 'R' and l_shipmode = 'MAIL';

  ANSWER: 671392 rows in set (29.45 sec)


  possible plan:
          SF (l_returnflag = 'R' and ...) => _l
          On _l:
                  P (l_orderkey,l_partkey,l_suppkey) => __l
          On __l:
                  W (__l)
  */
  std::cout << " TBA\n";
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << " Usage: ./test.out [1-8] \n";
    exit(0);
  }

  void (*query_ptr[])() = {&q1, &q2, &q3, &q4, &q5, &q6, &q7, &q8};
  void (*query)();
  int qindx = atoi(argv[1]);

  if (qindx > 0 && qindx < 9) {
    query = query_ptr[qindx - 1];
    query();
    std::cout << "\n\n";
  } else {
    std::cout << " ERROR!!!!\n";
  }
}
