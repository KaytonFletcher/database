#include "../statistics/Statistics.h"

#include "gtest/gtest.h"
#include <cstdlib>

extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char *);
extern "C" int yyparse(void);
extern struct AndList *final;

TEST(StatisticsDeathTest, NoRelations) {
  Statistics stats;
  ASSERT_DEATH(stats.Estimate(nullptr, nullptr, 0),
               "Number of relations to join must be greater than 0");
}

TEST(StatisticsDeathTest, SingleRelationDeath) {
  Statistics stats;

  char *nationName = strdup("nation");
  stats.AddRel(nationName, 10);

  char *badRelName[1] = {strdup("not_test_rel")};
  ASSERT_DEATH(
      stats.Estimate(nullptr, badRelName, 1),
      "Relation specified in list to join does not exist: can't estimate");

  free(nationName);
  free(badRelName[0]);
}

TEST(StatisticsTest, CrossProduct) {
  Statistics stats;
  char *firstRelName = strdup("nation");
  char *secondRelName = strdup("region");
  char *relNames[2] = {firstRelName, secondRelName};
  stats.AddRel(firstRelName, 42);
  stats.AddRel(secondRelName, 500);

  ASSERT_EQ(21000, stats.Estimate(nullptr, relNames, 2));

  free(firstRelName);
  free(secondRelName);
}

TEST(StatisticsTest, ReadAndWrite) {
  Statistics s;
  char *relName[] = {strdup("nation"), strdup("region")};

  char *foreignKey = strdup("n_regionkey");
  char *key = strdup("r_regionkey");
  char *extraAttr = strdup("n_name");
  s.AddRel(relName[0], 10);
  s.AddAtt(relName[0], foreignKey, 10);
  s.AddAtt(relName[0], extraAttr, 10);

  s.AddRel(relName[1], 800);
  s.AddAtt(relName[1], key, 10);

  ASSERT_EQ(s.NumUniqueAttributes(), 3);
  ASSERT_EQ(s.NumRelations(), 2);
  ASSERT_EQ(s.NumRelationsWithAttribute(key), 1);

  char *fileName = strdup("mmasdnasdanfa");

  s.Write(fileName);

  Statistics newStats;
  newStats.Read(fileName);

  ASSERT_EQ(newStats.NumUniqueAttributes(), 3);
  ASSERT_EQ(newStats.NumRelations(), 2);
  ASSERT_EQ(newStats.NumRelationsWithAttribute(foreignKey), 1);

  remove(fileName);

  free(relName[0]);
  free(relName[1]);
  free(fileName);
  free(key);
  free(foreignKey);
}

TEST(StatisticsTest, ReadWriteAfterApply) {
  Statistics s;
  char *relName[] = {strdup("supplier"), strdup("partsupp")};

  char *supKey = strdup("s_suppkey");
  char *supForeignKey = strdup("ps_suppkey");
  s.AddRel(relName[0], 10000);
  s.AddAtt(relName[0], supKey, 10000);

  s.AddRel(relName[1], 800000);
  s.AddAtt(relName[1], supForeignKey, 10000);

  const char *cnf = "(s_suppkey = ps_suppkey)";

  yy_scan_string(cnf);
  yyparse();

  ASSERT_EQ(800000, s.Estimate(final, relName, 2));

  s.Apply(final, relName, 2);

  char *fileName = strdup("stupidtemp");

  s.Write(fileName);
  Statistics s2;
  s2.Read(fileName);

  ASSERT_EQ(800000, s2.Estimate(final, relName, 2));

  remove(fileName);
  free(relName[0]);
  free(relName[1]);
  free(fileName);
  free(supKey);
  free(supForeignKey);
}

TEST(StatisticsTest, SimpleEquiJoin) {
  Statistics s;
  char *relName[] = {strdup("supplier"), strdup("partsupp")};

  char *supKey = strdup("s_suppkey");
  char *supForeignKey = strdup("ps_suppkey");
  s.AddRel(relName[0], 10000);
  s.AddAtt(relName[0], supKey, 10000);

  s.AddRel(relName[1], 800000);
  s.AddAtt(relName[1], supForeignKey, 10000);

  const char *cnf = "(s_suppkey = ps_suppkey)";

  yy_scan_string(cnf);
  yyparse();

  ASSERT_EQ(800000, s.Estimate(final, relName, 2));

  free(relName[0]);
  free(relName[1]);

  free(supKey);
  free(supForeignKey);
}

