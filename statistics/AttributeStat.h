#pragma once

#include "Histogram.h"

template <typename T>
struct AttributeStat {
  int numDistinct = 0;
  Histogram<T> histogram;
};