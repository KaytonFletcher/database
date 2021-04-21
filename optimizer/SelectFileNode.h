#pragma once

#include "../compare/CNF.h"
#include "../compare/OrderMaker.h"
#include "../db_core/Function.h"
#include "../optionals/Relation.h"
#include "Node.h"

class SelectFileNode : public Node {
private:
  CNF selection;
  Record literal;
  int numToSkip = 0;
  Schema outputSchema;

public:
  SelectFileNode(Schema &schema, int outPipeID, std::string aliasName);

  const Schema &GetOutputSchema() const { return outputSchema; }

  void Print() const {
    std::cout << "SELECT FILE operation" << std::endl;
    std::cout << "Input Pipe " << leftInputPipeID << std::endl;
    std::cout << "Output Pipe " << outPipeID << std::endl;
    std::cout << "Output Schema: " << std::endl;
    this->outputSchema.Print(this->aliasRelName);
    std::cout << std::endl;
  }
};