#include "../../relational_ops/GroupBy.h"
#include "../../relational_ops/WriteOut.h"
#include "../../test.h"
#include "gtest/gtest.h"
#include <cstdlib>

Attribute IA = {strdup("int"), Int};
Attribute SA = {strdup("string"), String};
Attribute DA = {strdup("double"), Double};

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

TEST(GroupByTest, SimpleGroupBy) {
  DBTest testProgram;
  Relation *rel = testProgram.relations.find("nation")->second;
  rel->df.Open(rel->path());

  // used to select all tuples from nation.bin (25 in total)
  char *selectAllCNF = strdup("(n_nationkey = n_nationkey)");
  rel->get_cnf(selectAllCNF);
  free(selectAllCNF);

  rel->sf.Use_n_Pages(1);
  rel->sf.Run(rel->df, rel->pipe, rel->cnf, rel->literal);
  rel->sf.WaitUntilDone();

  GroupBy groupBy;
  groupBy.Use_n_Pages(1);

  OrderMaker groupAtts;
  groupAtts.numAtts = 1;
  groupAtts.whichAtts[0] = 2;
  groupAtts.whichTypes[0] = Int;

  Pipe outPipe(100);
  Function calc;

  // we group by regionkey ([0 - 4], 5 in total)
  // we also sum by regionkey. There is 5 of each so
  // the tuples will be
  // (0, 0), (5*1, 1), (5*2, 2), (5*3, 3), (5*4, 4)
  char *str_sum = strdup("(n_regionkey)");
  rel->get_cnf(str_sum, calc);
  free(str_sum);

  groupBy.Run(rel->pipe, outPipe, groupAtts, 4, calc);
  groupBy.WaitUntilDone();

  int count = 0;
  Record buffer;

  Attribute shared = {strdup("n_regionkey"), Int};
  Attribute *attrs[]{&IA, &shared};
  Schema schema("nation.bin", 2, attrs);

  while (outPipe.Remove(&buffer)) {
    buffer.Print(&schema);
    count++;
  }

  ASSERT_EQ(count, 5);
  testProgram.cleanup();
}

TEST(GroupByTest, GroupByWithJoin) {
  DBTest testProgram;
  Relation *leftRel = testProgram.relations.find("region")->second;
  leftRel->df.Open(leftRel->path());

  // used to select all tuples from region.bin (5 in total)
  char *selectAllCNF = strdup("(r_regionkey = r_regionkey)");
  leftRel->get_cnf(selectAllCNF);
  free(selectAllCNF);

  leftRel->sf.Use_n_Pages(1);
  leftRel->sf.Run(leftRel->df, leftRel->pipe, leftRel->cnf, leftRel->literal);

  Relation *rightRel = testProgram.relations.find("nation")->second;
  rightRel->df.Open(rightRel->path());

  // used to select all tuples from nation.bin (25 in total)
  char *selectAllCNFRight = strdup("(n_nationkey = n_nationkey)");
  rightRel->get_cnf(selectAllCNFRight);
  free(selectAllCNFRight);

  rightRel->sf.Use_n_Pages(1);
  rightRel->sf.Run(rightRel->df, rightRel->pipe, rightRel->cnf,
                   rightRel->literal);

  CNF joinCNF;
  Record joinLiteral;
  char *joinPred = strdup("(r_regionkey = n_regionkey)");
  get_cnf(joinPred, leftRel->schema(), rightRel->schema(), joinCNF,
          joinLiteral);
  free(joinPred);

  Attribute nation_key = {strdup("n_nationkey"), Int};

  int outAtts = 7; // 3 from region, 4 from nation
  Attribute *joinatt[] = {&IA, &SA, &SA, &nation_key, &SA, &IA, &SA};
  Schema joinSchema("join_schema", outAtts, joinatt);

  Join simpleJoin;
  Pipe joinOut(100);
  simpleJoin.Use_n_Pages(1);

  simpleJoin.Run(leftRel->pipe, rightRel->pipe, joinOut, joinCNF, joinLiteral,
                 3, 4);

  simpleJoin.WaitUntilDone();

  OrderMaker groupAtts;
  groupAtts.numAtts = 1;
  groupAtts.whichAtts[0] = 5;
  groupAtts.whichTypes[0] = Int;

  GroupBy groupBy;
  groupBy.Use_n_Pages(1);
  Pipe groupOut(100);

  Function groupByFunc;
  char *str_sum = strdup("(n_nationkey)");
  get_cnf(str_sum, &joinSchema, groupByFunc);
  free(str_sum);

  groupBy.Run(joinOut, groupOut, groupAtts, 7, groupByFunc);

  groupBy.WaitUntilDone();
  leftRel->sf.WaitUntilDone();
  rightRel->sf.WaitUntilDone();

  Attribute *groupAttrs[] = {&IA, &IA};
  Schema groupSchema("groupSchema", 2, groupAttrs);
  Record buffer;
  int count = 0;
  while (groupOut.Remove(&buffer)) {
    buffer.Print(&groupSchema);
    count++;
  }

  ASSERT_EQ(count, 5);
}

