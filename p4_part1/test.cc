
#include <cstring>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "../ParseTree.h"
#include "../statistics/Statistics.h"

extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char *);
extern "C" int yyparse(void);
extern struct AndList *final;

using namespace std;

void PrintOperand(struct Operand *pOperand) {
  if (pOperand != NULL) {
    cout << pOperand->value << " ";
  } else
    return;
}

void PrintComparisonOp(struct ComparisonOp *pCom) {
  if (pCom != NULL) {
    PrintOperand(pCom->left);
    switch (pCom->code) {
    case 1:
      cout << " < ";
      break;
    case 2:
      cout << " > ";
      break;
    case 3:
      cout << " = ";
      break;
    default:
      cout << "Error pCom code not found: " << pCom->code;
    }
    PrintOperand(pCom->right);

  } else {
    return;
  }
}
void PrintOrList(struct OrList *pOr) {
  if (pOr != NULL) {
    struct ComparisonOp *pCom = pOr->left;
    PrintComparisonOp(pCom);

    if (pOr->rightOr) {
      cout << " OR ";
      PrintOrList(pOr->rightOr);
    }
  } else {
    return;
  }
}
void PrintAndList(struct AndList *pAnd) {
  if (pAnd != NULL) {
    struct OrList *pOr = pAnd->left;
    PrintOrList(pOr);
    if (pAnd->rightAnd) {
      cout << " AND ";
      PrintAndList(pAnd->rightAnd);
    }
  } else {
    return;
  }
}

char *fileName = strdup("Statistics.txt");

void q0() {

  Statistics s;
  char *relName[] = {strdup("supplier"), strdup("partsupp")};

  s.AddRel(relName[0], 10000);
  s.AddAtt(relName[0], strdup("s_suppkey"), 10000);

  s.AddRel(relName[1], 800000);
  s.AddAtt(relName[1], strdup("ps_suppkey"), 10000);

  const char *cnf = "(s_suppkey = ps_suppkey)";

  yy_scan_string(cnf);
  yyparse();
  double result = s.Estimate(final, relName, 2);
  if (result != 800000)
    cout << "error in estimating Q1 before apply \n ";

  s.Apply(final, relName, 2);

  // test write and read
  s.Write(fileName);

  // reload the statistics object from file
  // Statistics s1;
  // s1.Read(fileName);
  cnf = "(s_suppkey>1000)"; // = roughly 266,666
  yy_scan_string(cnf);
  yyparse();
  double dummy = s.Estimate(final, relName, 2);
  if (fabs(dummy * 3.0 - result) > 0.1) {
    cout << "Read or write or last apply is not correct\n";
  }
}

void q1() {

  Statistics s;
  char *relName[] = {strdup("lineitem")};

  s.AddRel(relName[0], 6001215);
  s.AddAtt(relName[0], strdup("l_returnflag"), 3);
  s.AddAtt(relName[0], strdup("l_discount"), 11);
  s.AddAtt(relName[0], strdup("l_shipmode"), 7);

  const char *cnf =
      "(l_returnflag = 'R') AND (l_discount < 0.04 OR l_shipmode = 'MAIL')";

  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 1);
  cout << "Your estimation Result  " << result;
  cout << strdup("\n Correct Answer: 671758");

  s.Apply(final, relName, 1);

  // test write and read
  s.Write(fileName);
}

void q2() {

  Statistics s;
  char *relName[] = {strdup("orders"), strdup("customer"), strdup("nation")};

  s.AddRel(relName[0], 1500000);
  s.AddAtt(relName[0], strdup("o_custkey"), 150000);

  s.AddRel(relName[1], 150000);
  s.AddAtt(relName[1], strdup("c_custkey"), 150000);
  s.AddAtt(relName[1], strdup("c_nationkey"), 25);

  s.AddRel(relName[2], 25);
  s.AddAtt(relName[2], strdup("n_nationkey"), 25);

  const char *cnf = "(c_custkey = o_custkey)";
  yy_scan_string(cnf);
  yyparse();

  // Join the first two relations in relName
  s.Apply(final, relName, 2);

  cnf = " (c_nationkey = n_nationkey)";
  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 3);
  if (fabs(result - 1500000) > 0.1)
    cout << "error in estimating Q2\n";
  s.Apply(final, relName, 3);

  s.Write(fileName);
}

