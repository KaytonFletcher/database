#include "Comparison.h"

using namespace std;

Comparison::Comparison() {}

Comparison::Comparison(const Comparison &copy_me) {
  operand1 = copy_me.operand1;
  whichAtt1 = copy_me.whichAtt1;
  operand2 = copy_me.operand2;
  whichAtt2 = copy_me.whichAtt2;

  attType = copy_me.attType;

  op = copy_me.op;
}

void Comparison ::Print() {

  cout << "Att " << whichAtt1 << " from ";

  if (operand1 == Left)
    cout << "left record ";
  else if (operand1 == Right)
    cout << "right record ";
  else
    cout << "literal record ";

  if (op == LessThan)
    cout << "< ";
  else if (op == GreaterThan)
    cout << "> ";
  else
    cout << "= ";

  cout << "Att " << whichAtt2 << " from ";

  if (operand2 == Left)
    cout << "left record ";
  else if (operand2 == Right)
    cout << "right record ";
  else
    cout << "literal record ";

  if (attType == Int)
    cout << "(Int)";
  else if (attType == Double)
    cout << "(Double)";
  else
    cout << "(String)";
}

