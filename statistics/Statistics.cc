#include "Statistics.h"
#include "AttributeStat.h"
#include "JoinStat.h"
#include "RelationStat.h"
#include "StatsHelper.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

using AttributeType = std::variant<AttributeStat<int>, AttributeStat<double>,
                                   AttributeStat<std::string>>;

Statistics::Statistics() {

  FILE *fp = fopen(settings.c_str(), "r");
  if (fp) {
    char *mem = (char *)malloc(80 * 3);
    char *temp_catalog_path = &mem[0];
    char *temp_dbfile_dir = &mem[80];
    char *temp_tpch_dir = &mem[160];
    char line[80];
    fgets(line, 80, fp);
    sscanf(line, "%s\n", temp_catalog_path);
    fgets(line, 80, fp);
    sscanf(line, "%s\n", temp_dbfile_dir);
    fgets(line, 80, fp);
    sscanf(line, "%s\n", temp_tpch_dir);
    fclose(fp);
    if (!(temp_catalog_path && temp_dbfile_dir && temp_tpch_dir)) {
      std::cerr << " Test settings file 'test.cat' not in correct format.\n";
      free(mem);
      exit(1);
    } else {
      catalog_path = temp_catalog_path;
      dbfile_dir = temp_dbfile_dir;
      tpch_dir = temp_tpch_dir;
      free(mem);
    }
  } else {
    std::cerr << " Test settings files 'test.cat' missing \n";
    exit(1);
  }
}

Statistics::Statistics(Statistics &copyMe) {
  this->joinIDToJoinStat = copyMe.joinIDToJoinStat;
  this->attributes = copyMe.attributes;
  this->nameToRelationID = copyMe.nameToRelationID;
}

Statistics::~Statistics() {}

void Statistics::AddRel(char *relName, int numTuples) {
  auto idPair = this->nameToRelationID.find(relName);
  if (idPair != this->nameToRelationID.end()) {
    auto joinPair = this->joinIDToJoinStat.find(idPair->second.joinID);
    if (joinPair != this->joinIDToJoinStat.end()) {
      joinPair->second.UpdateRelation(idPair->second.pos, numTuples);
    }
  } else {
    this->joinIDToJoinStat.insert(
        {nextJoinID,
         JoinStat{{RelationStat{
                      numTuples, relName, Schema(catalog_path.c_str(), relName),
                      std::unordered_map<std::string, AttributeType>()}},
                  numTuples}});

    this->nameToRelationID[relName] = {nextJoinID++, 0};
  }
}

void Statistics::AddAtt(char *relName, char *attName, int numDistincts) {
  auto idPair = this->nameToRelationID.find(relName);

  if (idPair == this->nameToRelationID.end()) {
    std::cerr << "Relation specified does not exist" << std::endl;
    exit(1);
  }

  auto joinPair = this->joinIDToJoinStat.find(idPair->second.joinID);
  if (joinPair == this->joinIDToJoinStat.end()) {
    std::cerr << "Relation specified does not exist" << std::endl;
    exit(1);
  }

  if (joinPair->second.InsertAttribute(
          attName, numDistincts, idPair->second.pos, Statistics::dbfile_dir)) {
    this->attributes.insert({attName, {idPair->second}});
  }
}

void Statistics::CopyRel(char *oldName, char *newName) {
  auto relIDPair = this->nameToRelationID.find(oldName);
  if (relIDPair == this->nameToRelationID.end()) {
    std::cerr << "Relation specified does not exist, unable to copy"
              << std::endl;
    exit(1);
  }
  const RelationStat &rel = this->joinIDToJoinStat.at(relIDPair->second.joinID)
                                .GetRelation(relIDPair->second.pos);

  this->joinIDToJoinStat.insert({nextJoinID, JoinStat{{rel}, rel.numTuples}});

  this->nameToRelationID[newName] = {nextJoinID++, 0};

  for (const auto &attr : rel.attributes) {
    this->attributes.at(attr.first).insert({nextJoinID - 1, 0});
  }
}

void Statistics::Read(char *fromWhere) {
  std::ifstream in(fromWhere);
  in >> *this;
}

void Statistics::Write(char *fromWhere) {
  std::ofstream out(fromWhere);
  out << *this;
}