// Note there is a self join
void q3() {

  Statistics s;
  char *relName[] = {strdup("supplier"), strdup("customer"), strdup("nation")};

  s.AddRel(relName[0], 10000);
  s.AddAtt(relName[0], strdup("s_nationey"), 25);

  s.AddRel(relName[1], 150000);
  s.AddAtt(relName[1], strdup("c_custkey"), 150000);
  s.AddAtt(relName[1], strdup("c_nationkey"), 25);

  s.AddRel(relName[2], 25);
  s.AddAtt(relName[2], strdup("n_nationkey"), 25);

  s.CopyRel(strdup("nation"), strdup("n1"));
  s.CopyRel(strdup("nation"), strdup("n2"));
  s.CopyRel(strdup("supplier"), strdup("s"));
  s.CopyRel(strdup("customer"), strdup("c"));

  char *set1[] = {strdup("s"), strdup("n1")};
  const char *cnf = "(s.s_nationkey = n1.n_nationkey)";
  yy_scan_string(cnf);
  yyparse();
  s.Apply(final, set1, 2);

  char *set2[] = {strdup("c"), strdup("n2")};
  cnf = "(c.c_nationkey = n2.n_nationkey)";
  yy_scan_string(cnf);
  yyparse();
  s.Apply(final, set2, 2);

  char *set3[] = {strdup("c"), strdup("s"), strdup("n1"), strdup("n2")};
  cnf = " (n1.n_nationkey = n2.n_nationkey )";
  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, set3, 4);
  if (fabs(result - 60000000.0) > 0.1)
    cout << "error in estimating Q3\n";

  s.Apply(final, set3, 4);

  s.Write(fileName);
}

void q4() {

  Statistics s;
  char *relName[] = {strdup("part"), strdup("partsupp"), strdup("supplier"),
                     strdup("nation"), strdup("region")};

  s.AddRel(relName[0], 200000);
  s.AddAtt(relName[0], strdup("p_partkey"), 200000);
  s.AddAtt(relName[0], strdup("p_size"), 50);

  s.AddRel(relName[1], 800000);
  s.AddAtt(relName[1], strdup("ps_suppkey"), 10000);
  s.AddAtt(relName[1], strdup("ps_partkey"), 200000);

  s.AddRel(relName[2], 10000);
  s.AddAtt(relName[2], strdup("s_suppkey"), 10000);
  s.AddAtt(relName[2], strdup("s_nationkey"), 25);

  s.AddRel(relName[3], 25);
  s.AddAtt(relName[3], strdup("n_nationkey"), 25);
  s.AddAtt(relName[3], strdup("n_regionkey"), 5);

  s.AddRel(relName[4], 5);
  s.AddAtt(relName[4], strdup("r_regionkey"), 5);
  s.AddAtt(relName[4], strdup("r_name"), 5);

  s.CopyRel(strdup("part"), strdup("p"));
  s.CopyRel(strdup("partsupp"), strdup("ps"));
  s.CopyRel(strdup("supplier"), strdup("s"));
  s.CopyRel(strdup("nation"), strdup("n"));
  s.CopyRel(strdup("region"), strdup("r"));

  const char *cnf = "(p.p_partkey=ps.ps_partkey) AND (p.p_size = 2)";
  yy_scan_string(cnf);
  yyparse();
  s.Apply(final, relName, 2);

  cnf = "(s.s_suppkey = ps.ps_suppkey)";
  yy_scan_string(cnf);
  yyparse();
  s.Apply(final, relName, 3);

  cnf = " (s.s_nationkey = n.n_nationkey)";
  yy_scan_string(cnf);
  yyparse();
  s.Apply(final, relName, 4);

  cnf = "(n.n_regionkey = r.r_regionkey) AND (r.r_name = 'AMERICA') ";
  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 5);
  if (fabs(result - 3200) > 0.1)
    cout << "error in estimating Q4\n";

  s.Apply(final, relName, 5);

  s.Write(fileName);
}

