#include "Statistics.h"
#include "AttributeStat.h"
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
  this->relations = copyMe.relations;
  this->attributes = copyMe.attributes;
  this->joins = copyMe.joins;
}

Statistics::~Statistics() {}

void Statistics::AddRel(char *relName, int numTuples) {
  auto rel = this->relations.find(relName);
  if (rel != this->relations.end()) {
    rel->second.numTuples = numTuples;
  } else {
    this->relations[relName] =
        RelationStat{numTuples, Schema(catalog_path.c_str(), relName),
                     std::unordered_map<std::string, AttributeType>()};

    // join begins as a singleton set for each added relation
    this->joins[relName] = std::unordered_set{std::string(relName)};
  }
}

void Statistics::AddAtt(char *relName, char *attName, int numDistincts) {
  auto relPair = this->relations.find(relName);
  if (relPair == this->relations.end()) {
    std::cerr << "Relation specified does not exist" << std::endl;
    exit(1);
  } else {
    RelationStat &rel = relPair->second;

    switch (rel.schema.FindType(attName)) {
    case Int:
      rel.attributes.insert({attName, AttributeStat<int>{numDistincts}});
    case Double:
      rel.attributes.insert({attName, AttributeStat<double>{numDistincts}});
    case String:
      rel.attributes.insert(
          {attName, AttributeStat<std::string>{numDistincts}});
    }

    auto attrPair = this->attributes.find(attName);
    if (attrPair == this->attributes.end()) {
      this->attributes.insert({attName, {relName}});
    } else {
      attrPair->second.insert(relName);
    }
  }
}

void Statistics::CopyRel(char *oldName, char *newName) {
  auto rel = this->relations.find(oldName);
  if (rel == this->relations.end()) {
    std::cerr << "Relation specified does not exist, unable to change name"
              << std::endl;
    exit(1);
  } else {
    this->relations[newName] = this->relations[oldName];
    this->joins[newName] = this->joins[oldName];

    for (auto attrPair : rel->second.attributes) {
      this->attributes[attrPair.first].insert(newName);
    }
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

  // gets pointers to the "join" sets specified in the "relNames" list
  // i.e. {A,B}, {C}, {D,E} -> {[A, B], [C], [D, E]}
  std::vector<std::unordered_set<std::string> *> groups;

  validateRelNames(relNames, numToJoin, parseTree, groups);

  std::cout << "Number of groups found: " << groups.size() << std::endl;

  if (parseTree == nullptr) {
    std::cout << "No parse tree: we have a cross product" << std::endl;

    int est = 1;
    // we cross product every relation in every group. Expensive!
    for (auto group : groups) {
      for (auto relName : *group) {
        std::cout << relName << std::endl;
        est *= this->relations[relName].numTuples;
      }
    }
    return est;
  }
}

double Statistics::Estimate(struct AndList *parseTree, char **relNames,
                            int numToJoin) {

  if (numToJoin <= 0) {
    std::cerr << "Number of relations to join must be greater than 0"
              << std::endl;
    exit(1);
  }

  // gets pointers to the "join" sets specified in the "relNames" list
  // i.e. {A,B}, {C}, {D,E} -> {[A, B], [C], [D, E]}
  std::vector<std::unordered_set<std::string> *> groups;

  validateRelNames(relNames, numToJoin, parseTree, groups);

  std::cout << "Number of groups found: " << groups.size() << std::endl;

  if (parseTree == nullptr) {
    std::cout << "No parse tree: we have a cross product" << std::endl;

    int est = 1;
    // we cross product every relation in every group. Expensive!
    for (auto group : groups) {
      for (auto relName : *group) {
        std::cout << relName << std::endl;
        est *= this->relations[relName].numTuples;
      }
    }
    return est;
  }

  // now we go through and build the comparison structure
  for (int whichAnd = 0; 1; whichAnd++, parseTree = parseTree->rightAnd) {

    // see if we have run off of the end of all of the ANDs
    if (parseTree == nullptr) {
      break;
    }

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
        std::string leftRelName;
        splitString(myOr->left->left->value, leftAttrName, leftRelName);

        std::string rightAttrName;
        std::string rightRelName;
        splitString(myOr->left->right->value, rightAttrName, rightRelName);

        auto leftAttrPair = this->attributes.find(leftAttrName);
        auto rightAttrPair = this->attributes.find(rightAttrName);

        if (leftAttrPair == this->attributes.end() ||
            rightAttrPair == this->attributes.end()) {
          std::cerr << "Error: Attribute specified not found in any relation"
                    << std::endl;
          exit(1);
        }

        resolveRelationName(leftAttrPair->second, leftRelName);
        resolveRelationName(rightAttrPair->second, rightRelName);

        auto leftJoinNames = this->joins.find(leftRelName);
        if (leftJoinNames != this->joins.end()) {
          std::cerr << "Error: Relation not a part of a join:" << leftRelName
                    << std::endl;
          exit(1);
        }

        int leftDistinct = 0;
        for (auto &relName : leftJoinNames->second) {
          auto rel = this->relations.find(relName);
          if (rel == this->relations.end()) {
            std::cerr << "Error: Relation in join not found:" << relName
                      << std::endl;
            exit(1);
          } else {
          }
        }

        RelationStat &leftRel = this->relations[leftRelName];
        RelationStat &rightRel = this->relations[rightRelName];

        AttributeStat &leftAttr = leftRel.attributes[leftAttrName];
        AttributeStat &rightAttr = rightRel.attributes[rightAttrName];

      } else if (myOr->left->left->code == NAME) {
      }
    }
  }

  return 0.0;
}

