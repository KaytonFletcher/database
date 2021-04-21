#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "../optionals/Relation.h"

namespace TPCH {

const std::string nation = "nation";
const std::string region = "region";
const std::string customer = "customer";
const std::string part = "part";
const std::string partsupp = "partsupp";
const std::string supplier = "supplier";
const std::string orders = "orders";
const std::string lineitem = "lineitem";

const std::string settings = "test.cat";

const int PIPE_SIZE = 100;

void ParseConfigFile(std::unordered_map<std::string, Schema *> &schemas);
} // namespace TPCH