void q5() {

  Statistics s;
  char *relName[] = {strdup("customer"), strdup("orders"), strdup("lineitem")};

  s.AddRel(relName[0], 150000);
  s.AddAtt(relName[0], strdup("c_custkey"), 150000);
  s.AddAtt(relName[0], strdup("c_mktsegment"), 5);

  s.AddRel(relName[1], 1500000);
  s.AddAtt(relName[1], strdup("o_orderkey"), 1500000);
  s.AddAtt(relName[1], strdup("o_custkey"), 150000);
  s.AddAtt(relName[1], strdup("o_orderdate"), -1);

  s.AddRel(relName[2], 6001215);
  s.AddAtt(relName[2], strdup("l_orderkey"), 1500000);

  const char *cnf =
      "(c_mktsegment = 'BUILDING')  AND (c_custkey = o_custkey)  AND "
      "(o_orderdate < '1995-03-1')";
  yy_scan_string(cnf);
  yyparse();
  s.Apply(final, relName, 2);

  cnf = " (l_orderkey = o_orderkey) ";
  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 3);

  if (fabs(result - 584911) > 0.1)
    cout << "error in estimating Q5\n";

  s.Apply(final, relName, 3);

  s.Write(fileName);
}

void q6() {

  Statistics s;
  char *relName[] = {strdup("partsupp"), strdup("supplier"), strdup("nation")};

  s.AddRel(relName[0], 800000);
  s.AddAtt(relName[0], strdup("ps_suppkey"), 10000);

  s.AddRel(relName[1], 10000);
  s.AddAtt(relName[1], strdup("s_suppkey"), 10000);
  s.AddAtt(relName[1], strdup("s_nationkey"), 25);

  s.AddRel(relName[2], 25);
  s.AddAtt(relName[2], strdup("n_nationkey"), 25);
  s.AddAtt(relName[2], strdup("n_name"), 25);

  const char *cnf = " (s_suppkey = ps_suppkey) ";
  yy_scan_string(cnf);
  yyparse();
  s.Apply(final, relName, 2);

  cnf = " (s_nationkey = n_nationkey)  AND (n_name = 'AMERICA')   ";
  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 3);

  if (fabs(result - 32000) > 0.1)
    cout << "error in estimating Q6\n";
  s.Apply(final, relName, 3);

  s.Write(fileName);
}

void q7() {

  Statistics s;
  char *relName[] = {strdup("orders"), strdup("lineitem")};

  s.AddRel(relName[0], 1500000);
  s.AddAtt(relName[0], strdup("o_orderkey"), 1500000);

  s.AddRel(relName[1], 6001215);
  s.AddAtt(relName[1], strdup("l_orderkey"), 1500000);
  s.AddAtt(relName[1], strdup("l_receiptdate"), 1500000);

  const char *cnf =
      "(l_receiptdate >'1995-02-01' ) AND (l_orderkey = o_orderkey)";

  yy_scan_string(cnf);
  yyparse();
  double result = s.Estimate(final, relName, 2);

  if (fabs(result - 2000405) > 0.1)
    cout << "error in estimating Q7\n";

  s.Apply(final, relName, 2);
  s.Write(fileName);
}