TEST(DuplicateRemoval, SimpleDuplicateRemoval) {
  DBTest testProgram;
  Relation *rel = testProgram.relations.find("nation")->second;
  rel->df.Open(rel->path());

  // used to select all tuples from nation.bin (25 in total)
  char *selectAllCNF = strdup("(n_nationkey = n_nationkey)");
  rel->get_cnf(selectAllCNF);
  free(selectAllCNF);

  rel->sf.Use_n_Pages(1);
  // getting the records from the nation DBFile twice!
  rel->sf.Run(rel->df, rel->pipe, rel->cnf, rel->literal);
  rel->sf.WaitUntilDone();
  rel->sf.Run(rel->df, rel->pipe, rel->cnf, rel->literal);
  rel->sf.WaitUntilDone();

  int count = 0;
  Record buffer;
  Pipe tempPipe(100);
  while (rel->pipe.Remove(&buffer)) {
    count++;
    tempPipe.Insert(&buffer);
  }
  tempPipe.ShutDown();

  ASSERT_EQ(count, 50);

  DuplicateRemoval duplicateRemoval;
  duplicateRemoval.Use_n_Pages(1);

  Pipe outPipe(100);
  duplicateRemoval.Run(tempPipe, outPipe, *rel->schema());
  duplicateRemoval.WaitUntilDone();

  count = 0;
  while (outPipe.Remove(&buffer)) {
    count++;
  }

  ASSERT_EQ(count, 25);
  testProgram.cleanup();
}

TEST(Join, SimpleJoin) {
  DBTest testProgram;
  Relation *leftRel = testProgram.relations.find("region")->second;
  leftRel->df.Open(leftRel->path());

  // used to select all tuples from region.bin (5 in total)
  char *selectAllCNF = strdup("(r_regionkey = r_regionkey)");
  leftRel->get_cnf(selectAllCNF);
  free(selectAllCNF);

  leftRel->sf.Use_n_Pages(1);
  leftRel->sf.Run(leftRel->df, leftRel->pipe, leftRel->cnf, leftRel->literal);

  Relation *rightRel = testProgram.relations.find("nation")->second;
  rightRel->df.Open(rightRel->path());

  // used to select all tuples from nation.bin (25 in total)
  char *selectAllCNFRight = strdup("(n_nationkey = n_nationkey)");
  rightRel->get_cnf(selectAllCNFRight);
  free(selectAllCNFRight);

  rightRel->sf.Use_n_Pages(1);
  rightRel->sf.Run(rightRel->df, rightRel->pipe, rightRel->cnf,
                   rightRel->literal);

  CNF joinCNF;
  Record joinLiteral;
  char *joinPred = strdup("(r_regionkey = n_regionkey)");
  get_cnf(joinPred, leftRel->schema(), rightRel->schema(), joinCNF,
          joinLiteral);
  free(joinPred);

  int outAtts = 7; // 3 from region, 4 from nation
  Attribute *joinatt[] = {&IA, &SA, &SA, &IA, &SA, &IA, &SA};
  Schema joinSchema("join_schema", outAtts, joinatt);

  Join simpleJoin;
  Pipe outPipe(100);
  simpleJoin.Use_n_Pages(1);

  simpleJoin.Run(leftRel->pipe, rightRel->pipe, outPipe, joinCNF, joinLiteral,
                 3, 4);

  leftRel->sf.WaitUntilDone();
  rightRel->sf.WaitUntilDone();
  simpleJoin.WaitUntilDone();

  Record buffer;
  int count = 0;
  while (outPipe.Remove(&buffer)) {
    buffer.Print(&joinSchema);
    count++;
  }

  ASSERT_EQ(count, 25);

  testProgram.cleanup();
}

