#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "../ParseTree.h"
#include "AttributeStat.h"

using AttributeType = std::variant<AttributeStat<int>, AttributeStat<double>,
                                   AttributeStat<std::string>>;

void splitString(char *originalString, std::string &attrName,
                 std::string &relName);

void resolveRelationName(const std::unordered_set<std::string> &relNames,
                         std::string &relName);

// parses AndTree looking for all the attributes used in the CNF
void validateAttributeNames(struct AndList *parseTree,
                            std::unordered_set<std::string> &relAttributeNames);

void getAttrNameFromMap(
    std::unordered_map<std::string, AttributeType> &attrMap,
    std::unordered_set<std::string> &attrNames);