// Note  OR conditions are not independent.
void q8() {

  Statistics s;
  char *relName[] = {strdup("part"), strdup("partsupp")};

  s.AddRel(relName[0], 200000);
  s.AddAtt(relName[0], strdup("p_partkey"), 200000);
  s.AddAtt(relName[0], strdup("p_size"), 50);

  s.AddRel(relName[1], 800000);
  s.AddAtt(relName[1], strdup("ps_partkey"), 200000);

  const char *cnf =
      "(p_partkey=ps_partkey) AND (p_size =3 OR p_size=6 OR p_size =19)";

  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 2);

  if (fabs(result - 48000) > 0.1)
    cout << "error in estimating Q8\n";

  s.Apply(final, relName, 2);

  s.Write(fileName);
}
void q9() {

  Statistics s;
  char *relName[] = {strdup("part"), strdup("partsupp"), strdup("supplier")};

  s.AddRel(relName[0], 200000);
  s.AddAtt(relName[0], strdup("p_partkey"), 200000);
  s.AddAtt(relName[0], strdup("p_name"), 199996);

  s.AddRel(relName[1], 800000);
  s.AddAtt(relName[1], strdup("ps_partkey"), 200000);
  s.AddAtt(relName[1], strdup("ps_suppkey"), 10000);

  s.AddRel(relName[2], 10000);
  s.AddAtt(relName[2], strdup("s_suppkey"), 10000);

  const char *cnf =
      "(p_partkey=ps_partkey) AND (p_name = 'dark green antique puff wheat') ";
  yy_scan_string(cnf);
  yyparse();
  s.Apply(final, relName, 2);

  cnf = " (s_suppkey = ps_suppkey) ";
  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 3);
  if (fabs(result - 4) > 0.5)
    cout << "error in estimating Q9\n";

  s.Apply(final, relName, 3);

  s.Write(fileName);
}

void q10() {

  Statistics s;
  char *relName[] = {strdup("customer"), strdup("orders"), strdup("lineitem"),
                     strdup("nation")};

  s.AddRel(relName[0], 150000);
  s.AddAtt(relName[0], strdup("c_custkey"), 150000);
  s.AddAtt(relName[0], strdup("c_nationkey"), 25);

  s.AddRel(relName[1], 1500000);
  s.AddAtt(relName[1], strdup("o_orderkey"), 1500000);
  s.AddAtt(relName[1], strdup("o_custkey"), 150000);
  s.AddAtt(relName[1], strdup("o_orderdate"), -1);

  s.AddRel(relName[2], 6001215);
  s.AddAtt(relName[2], strdup("l_orderkey"), 1500000);

  s.AddRel(relName[3], 25);
  s.AddAtt(relName[3], strdup("n_nationkey"), 25);

  const char *cnf =
      "(c_custkey = o_custkey)  AND (o_orderdate > '1994-01-23') ";
  yy_scan_string(cnf);
  yyparse();
  s.Apply(final, relName, 2);

  cnf = " (l_orderkey = o_orderkey) ";
  yy_scan_string(cnf);
  yyparse();

  s.Apply(final, relName, 3);

  cnf = "(c_nationkey = n_nationkey) ";
  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 4);

  if (fabs(result - 4132980) > 2000)
    cout << "error in estimating Q10\n";

  s.Apply(final, relName, 4);

  s.Write(fileName);
}

void q11() {

  Statistics s;
  char *relName[] = {strdup("part"), strdup("lineitem")};

  s.AddRel(relName[0], 200000);
  s.AddAtt(relName[0], strdup("p_partkey"), 200000);
  s.AddAtt(relName[0], strdup("p_container"), 40);

  s.AddRel(relName[1], 6001215);
  s.AddAtt(relName[1], strdup("l_partkey"), 200000);
  s.AddAtt(relName[1], strdup("l_shipinstruct"), 4);
  s.AddAtt(relName[1], strdup("l_shipmode"), 7);

  const char *cnf =
      "(l_partkey = p_partkey) AND (l_shipmode = 'AIR' OR l_shipmode = "
      "'AIR REG') AND (p_container ='SM BOX' OR p_container = 'SM "
      "PACK')  AND (l_shipinstruct = 'DELIVER IN PERSON')";

  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 2);
  if (fabs(result - 19425.1) > 0.5)
    cout << "error in estimating Q11\n";
  s.Apply(final, relName, 2);

  s.Write(fileName);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "You need to supply me the query number to run as a command-line "
            "arg.."
         << endl;
    cerr << "Usage: ./test.out [0-11] >" << endl;
    exit(1);
  }

  void (*query_ptr[])() = {&q0, &q1, &q2, &q3, &q4,  &q5,
                           &q6, &q7, &q8, &q9, &q10, &q11};
  void (*query)();
  int qindx = atoi(argv[1]);

  if (qindx >= 0 && qindx < 12) {
    query = query_ptr[qindx];
    query();
    cout << "\n\n";
  } else {
    cout << " ERROR!!!!\n";
  }
}
