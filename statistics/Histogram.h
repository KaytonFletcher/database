#pragma once

#include <map>
#include <string>
#include <type_traits>

#include "../db_core/Defs.h"
#include "../db_core/Pipe.h"
#include "../relational_ops/Count.h"

template <typename T> struct identity { typedef T type; };

template <typename T> class Histogram {
private:
  void ConstructFromRecord(Record &rec, Schema &schema, identity<std::string>) {

    char *bits = rec.GetBits();

    int pointer = ((int *)bits)[1];
    int *count = (int *)&(bits[pointer]);

    pointer = ((int *)bits)[2];
    this->numCollected += *count;
    this->counts.insert({std::string(&(bits[pointer])), *count});
  }

  template <typename TL>
  void ConstructFromRecord(Record &rec, Schema &schema, identity<TL>) {

    char *bits = rec.GetBits();

    int pointer = ((int *)bits)[1];
    int *count = (int *)&(bits[pointer]);
    this->numCollected += *count;
    pointer = ((int *)bits)[2];
    this->counts.insert({*(T *)&(bits[pointer]), *count});
  }

public:
  int numCollected = 0;
  std::map<T, int> counts;

  Histogram(Pipe &inPipe, Type valueType, int numBuckets) {
    // if(constexpr std::is_same_v<T, std::string>)
    Attribute countAttr;
    countAttr.myType = Int;
    countAttr.name = strdup("count");

    Attribute valueAttr;
    valueAttr.myType = valueType;
    valueAttr.name = strdup("value");

    Attribute *attrs[2] = {&countAttr, &valueAttr};
    Schema schema("temp", 2, attrs);

    Record buffer;
    while (inPipe.Remove(&buffer)) {
      this->ConstructFromRecord(buffer, schema);
    }
  }

  Histogram() {}

  Histogram(std::map<T, int> &cnts) : counts(cnts) {
    for (const auto &pair : cnts) {
      this->numCollected += pair.second;
    }
  }

  double GetEstimate(T val, ComparisonOp &op) {
    int est = 0;

    switch (op.code) {
    case LESS_THAN:
      for (const auto &[key, count] : this->counts) {
        if (key >= val)
          break;
        est += count;
      }
      break;

    case GREATER_THAN: {
      bool first = true;
      for (auto itr = this->counts.lower_bound(val); itr != counts.end();
           ++itr) {
        if (itr->first != val) {
          est += itr->second;
        }
      }
    } break;
    default:
      auto itr = this->counts.lower_bound(val);
      if (itr != this->counts.end()) {
        est = itr->second;
      } else {
        est = 0;
      }

      break;
    }
    return (double)est / this->numCollected;
  }

  void ConstructFromRecord(Record &rec, Schema &schema) {
    this->ConstructFromRecord(rec, schema, identity<T>());
  }
};