void Statistics::Apply(struct AndList *parseTree, char **relNames,
                       int numToJoin) {

  if (numToJoin <= 0) {
    std::cerr << "Number of relations to join must be greater than 0"
              << std::endl;
    exit(1);
  }

  std::vector<JoinBundle> joins;

  validateInput(parseTree, relNames, numToJoin, joins);

  if (parseTree == nullptr) {
    if (joins.size() <= 1) {
      return;
    } else {

      // we cross product every relation in every group. Expensive!
      int joinID = joins[0].id;
      JoinStat &firstJoin = joins[0].stat;
      int index = firstJoin.relations.size();
      for (uint i = 1; i < joins.size(); i++) {
        firstJoin.CrossProduct(joins[i].stat);
        this->cleanupJoin(joins[i].stat, joins[i].id, joinID, index);
      }
    }
  } else {

    std::vector<double> andProbs;
    std::vector<FutureJoin> joins;
    // now we go through and build the comparison structure
    for (int whichAnd = 0; 1; whichAnd++, parseTree = parseTree->rightAnd) {

      // see if we have run off of the end of all of the ANDs
      if (parseTree == nullptr) {
        break;
      }

      std::vector<double> orProbs;
      // we have not, so we iterate over ORs hanging off of this AND
      struct OrList *myOr = parseTree->left;
      for (int whichOr = 0; 1; whichOr++, myOr = myOr->rightOr) {

        // see if we have run off of the end of the ORs
        if (myOr == nullptr) {
          break;
        }

        // we have an equi-join
        if (myOr->left->left->code == NAME && myOr->left->right->code == NAME &&
            myOr->left->code == EQUALS) {

          std::string leftAttrName;
          RelationID leftRelID =
              this->determineRelation(myOr->left->left->value, leftAttrName);

          std::string rightAttrName;
          RelationID rightRelID =
              this->determineRelation(myOr->left->right->value, rightAttrName);

          joins.push_back(
              FutureJoin{leftRelID, leftAttrName, rightRelID, rightAttrName});

        } else if (myOr->left->left->code == NAME) {
          std::pair<double, int> selEst =
              getEstimate(myOr->left->left->value, myOr->left->right->value,
                          myOr->left, orProbs);

        } else if (myOr->left->right->code == NAME) {

          std::pair<double, int> selEst =
              getEstimate(myOr->left->right->value, myOr->left->left->value,
                          myOr->left, orProbs);
        }
      }

      if (!orProbs.empty()) {
        if (orProbs.size() == 1) {
          andProbs.push_back(orProbs[0]);
        } else {
          double andProb = orProbs[0];
          double mult = orProbs[0];

          for (uint i = 1; i < orProbs.size(); i++) {
            andProb += orProbs[i];
            mult *= orProbs[i];
          }
          andProbs.push_back(andProb - mult);
        }
      }
    }

    if (joins.size() > 1) {
      std::cerr << "Not set up to apply more than on join at a time"
                << std::endl;
      exit(1);
    }

    if (!joins.empty()) {
      this->equiJoin(joins[0], andProbs);
    }
  }
}

double Statistics::Estimate(struct AndList *parseTree, char **relNames,
                            int numToJoin) {

  if (numToJoin <= 0) {
    std::cerr << "Number of relations to join must be greater than 0"
              << std::endl;
    exit(1);
  }

  std::vector<JoinBundle> joins;

  validateInput(parseTree, relNames, numToJoin, joins);

  if (parseTree == nullptr) {
    int est = 1;
    // we cross product every relation in every group. Expensive!
    for (const auto &join : joins) {
      est *= join.stat.GetNumTuples();
    }
    return est;
  }

  int equijoin = 0;
  std::vector<double> andProbs;

  // now we go through and build the comparison structure
  for (int whichAnd = 0; 1; whichAnd++, parseTree = parseTree->rightAnd) {

    // see if we have run off of the end of all of the ANDs
    if (parseTree == nullptr) {
      break;
    }

    // we have not, so we iterate over ORs hanging off of this AND
    struct OrList *myOr = parseTree->left;
    std::vector<double> orProbs;
    for (int whichOr = 0; 1; whichOr++, myOr = myOr->rightOr) {

      // see if we have run off of the end of the ORs
      if (myOr == nullptr) {
        break;
      }

      // we have an equi-join
      if (myOr->left->left->code == NAME && myOr->left->right->code == NAME &&
          myOr->left->code == EQUALS) {

        std::string leftAttrName;
        RelationID leftRelID =
            this->determineRelation(myOr->left->left->value, leftAttrName);

        std::string rightAttrName;
        RelationID rightRelID =
            this->determineRelation(myOr->left->right->value, rightAttrName);

        JoinStat &leftJoin = this->joinIDToJoinStat.at(leftRelID.joinID);
        JoinStat &rightJoin = this->joinIDToJoinStat.at(rightRelID.joinID);

        equijoin = leftJoin.JoinEstimate(leftAttrName, leftRelID.pos, rightJoin,
                                         rightAttrName, rightRelID.pos);

      } else if (myOr->left->left->code == NAME) {
        std::pair<double, int> selEst =
            getEstimate(myOr->left->left->value, myOr->left->right->value,
                        myOr->left, orProbs);

        if (equijoin == 0) {
          equijoin = selEst.second;
        }
      } else if (myOr->left->right->code == NAME) {

        std::pair<double, int> selEst =
            getEstimate(myOr->left->right->value, myOr->left->left->value,
                        myOr->left, orProbs);

        if (equijoin == 0) {
          equijoin = selEst.second;
        }
      }
    }
    if (!orProbs.empty()) {
      if (orProbs.size() == 1) {
        andProbs.push_back(orProbs[0]);
      } else {
        double andProb = orProbs[0];
        double mult = orProbs[0];

        for (uint i = 1; i < orProbs.size(); i++) {
          andProb += orProbs[i];
          mult *= orProbs[i];
        }
        andProbs.push_back(andProb - mult);
      }
    }
  }

  if (andProbs.empty()) {
    return equijoin;
  }

  double andProb = andProbs[0];
  for (uint i = 1; i < andProbs.size(); i++) {
    andProb *= andProbs[i];
  }
  return (equijoin * andProb);
}

