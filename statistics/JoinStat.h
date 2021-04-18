#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>

#include "../db_core/Schema.h"
#include "../relational_ops/SelectFile.h"
#include "AttributeStat.h"
#include "Histogram.h"
#include "RelationStat.h"
#include "StatsHelper.h"

struct JoinStat {
  std::vector<RelationStat> relations;
  int numTuples = 0;
  double prob = 1.0;
  void CrossProduct(const JoinStat &other);
  void EquiJoin(const JoinStat &other, int numTuples, double prob);

  const AttributeType &GetAttribute(std::string &attrName);

  int JoinEstimate(std::string &attrName, int pos, JoinStat &other,
                   std::string &otherAttrName, int otherPos);

  std::pair<double, int> SelectEstimate(std::string &attrName, int pos,
                                        char *value, ComparisonOp &op);

  bool InsertAttribute(const char *attrName, int numDistinct, int relPos,
                       std::string &dbFilePath);

  int GetNumTuples();

  const RelationStat &GetRelation(int pos);

  void UpdateRelation(int pos, int numTuples);
};

struct JoinBundle {
  int id = 0;
  JoinStat &stat;
  std::vector<std::string> relNames;
};
