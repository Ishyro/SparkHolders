#ifndef _STRING_H_
#define _STRING_H_

#include <string>
#include <sstream>

namespace String {
  std::string extract(std::stringstream * ss);
  bool extract_bool(std::stringstream * ss);
  int extract_int(std::stringstream * ss);
  long extract_long(std::stringstream * ss);
  float extract_float(std::stringstream * ss);

  void insert(std::stringstream * ss, std::string data);
  void insert_bool(std::stringstream * ss, bool data);
  void insert_int(std::stringstream * ss, int data);
  void insert_long(std::stringstream * ss, long data);
  void insert_float(std::stringstream * ss, float data);
};

#endif // _STRING_H_
