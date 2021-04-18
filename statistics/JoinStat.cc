#include "JoinStat.h"

extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char *);
extern "C" int yyparse(void);
extern struct AndList *whereList;

using AttributeType = std::variant<AttributeStat<int>, AttributeStat<double>,
                                   AttributeStat<std::string>>;

void JoinStat::CrossProduct(const JoinStat &other) {
  this->relations.insert(this->relations.end(),
                         std::make_move_iterator(other.relations.begin()),
                         std::make_move_iterator(other.relations.end()));

  this->numTuples *= other.numTuples;
}

void JoinStat::EquiJoin(const JoinStat &other, int numTuples, double joinProb) {
  this->relations.insert(this->relations.end(),
                         std::make_move_iterator(other.relations.begin()),
                         std::make_move_iterator(other.relations.end()));

  this->prob *= joinProb;

  this->numTuples = numTuples;
}

const AttributeType &JoinStat::GetAttribute(std::string &attrName) {
  for (const auto &rel : this->relations) {
    auto attr = rel.attributes.find(attrName);
    if (attr != rel.attributes.end()) {
      return attr->second;
    }
  }

  std::cerr << "Attribute not found within join" << std::endl;
  exit(1);
}

int JoinStat::JoinEstimate(std::string &attrName, int pos, JoinStat &other,
                           std::string &otherAttrName, int otherPos) {
  RelationStat &rel = this->relations[pos];
  auto &attr = rel.attributes.at(attrName);

  RelationStat &otherRel = other.relations[otherPos];
  auto &otherAttr = otherRel.attributes.at(otherAttrName);

  int numDistinct = Stats::GetNumDistinct(attr);
  int numTuples = rel.numTuples;
  int otherDistinct = Stats::GetNumDistinct(otherAttr);
  int otherTuples = otherRel.numTuples;

  if (numDistinct == otherTuples) {
    return this->numTuples * this->prob * other.prob;
  } else {
    return other.numTuples * this->prob * other.prob;
  }
}

std::pair<double, int> JoinStat::SelectEstimate(std::string &attrName, int pos,
                                                char *value, ComparisonOp &op) {
  double est = Stats::GetHistogramEstimate(
      this->relations[pos].attributes.at(attrName), value, op);
  return {est * this->prob, this->numTuples};
}

bool JoinStat::InsertAttribute(const char *attrName, int numDistinct,
                               int relPos, std::string &dbFilePath) {
  RelationStat &rel = this->relations[relPos];
  auto attrPair = rel.attributes.find(attrName);

  if (numDistinct == -1) {
    numDistinct = rel.numTuples;
  }

  if (attrPair != rel.attributes.end()) {
    Stats::UpdateNumDistinct(attrPair->second, numDistinct);
    return false;
  } else {

    Count count;
    Pipe inPipe(100);
    Pipe outPipe(100);

    DBFile dbfile;
    dbfile.Open((dbFilePath + rel.name + ".bin").c_str());

    CNF tempCNF;
    Record tempLiteral;

    char *selectAllCNF = strdup(
        ("(" + std::string(attrName) + " = " + std::string(attrName) + ")")
            .c_str());
    yy_scan_string(selectAllCNF);
    yyparse();
    tempCNF.GrowFromParseTree(whereList, &rel.schema, tempLiteral);
    int tuplesToSkip = std::ceil((double)rel.numTuples / (double)100000) - 1;
    SelectFile sf;
    sf.Use_n_Pages(16);
    sf.Run(dbfile, inPipe, tempCNF, tempLiteral, tuplesToSkip);

    const std::string cnfStr = "(" + std::string(attrName) + ")";
    yy_scan_string(cnfStr.c_str());
    yyparse();

    OrderMaker order;

    CNF cnf;
    Record literal;

    cnf.GrowFromParseTree(whereList, &rel.schema, literal);
    cnf.GetSortOrders(order);

    count.Use_n_Pages(16);
    count.Run(inPipe, outPipe, order, 1);

    switch (rel.schema.FindType(attrName)) {
    case Int:
      rel.attributes.insert(
          {attrName,
           AttributeStat<int>{numDistinct, Histogram<int>(outPipe, Int, 1)}});
      break;
    case Double:
      rel.attributes.insert(
          {attrName, AttributeStat<double>{
                         numDistinct, Histogram<double>(outPipe, Double, 1)}});
      break;
    default:
      rel.attributes.insert(
          {attrName,
           AttributeStat<std::string>{
               numDistinct, Histogram<std::string>(outPipe, String, 1)}});
    }
    count.WaitUntilDone();
    sf.WaitUntilDone();
    return true;
  }
}

int JoinStat::GetNumTuples() {
  int tot = 0;
  for (const auto &rel : this->relations) {
    tot += rel.numTuples;
  }
  return tot;
}
const RelationStat &JoinStat::GetRelation(int pos) {
  return this->relations[pos];
}

void JoinStat::UpdateRelation(int pos, int numTuples) {
  this->relations[pos].numTuples = numTuples;
}
