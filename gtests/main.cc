#include "../statistics/Statistics.h"

#include "gtest/gtest.h"
#include <cstdlib>

// extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char *);
// extern "C" int yyparse(void);
// extern struct AndList *final;

TEST(StatisticsDeathTest, NoRelations) {
  Statistics stats;
  ASSERT_DEATH(stats.Estimate(nullptr, nullptr, 0),
               "Number of relations to join must be greater than 0");
}

TEST(StatisticsDeathTest, SingleRelationDeath) {
  Statistics stats;

  char *testRelName = strdup("test_rel");
  stats.AddRel(testRelName, 10);

  char *badRelName[1] = {strdup("not_test_rel")};
  ASSERT_DEATH(
      stats.Estimate(nullptr, badRelName, 1),
      "Relation specified in list to join does not exist: can't estimate");

  free(testRelName);
  free(badRelName[0]);
}

TEST(StatisticsTest, CrossProduct) {
  Statistics stats;
  char *testRelName = strdup("test_rel");
  char *secondRelName = strdup("rel_to_cross");
  char *relNames[2] = {testRelName, secondRelName};
  stats.AddRel(testRelName, 42);
  stats.AddRel(secondRelName, 500);

  ASSERT_EQ(21000, stats.Estimate(nullptr, relNames, 2));

  free(testRelName);
  free(secondRelName);
}

TEST(StatisticsTest, ReadAndWrite) {
  Statistics s;
  char *relName[] = {strdup("s"), strdup("p"), strdup("useless")};

  char *supKey = strdup("s_suppkey");
  char *supForeignKey = strdup("ps_suppkey");
  s.AddRel(relName[0], 10);
  s.AddAtt(relName[0], supKey, 10);

  s.AddRel(relName[1], 800);
  s.AddAtt(relName[1], supForeignKey, 10);

  s.AddRel(relName[2], 2);
  s.AddAtt(relName[2], supForeignKey, 10);

  ASSERT_EQ(s.NumAttributes(), 2);
  ASSERT_EQ(s.NumRelations(), 3);
  ASSERT_EQ(s.NumRelationsWithAttribute(supForeignKey), 2);

  char *fileName = strdup("mmasdnasdanfa");

  s.Write(fileName);

  Statistics newStats;
  newStats.Read(fileName);

  ASSERT_EQ(newStats.NumAttributes(), 2);
  ASSERT_EQ(newStats.NumRelations(), 3);
  ASSERT_EQ(newStats.NumRelationsWithAttribute(supForeignKey), 2);

  remove(fileName);

  free(relName[0]);
  free(relName[1]);
  free(relName[2]);
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

TEST(StatisticsTest, NameResolve) {
  Statistics s;
  char *relName[] = {strdup("s"), strdup("p")};

  char *supKey = strdup("s_suppkey");
  char *supForeignKey = strdup("ps_suppkey");
  s.AddRel(relName[0], 10);
  s.AddAtt(relName[0], supKey, 10);

  s.AddRel(relName[1], 800);
  s.AddAtt(relName[1], supForeignKey, 10);

  const char *cnf = "(s.s_suppkey = p.ps_suppkey)";

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
  char *relName[] = {strdup("s"), strdup("p"), strdup("useless")};

  char *supKey = strdup("s_suppkey");
  char *supForeignKey = strdup("ps_suppkey");
  s.AddRel(relName[0], 10);
  s.AddAtt(relName[0], supKey, 10);

  s.AddRel(relName[1], 800);
  s.AddAtt(relName[1], supForeignKey, 10);

  s.AddRel(relName[2], 2);
  s.AddAtt(relName[2], supForeignKey, 10);

  const char *cnf = "(s.s_suppkey = p.ps_suppkey)";

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
  char *relName[] = {strdup("s"), strdup("p"), strdup("useless")};

  char *supKey = strdup("s_suppkey");
  char *supForeignKey = strdup("ps_suppkey");
  s.AddRel(relName[0], 10);
  s.AddAtt(relName[0], supKey, 10);

  s.AddRel(relName[1], 800);
  s.AddAtt(relName[1], supForeignKey, 10);

  s.AddRel(relName[2], 2);
  s.AddAtt(relName[2], supForeignKey, 10);

  ASSERT_EQ(s.NumAttributes(), 2);
  ASSERT_EQ(s.NumRelations(), 3);
  ASSERT_EQ(s.NumRelationsWithAttribute(supForeignKey), 2);

  char *newRelName = strdup("newRel");
  s.CopyRel(relName[1], newRelName);

  ASSERT_EQ(s.NumAttributes(), 2);
  ASSERT_EQ(s.NumRelations(), 4);
  ASSERT_EQ(s.NumRelationsWithAttribute(supForeignKey), 3);

  free(relName[0]);
  free(relName[1]);
  free(relName[2]);
  free(newRelName);
  free(supKey);
  free(supForeignKey);
}

// TEST(StatisticsDeathTest, SingleRelationSuccess) {

//   Statistics stats;
//   char *testRelName[1] = {strdup("test_rel")};

//   stats.AddRel(testRelName[0], 10);

//   EXPECT_EXIT(stats.Estimate(nullptr, testRelName, 1),
//               testing::ExitedWithCode(0), "Success");
// }

TEST(StatisticsTest, AddRel) {
  Statistics stats;
  char *testRelName = strdup("test_rel");
  stats.AddRel(testRelName, 10);

  // doesn't error out, just updates numTuples
  stats.AddRel(testRelName, 15);

  char *testAttName = strdup("test_key");
  stats.AddAtt(testRelName, testAttName, 15);

  free(testRelName);
  free(testAttName);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
