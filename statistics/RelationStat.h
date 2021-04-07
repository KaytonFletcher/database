#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "../db_core/Schema.h"
#include "AttributeStat.h"

struct RelationStat {
  int numTuples = 0;
  Schema schema;
  std::unordered_map<std::string,
                     std::variant<AttributeStat<int>, AttributeStat<double>,
                                  AttributeStat<std::string>>>
      attributes;
};