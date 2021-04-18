#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../ParseTree.h"
#include "../db_core/Pipe.h"
#include "../db_file/DBFile.h"
#include "../relational_ops/Count.h"
#include "AttributeStat.h"
#include "Histogram.h"
#include "JoinStat.h"
#include "RelationStat.h"
#include "StatsHelper.h"

struct FutureJoin {
  RelationID &leftRelID;
  std::string &leftAttrName;
  RelationID &rightRelID;
  std::string &rightAttrName;
};

class Statistics {
private:
  int nextJoinID = 0;

  std::unordered_map<int, JoinStat> joinIDToJoinStat;
  std::unordered_map<std::string, RelationID> nameToRelationID;
  std::unordered_map<std::string,
                     std::unordered_set<RelationID, RelationID::HashFunction>>
      attributes;

  void validateInput(struct AndList *parseTree, char **relNames, int numToJoin,
                     std::vector<JoinBundle> &joins);

  RelationID determineRelation(char *originalString, std::string &attrName);

  void cleanupJoin(JoinStat &join, int oldJoinID, int newJoinID,
                   int &startingIndex);

  std::pair<double, int> getEstimate(char *inputAttrName, char *literalValue,
                                     ComparisonOp *op,
                                     std::vector<double> &orProbs) {
    std::string attrName;
    RelationID relID = this->determineRelation(inputAttrName, attrName);

    JoinStat &join = this->joinIDToJoinStat.at(relID.joinID);

    auto est = join.SelectEstimate(attrName, relID.pos, literalValue, *op);

    orProbs.push_back(est.first);
    return est;
  }

  void equiJoin(FutureJoin &futureJoin, std::vector<double> &andProbs) {
    JoinStat &leftJoin = this->joinIDToJoinStat.at(futureJoin.leftRelID.joinID);
    JoinStat &rightJoin =
        this->joinIDToJoinStat.at(futureJoin.rightRelID.joinID);

    int index = leftJoin.relations.size();

    double joinProb = 1.0;
    if (!andProbs.empty()) {
      joinProb = andProbs[0];

      for (uint i = 1; i < andProbs.size(); i++) {
        joinProb *= andProbs[i];
      }
    }

    int leftJoinEst = leftJoin.JoinEstimate(
        futureJoin.leftAttrName, futureJoin.leftRelID.pos, rightJoin,
        futureJoin.rightAttrName, futureJoin.rightRelID.pos);
    leftJoin.EquiJoin(rightJoin, leftJoinEst, joinProb);

    this->cleanupJoin(rightJoin, futureJoin.rightRelID.joinID,
                      futureJoin.leftRelID.joinID, index);
  }

  inline static const std::string settings = "test.cat";

  // This file is used to parse the database schemas
  inline static std::string catalog_path = "catalog";

  // dir where .bin files created by DBFile class can be found or created
  inline static std::string dbfile_dir = "";

  // dir where dbgen tpch files (extension *.tbl) can be found
  inline static std::string tpch_dir = "./";

public:
  Statistics();
  Statistics(Statistics &copyMe); // Performs deep copy
  ~Statistics();
  int NumRelations() { return this->nameToRelationID.size(); }
  int NumUniqueAttributes() { return this->attributes.size(); }
  int NumRelationsWithAttribute(const std::string &attr) {
    if (this->attributes.find(attr) != this->attributes.end()) {
      return this->attributes[attr].size();
    } else {
      return 0;
    }
  }

  void AddRel(char *relName, int numTuples);
  void AddAtt(char *relName, char *attName, int numDistincts);
  void CopyRel(char *oldName, char *newName);

  void Read(char *fromWhere);
  void Write(char *fromWhere);

  void Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
  double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);

  friend std::istream &operator>>(std::istream &is, Statistics &o);
  friend std::ostream &operator<<(std::ostream &os, Statistics &o);
};