#pragma once

#include <thread>

#include "../db_core/Function.h"
#include "../db_core/Pipe.h"
#include "../db_core/Record.h"
#include "../db_file/DBFile.h"

class RelationalOp {
public:
  std::thread worker;

  // blocks the caller until the particular relational operator
  // has run to completion
  virtual void WaitUntilDone() = 0;

  // tell us how much internal memory the operation can use
  virtual void Use_n_Pages(int n) = 0;
};
