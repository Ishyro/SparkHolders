#include "data/Speech.h"


std::string Speech::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert(ss, content);
  std::string result = ss->str();
  delete ss;
  return result;
}

Speech * Speech::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  std::string content = String::extract(ss);
  delete ss;
  return new Speech(name, content);
}
