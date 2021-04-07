#pragma once

#include <map>
#include <string>

#include "../db_core/Defs.h"
#include "../db_core/Pipe.h"
#include "../relational_ops/Count.h"

// extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char *);
// extern "C" int yyparse(void);
// extern struct AndList *final;

template <typename T> class Histogram {
private:
  std::map<T, int> counts;

public:
  Histogram() {
    Pipe inPipe(100);
    Pipe outPipe(100);
    Count count()
  }

  int GetEstimate(T val, CompOperator op) {
    int est = 0;

    switch (op) {
    case LessThan:
      for (const auto &[key, count] : this->counts) {
        if (key >= val)
          break;
        est += count;
      }
      break;

    case GreaterThan: {
      auto itr = this->counts.lower_bound(val);
      while (itr != this->counts.end()) {
        est += itr.second;
      }
    } break;
    default:
      auto itr = this->counts.lower_bound(val);
      if (itr != this->counts.end()) {
        est = itr.second;
      } else {
        est = 0;
      }

      break;
    }
    return est;
  }
};