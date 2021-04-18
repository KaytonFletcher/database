#include "StatsHelper.h"

int Stats::GetNumDistinct(AttributeType &attr) {
  switch (attr.index()) {
  case 0:
    return std::get_if<AttributeStat<int>>(&attr)->numDistinct;
  case 1:
    return std::get_if<AttributeStat<double>>(&attr)->numDistinct;
  default:
    return std::get_if<AttributeStat<std::string>>(&attr)->numDistinct;
  }
}

void Stats::UpdateNumDistinct(AttributeType &attr, int numDistinct) {

  AttributeStat<int> *attrInt = std::get_if<AttributeStat<int>>(&attr);
  if (attrInt != nullptr) {
    attrInt->numDistinct = numDistinct;
    return;
  }

  AttributeStat<double> *attrDouble = std::get_if<AttributeStat<double>>(&attr);
  if (attrDouble != nullptr) {
    attrDouble->numDistinct = numDistinct;
    return;
  }

  AttributeStat<std::string> *attrString =
      std::get_if<AttributeStat<std::string>>(&attr);
  if (attrString != nullptr) {
    attrString->numDistinct = numDistinct;
  }
}

double Stats::GetHistogramEstimate(AttributeType &attr, char *val,
                                   ComparisonOp &op) {
  switch (attr.index()) {
  case 0:
    return std::get_if<AttributeStat<int>>(&attr)->histogram.GetEstimate(
        std::stoi(val), op);
  case 1:
    return std::get_if<AttributeStat<double>>(&attr)->histogram.GetEstimate(
        std::stod(val), op);
  default:
    return std::get_if<AttributeStat<std::string>>(&attr)
        ->histogram.GetEstimate(val, op);
  }
}

void Stats::SplitString(char *originalString, std::string &attrName,
                        std::string &relName) {

  std::string temp = originalString;

  std::size_t pos = temp.find('.');
  if (pos == std::string::npos) {
    attrName = temp;
  } else {
    attrName = temp.substr(pos + 1);
    relName = temp.substr(0, pos);
  }
}

void Stats::ResolveRelationName(const std::unordered_set<std::string> &relNames,
                                std::string &relName) {
  if (relNames.size() == 0) {
    std::cerr << "No relations supplied to resolve" << std::endl;
    exit(1);
  }

  if (relNames.size() > 1) {
    auto itr = relNames.find(relName);
    if (relName.empty() || itr == relNames.end()) {
      std::cerr << "Multiple relations have this attribute, must specify "
                   "relation"
                << std::endl;
      exit(1);
    } else if (relName.empty()) {
      relName = *itr;
    }
  } else {
    relName = *relNames.begin();
  }
}

void Stats::ValidateAttributeNames(
    struct AndList *parseTree,
    std::unordered_set<std::string> &relAttributeNames) {

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
        using AttributeType =
            std::variant<AttributeStat<int>, AttributeStat<double>,
                         AttributeStat<std::string>>;
      }

      if (myOr->left->left->code == NAME) {
        std::string attrName;
        std::string relName;
        SplitString(myOr->left->left->value, attrName, relName);
        if (relAttributeNames.find(attrName) == relAttributeNames.end()) {
          std::cerr << "CNF uses attribute not found in the relations "
                       "specified in RelNames"
                    << std::endl;
          exit(1);
        }
      }

      if (myOr->left->right->code == NAME) {
        std::string attrName;
        std::string relName;
        SplitString(myOr->left->right->value, attrName, relName);
        if (relAttributeNames.find(attrName) == relAttributeNames.end()) {
          std::cerr << "CNF uses attribute not found in the relations "
                       "specified in RelNames"
                    << std::endl;
          exit(1);
        }
      }
    }
  }
}

void Stats::GetAttrNameFromMap(
    const std::unordered_map<std::string, AttributeType> &attrMap,
    std::unordered_set<std::string> &attrNames) {
  for (auto const &attrPair : attrMap)
    attrNames.insert(attrPair.first);
}
