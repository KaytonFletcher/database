#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "../ParseTree.h"
#include "AttributeStat.h"
#include "RelationStat.h"

using AttributeType = std::variant<AttributeStat<int>, AttributeStat<double>,
                                   AttributeStat<std::string>>;

namespace Stats {

int GetNumDistinct(AttributeType &attr);

void UpdateNumDistinct(AttributeType &attr, int numDistinct);

double GetHistogramEstimate(AttributeType &attr, char* val, ComparisonOp& op);

void SplitString(char *originalString, std::string &attrName,
                 std::string &relName);


// parses AndTree looking for all the attributes used in the CNF
void ValidateAttributeNames(struct AndList *parseTree,
                            std::unordered_set<std::string> &relAttributeNames);

void GetAttrNameFromMap(
    const std::unordered_map<std::string, AttributeType> &attrMap,
    std::unordered_set<std::string> &attrNames);


} // namespace Stats