void Statistics::cleanupJoin(JoinStat &join, int oldJoinID, int newJoinID,
                             int &startingIndex) {
  for (uint i = 0; i < join.relations.size(); i++) {
    RelationStat &tempRel = join.relations[i];
    for (const auto &attrPair : tempRel.attributes) {
      this->attributes.at(attrPair.first)
          .erase(this->nameToRelationID.at(tempRel.name));
      this->attributes.at(attrPair.first).insert({newJoinID, startingIndex});
    }

    this->nameToRelationID.at(tempRel.name) = {newJoinID, startingIndex++};
  }
  this->joinIDToJoinStat.erase(oldJoinID);
}

void Statistics::validateInput(struct AndList *parseTree, char **relNames,
                               int numToJoin, std::vector<JoinBundle> &joins) {

  uint numLeft = numToJoin;
  std::unordered_set<int> visited;
  std::unordered_map<int, int> joinIDToNumRelations;
  std::unordered_set<std::string> relAttributeNames;

  for (int i = 0; i < numToJoin; i++) {
    char *currRelName = relNames[i];

    auto idPair = this->nameToRelationID.find(currRelName);
    if (idPair == this->nameToRelationID.end()) {
      std::cerr << "Relation specified in list to join does not exist: can't "
                   "estimate"
                << std::endl;
      exit(1);
    }

    auto joinPair = this->joinIDToJoinStat.find(idPair->second.joinID);
    if (joinPair == this->joinIDToJoinStat.end()) {
      std::cerr << "Relation specified in list to join does not exist: can't "
                   "estimate"
                << std::endl;
      exit(1);
    }

    JoinStat &join = joinPair->second;
    uint numRelations = join.relations.size();
    const RelationStat &rel = join.GetRelation(idPair->second.pos);

    Stats::GetAttrNameFromMap(rel.attributes, relAttributeNames);

    if (visited.find(i) != visited.end()) {
      continue;
    }
    visited.insert(i);

    std::vector<std::string> relsInJoin(join.relations.size());

    relsInJoin[0] = currRelName;

    // std::cout << "Current rel: " << currRelName << std::endl;

    uint numFound = 1;
    for (int j = i + 1; j < numToJoin; j++) {
      auto tempJoin = this->nameToRelationID.find(relNames[j]);
      if (tempJoin != this->nameToRelationID.end() &&
          tempJoin->second.joinID == idPair->second.joinID) {
        relsInJoin[numFound] = relNames[j];
        numFound++;
        visited.insert(j);
      }
    }

    if (numFound != numRelations) {
      std::cerr << "Relation " << currRelName
                << " is part of a join that was not specified completely: "
                << numFound << " != " << numRelations << std::endl;
      exit(1);
    }
    joins.push_back({joinPair->first, join, relsInJoin});
  }

  Stats::ValidateAttributeNames(parseTree, relAttributeNames);
}

RelationID Statistics::determineRelation(char *originalString,
                                         std::string &attrName) {
  std::string relName;
  Stats::SplitString(originalString, attrName, relName);

  auto rels = this->attributes.find(attrName);

  if (rels == this->attributes.end() || rels->second.empty()) {
    std::cerr << "Error: Attribute not found in any relation" << std::endl;
    exit(1);
  }

  if (rels->second.size() == 1) {
    return *rels->second.begin();
  } else {
    if (relName.empty()) {
      std::cerr << "Error: More than one relation with attribute name, must "
                   "specify relation"
                << std::endl;
      exit(1);
    } else {
      auto idPair = this->nameToRelationID.find(relName);
      if (idPair == this->nameToRelationID.end()) {
        std::cerr
            << "Error: Relation specified in attribute name does not exist"
            << std::endl;
        exit(1);
      } else {
        if (rels->second.find(idPair->second) != rels->second.end()) {
          return idPair->second;
        } else {
          std::cerr << "Error: Relation specified in attribute name does not "
                       "have attribute"
                    << std::endl;
          exit(1);
        }
      }
    }
  }
}