TEST(StatisticsTest, DoubleEquiJoin) {
  Statistics s;
  char *relName[] = {strdup("orders"), strdup("customer"), strdup("nation")};
  char *oKey = strdup("o_custkey");
  char *cKey = strdup("c_custkey");
  char *cnKey = strdup("c_nationkey");
  char *nKey = strdup("n_nationkey");

  s.AddRel(relName[0], 1500000);
  s.AddAtt(relName[0], oKey, 150000);

  s.AddRel(relName[1], 150000);
  s.AddAtt(relName[1], cKey, 150000);
  s.AddAtt(relName[1], cnKey, 25);

  s.AddRel(relName[2], 25);
  s.AddAtt(relName[2], nKey, 25);

  const char *cnf = "(c_custkey = o_custkey)";
  yy_scan_string(cnf);
  yyparse();

  ASSERT_EQ(1500000, s.Estimate(final, relName, 2));

  // Join the first two relations in relName
  s.Apply(final, relName, 2);

  cnf = " (c_nationkey = n_nationkey)";
  yy_scan_string(cnf);
  yyparse();

  double result = s.Estimate(final, relName, 3);
  ASSERT_EQ(result, 1500000);

  free(relName[0]);
  free(relName[1]);
  free(relName[2]);

  free(oKey);
  free(cKey);
  free(cnKey);
  free(nKey);
}

TEST(StatisticsTest, EquiJoinWithSelect) {
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
  ASSERT_NEAR(result, 3374000, 5000);
}

TEST(StatisticsTest, NameResolve) {
  Statistics s;
  char *relName[] = {strdup("supplier"), strdup("partsupp")};

  char *supKey = strdup("s_suppkey");
  char *supForeignKey = strdup("ps_suppkey");
  s.AddRel(relName[0], 10);
  s.AddAtt(relName[0], supKey, 10);

  s.AddRel(relName[1], 800);
  s.AddAtt(relName[1], supForeignKey, 10);

  const char *cnf = "(supplier.s_suppkey = partsupp.ps_suppkey)";

  yy_scan_string(cnf);
  yyparse();

  ASSERT_EQ(800, s.Estimate(final, relName, 2));

  free(relName[0]);
  free(relName[1]);

  free(supKey);
  free(supForeignKey);
}

TEST(StatisticsTest, NameResolveWithMultiple) {
  Statistics s;
  char *relName[] = {strdup("supplier"), strdup("partsupp"), strdup("copy")};

  char *supKey = strdup("s_suppkey");
  char *supForeignKey = strdup("ps_suppkey");
  s.AddRel(relName[0], 10);
  s.AddAtt(relName[0], supKey, 10);

  s.AddRel(relName[1], 800);
  s.AddAtt(relName[1], supForeignKey, 10);

  s.CopyRel(relName[1], relName[2]);

  const char *cnf = "(supplier.s_suppkey = copy.ps_suppkey)";

  yy_scan_string(cnf);
  yyparse();

  ASSERT_EQ(800, s.Estimate(final, relName, 2));

  free(relName[0]);
  free(relName[1]);
  free(relName[2]);

  free(supKey);
  free(supForeignKey);
}

TEST(StatisticsTest, CopyRelation) {
  Statistics s;
  char *relName[] = {strdup("supplier"), strdup("partsupp")};

  char *supKey = strdup("s_suppkey");
  char *supForeignKey = strdup("ps_suppkey");
  s.AddRel(relName[0], 10);
  s.AddAtt(relName[0], supKey, 10);

  s.AddRel(relName[1], 800);
  s.AddAtt(relName[1], supForeignKey, 10);

  ASSERT_EQ(s.NumUniqueAttributes(), 2);
  ASSERT_EQ(s.NumRelations(), 2);
  ASSERT_EQ(s.NumRelationsWithAttribute(supForeignKey), 1);

  char *newRelName = strdup("copy");
  s.CopyRel(relName[1], newRelName);

  ASSERT_EQ(s.NumUniqueAttributes(), 2);
  ASSERT_EQ(s.NumRelations(), 3);
  ASSERT_EQ(s.NumRelationsWithAttribute(supForeignKey), 2);

  free(relName[0]);
  free(relName[1]);
  free(newRelName);
  free(supKey);
  free(supForeignKey);
}

TEST(StatisticsTest, AddRel) {
  Statistics stats;
  char *testRelName = strdup("nation");
  stats.AddRel(testRelName, 10);

  // doesn't error out, just updates numTuples
  stats.AddRel(testRelName, 15);

  char *testAttName = strdup("n_nationkey");
  stats.AddAtt(testRelName, testAttName, 15);

  free(testRelName);
  free(testAttName);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
