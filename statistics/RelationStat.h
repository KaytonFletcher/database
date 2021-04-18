#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "../db_core/Schema.h"
#include "AttributeStat.h"

struct RelationStat {
  int numTuples = 0;
  std::string name;
  Schema schema;
  std::unordered_map<std::string,
                     std::variant<AttributeStat<int>, AttributeStat<double>,
                                  AttributeStat<std::string>>>
      attributes;

  RelationStat() : schema("", 0, nullptr) {}
  RelationStat(
      int nTuples, char *name, Schema sch,
      std::unordered_map<std::string,
                         std::variant<AttributeStat<int>, AttributeStat<double>,
                                      AttributeStat<std::string>>> attrs)
      : numTuples(nTuples), name(name), schema(sch), attributes(attrs) {}
};

struct RelationID {
  int joinID = 0;
  int pos = 0;

  bool operator==(const RelationID &otherRelID) const {
    if (this->joinID == otherRelID.joinID && this->pos == otherRelID.pos)
      return true;
    else
      return false;
  }

  struct HashFunction {

    size_t operator()(const RelationID &relID) const noexcept {
      size_t xHash = std::hash<int>()(relID.joinID);
      size_t yHash = std::hash<int>()(relID.pos) << 1;
      return xHash ^ yHash;
    }
  };
};