TEST(Join, JoinWithSum) {
  DBTest testProgram;
  Relation *leftRel = testProgram.relations.find("region")->second;
  leftRel->df.Open(leftRel->path());

  // used to select all tuples from region.bin (5 in total)
  char *selectAllCNF = strdup("(r_regionkey = r_regionkey)");
  leftRel->get_cnf(selectAllCNF);
  free(selectAllCNF);

  leftRel->sf.Use_n_Pages(1);
  leftRel->sf.Run(leftRel->df, leftRel->pipe, leftRel->cnf, leftRel->literal);

  Relation *rightRel = testProgram.relations.find("nation")->second;
  rightRel->df.Open(rightRel->path());

  // used to select all tuples from nation.bin (25 in total)
  char *selectAllCNFRight = strdup("(n_nationkey = n_nationkey)");
  rightRel->get_cnf(selectAllCNFRight);
  free(selectAllCNFRight);

  rightRel->sf.Use_n_Pages(1);
  rightRel->sf.Run(rightRel->df, rightRel->pipe, rightRel->cnf,
                   rightRel->literal);

  Pipe joinOutPipe(100);
  Join simpleJoin;

  CNF joinCNF;
  Record joinLiteral;
  char *joinPred = strdup("(r_regionkey = n_regionkey)");
  get_cnf(joinPred, leftRel->schema(), rightRel->schema(), joinCNF,
          joinLiteral);
  free(joinPred);

  Attribute nation_key = {strdup("n_nationkey"), Int};

  int outAtts = 7; // 3 from region, 4 from nation
  Attribute *joinatt[] = {&IA, &SA, &SA, &nation_key, &SA, &IA, &SA};
  Schema joinSchema("join_schema", outAtts, joinatt);

  simpleJoin.Use_n_Pages(1);
  simpleJoin.Run(leftRel->pipe, rightRel->pipe, joinOutPipe, joinCNF,
                 joinLiteral, 3, 4);

  Sum sumOnJoin;
  sumOnJoin.Use_n_Pages(1);

  Pipe sumOutPipe(100);
  Function sumFunc;

  char *str_sum = strdup("(n_nationkey)");
  get_cnf(str_sum, &joinSchema, sumFunc);
  free(str_sum);

  sumOnJoin.Run(joinOutPipe, sumOutPipe, sumFunc);

  simpleJoin.WaitUntilDone();
  sumOnJoin.WaitUntilDone();
  leftRel->sf.WaitUntilDone();
  rightRel->sf.WaitUntilDone();

  Record buffer;
  Attribute *sumAtts[] = {&IA};
  Schema sumSchema("sumSchema", 1, sumAtts);
  int count = 0;
  while (sumOutPipe.Remove(&buffer)) {
    count++;
    buffer.Print(&sumSchema);
  }
  ASSERT_EQ(count, 1);
  testProgram.cleanup();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
