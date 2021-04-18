#include "AttributeStat.h"
#include "Histogram.h"
#include <bits/types/FILE.h>
#include <sstream>

std::istream &operator>>(std::istream &is, AttributeStat<int> &s) {
  int numTuples = 0;

  std::string line;
  std::getline(is, line);
  std::stringstream ss(line);

  ss >> s.numDistinct;
  ss >> s.histogram.numCollected;
  ss >> numTuples;
  for (int i = 0; i < numTuples;) {

    std::getline(is, line);
    std::stringstream ss(line);
    while (ss.tellg() != -1) {
      int value;
      int count = 0;

      ss >> value;
      ss >> count;

      // std::cout << "Value: " << value << std::endl;
      // std::cout << "Count: " << count << std::endl;

      i++;
      s.histogram.counts.insert({value, count});
    }
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, const AttributeStat<int> &s) {
  os << s.numDistinct << " " << s.histogram.numCollected << " "
     << s.histogram.counts.size() << "\n";

  const int MAX_PER_LINE = 10000;
  uint cnt = 0;
  for (const auto &pair : s.histogram.counts) {
    cnt++;
    if (cnt == MAX_PER_LINE) {
      cnt = 0;
      os << "\n" << pair.first << " " << pair.second << " ";
    } else if (cnt == MAX_PER_LINE - 1 || cnt == s.histogram.counts.size()) {
      os << pair.first << " " << pair.second;
    } else {
      os << pair.first << " " << pair.second << " ";
    }
  }
  return os;
}

std::istream &operator>>(std::istream &is, AttributeStat<double> &s) {
  int numTuples = 0;

  std::string line;
  std::getline(is, line);
  std::stringstream ss(line);

  ss >> s.numDistinct;
  ss >> s.histogram.numCollected;
  ss >> numTuples;
  for (int i = 0; i < numTuples;) {

    std::getline(is, line);
    std::stringstream ss(line);
    while (ss.tellg() != -1) {
      double value;
      int count = 0;

      ss >> value;
      ss >> count;

      i++;
      s.histogram.counts.insert({value, count});
    }
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, const AttributeStat<double> &s) {
  os << s.numDistinct << " " << s.histogram.numCollected << " "
     << s.histogram.counts.size() << "\n";

  const int MAX_PER_LINE = 10000;
  uint cnt = 0;
  for (const auto &pair : s.histogram.counts) {
    cnt++;
    if (cnt == MAX_PER_LINE) {
      cnt = 0;
      os << "\n" << pair.first << " " << pair.second << " ";
    } else if (cnt == MAX_PER_LINE - 1 || cnt == s.histogram.counts.size()) {
      os << pair.first << " " << pair.second;
    } else {
      os << pair.first << " " << pair.second << " ";
    }
  }
  return os;
}

std::istream &operator>>(std::istream &is, AttributeStat<std::string> &s) {
  int numTuples = 0;

  std::string line;
  std::getline(is, line);
  std::stringstream ss(line);

  ss >> s.numDistinct;
  ss >> s.histogram.numCollected;
  ss >> numTuples;

  for (int i = 0; i < numTuples;) {
    std::string line;
    std::getline(is, line);

    uint pos = 0;
    while (pos <= line.size() - 1) {
      std::string value;
      int count = 0;

      uint first = line.find("\"", pos);
      uint second = line.find("\"", first + 1);
      pos = second + 1;
      value = line.substr(first + 1, second - first - 1);

      first = line.find(" ", pos);
      second = line.find(" ", first + 1);

      count = std::stoi(line.substr(first + 1, second - first - 1));

      pos = second + 1;

      // std::cout << "Value: " << value << std::endl;

      i++;
      s.histogram.counts.insert({value, count});
    }
  }
  return is;
}

std::ostream &operator<<(std::ostream &os,
                         const AttributeStat<std::string> &s) {
  os << s.numDistinct << " " << s.histogram.numCollected << " "
     << s.histogram.counts.size() << "\n";

  uint cnt = 0;
  for (const auto &pair : s.histogram.counts) {
    cnt++;
    if (cnt == 10000) {
      cnt = 0;
      os << "\n"
         << "\"" << pair.first << "\" " << pair.second << " ";
    } else {
      os << "\"" << pair.first << "\" " << pair.second << " ";
    }
  }
  return os;
}
