#pragma once

#include "../db_core/Schema.h"

class Node {
protected:
  int leftInputPipeID = 0;
  int rightInputPipeID = 0;
  int outPipeID = 0;
  Node *left = nullptr;
  Node *right = nullptr;
  std::string aliasRelName;
  
  Node(int outPipeID, std::string &aliasName)
      : outPipeID(outPipeID), aliasRelName(aliasName) {}

public:
  virtual void Print() const = 0;
  virtual const Schema &GetOutputSchema() const = 0;
  Node *GetLeft() { return left; }
  Node *GetRight() { return right; }
  void SetLeft(Node *tempLeft) { this->left = tempLeft; }
  void SetRight(Node *tempRight) { this->right = tempRight; }
};