std::istream &operator>>(std::istream &is, Statistics &s) {
  std::string temp;
  std::getline(is, temp);
  int numJoins = std::stoi(temp);

  // std::cout << "Num joins: " << numJoins << std::endl;

  for (int i = 0; i < numJoins; i++) {
    JoinStat join;
    std::getline(is, temp);
    std::stringstream joinSS(temp);

    // std::cout << temp << std::endl;

    int joinID = 0;
    int numRels = 0;

    joinSS >> joinID;
    joinSS >> join.numTuples;
    joinSS >> join.prob;
    joinSS >> numRels;

    // std::cout << "Join ID: " << joinID << std::endl;
    // std::cout << "Join Num Tuples: " << join.numTuples << std::endl;
    // std::cout << "Num rels: " << numRels << std::endl;

    for (int j = 0; j < numRels; j++) {
      std::getline(is, temp);
      std::stringstream relSS(temp);

      RelationStat rel;

      std::string relName;
      int numTuples = 0;
      int numAttrs = 0;

      relSS >> rel.name;
      relSS >> rel.numTuples;
      relSS >> numAttrs;

      // std::cout << "Rel Name: " << rel.name << std::endl;
      // std::cout << "Rel Num Tuples: " << rel.numTuples << std::endl;
      // std::cout << "Num Attrs: " << numAttrs << std::endl;

      RelationID relID = RelationID{joinID, j};
      s.nameToRelationID.insert({rel.name, relID});

      std::string type;
      std::string name;

      for (int k = 0; k < numAttrs; k++) {
        std::getline(is, temp);
        std::stringstream attrSS(temp);
        attrSS >> type;
        attrSS >> name;

        // std::cout << "Type: " << type << std::endl;
        // std::cout << "Att name: " << name << std::endl;

        if (type == "Int") {
          AttributeStat<int> attr;
          is >> attr;
          rel.attributes.insert({name, attr});
          if (s.attributes.find(name) != s.attributes.end()) {
            s.attributes.at(name).insert(relID);
          } else {
            s.attributes.insert({name, {relID}});
          }
        } else if (type == "Double ") {
          AttributeStat<double> attr;
          is >> attr;
          rel.attributes.insert({name, attr});
          if (s.attributes.find(name) != s.attributes.end()) {
            s.attributes.at(name).insert(relID);
          } else {
            s.attributes.insert({name, {relID}});
          }
        } else {
          AttributeStat<std::string> attr;
          is >> attr;
          rel.attributes.insert({name, attr});
          if (s.attributes.find(name) != s.attributes.end()) {
            s.attributes.at(name).insert(relID);
          } else {
            s.attributes.insert({name, {relID}});
          }
        }
      }

      join.relations.push_back(rel);
    }
    s.joinIDToJoinStat.insert({joinID, join});
  }

  return is;
}

std::ostream &operator<<(std::ostream &os, Statistics &s) {
  os << s.joinIDToJoinStat.size() << "\n";
  for (const auto &join : s.joinIDToJoinStat) {
    os << join.first << " " << join.second.numTuples << " " << join.second.prob
       << " " << join.second.relations.size() << "\n";
    for (const auto &rel : join.second.relations) {
      os << rel.name << " " << rel.numTuples << " " << rel.attributes.size()
         << "\n";

      for (auto &attrPair : rel.attributes) {
        auto &attr = attrPair.second;
        const AttributeStat<int> *attrInt =
            std::get_if<AttributeStat<int>>(&attr);
        if (attrInt != nullptr) {
          os << "Int " << attrPair.first << "\n" << *attrInt << "\n";
          continue;
        }

        const AttributeStat<double> *attrDouble =
            std::get_if<AttributeStat<double>>(&attr);
        if (attrDouble != nullptr) {
          os << "Double " << attrPair.first << "\n" << *attrDouble << "\n";
          continue;
        }

        const AttributeStat<std::string> *attrString =
            std::get_if<AttributeStat<std::string>>(&attr);
        if (attrString != nullptr) {
          os << "String " << attrPair.first << "\n" << *attrString << "\n";
          continue;
        }
      }
    }
  }
  return os;
}