void Statistics::validateRelNames(
    char **relNames, int numToJoin, struct AndList *parseTree,
    std::vector<std::unordered_set<std::string> *> &groups) {

  uint numLeft = numToJoin;
  std::unordered_set<int> visited;
  std::unordered_set<std::string> relAttributeNames;

  for (int i = 0; i < numToJoin; i++) {
    char *currRelName = relNames[i];
    auto rel = this->relations.find(currRelName);
    if (rel == this->relations.end()) {
      std::cerr << "Relation specified in list to join does not exist: can't "
                   "estimate"
                << std::endl;
      exit(1);
    } else {

      getAttrNameFromMap(rel->second.attributes, relAttributeNames);

      if (visited.find(i) != visited.end()) {
        continue;
      }
      visited.insert(i);

      auto join = this->joins[currRelName];

      if (join.size() > numLeft) {
        std::cerr << "Relation " << currRelName
                  << " is part of a join that was not specified completely"
                  << std::endl;
        exit(1);
      }

      numLeft--;
      uint numFound = 1;
      for (int j = i + 1; j < numToJoin; j++) {
        if (join.find(std::string(relNames[j])) != join.end()) {
          numLeft--;
          numFound++;
          visited.insert(j);
        }
      }
      if (numFound != join.size()) {
        std::cerr << "Relation " << currRelName
                  << " is part of a join that was not specified completely"
                  << std::endl;
        exit(1);
      }

      groups.push_back(&this->joins[currRelName]);
    }
  }

  validateAttributeNames(parseTree, relAttributeNames);
}

std::istream &operator>>(std::istream &is, Statistics &s) {
  std::string temp;
  std::getline(is, temp);
  int numRelations = std::stoi(temp);

  // std::cout << "Num relations: " << numRelations << std::endl;

  for (int i = 0; i < numRelations; i++) {
    std::string line;
    std::getline(is, line);
    std::stringstream ss(line);

    std::string relName;
    int numTuples = 0;
    int numAttrs = 0;

    ss >> relName;
    ss >> numTuples;
    ss >> numAttrs;

    // std::cout << "Relation Name: " << relName << std::endl;
    // std::cout << "Num Tuples: " << numTuples << std::endl;
    // std::cout << "Num Attrs: " << numAttrs << std::endl;

    std::unordered_map<std::string, AttributeStat<AttributeType>> attributes;

    for (int j = 0; j < numAttrs; j++) {
      std::string attrName;
      int numDistinct = 0;
      ss >> attrName;
      ss >> numDistinct;

      attributes.insert({attrName, AttributeStat{numDistinct}});

      if (s.attributes.find(attrName) != s.attributes.end()) {
        s.attributes[attrName].insert(relName);
      } else {
        s.attributes[attrName] = {relName};
      }
    }

    s.relations[relName] = RelationStat{numTuples, attributes};

    // assumes no apply was called, every join is a singleton
    s.joins[relName] = {relName};
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, Statistics &s) {
  os << s.relations.size() << "\n";
  for (auto rel : s.relations) {
    os << rel.first << " " << rel.second.numTuples << " "
       << rel.second.attributes.size() << " ";
    for (auto &attrPair : rel.second.attributes) {
      auto &attr = attrPair.second;
      AttributeStat<int> *attrInt = std::get_if<AttributeStat<int>>(&attr);
      if (attrInt != nullptr) {
        os << attrPair.first << " " << attrInt->numDistinct << "\n";
        continue;
      }

      AttributeStat<double> *attrDouble =
          std::get_if<AttributeStat<double>>(&attr);
      if (attrDouble != nullptr) {
        os << attrPair.first << " " << attrDouble->numDistinct << "\n";
        continue;
      }

      AttributeStat<std::string> *attrString =
          std::get_if<AttributeStat<std::string>>(&attr);
      if (attrString != nullptr) {
        os << attrPair.first << " " << attrString->numDistinct << "\n";
        continue;
      }
    }
  }

  return os;
}
