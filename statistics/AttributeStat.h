#pragma once

#include "Histogram.h"

#include <sstream>


template <typename T> struct AttributeStat {
  int numDistinct = 0;
  Histogram<T> histogram;
};

std::istream &operator>>(std::istream &is, AttributeStat<int> &s);

std::ostream &operator<<(std::ostream &os, const AttributeStat<int> &s);

std::istream &operator>>(std::istream &is, AttributeStat<double> &s);

std::ostream &operator<<(std::ostream &os, const AttributeStat<double> &s);

std::istream &operator>>(std::istream &is, AttributeStat<std::string> &s);

std::ostream &operator<<(std::ostream &os, const AttributeStat<std::string> &s);