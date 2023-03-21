#include <stdexcept>

#include "util/String.h"

#include "data/Database.h"
#include "data/Speech.h"

std::list<std::string> Speech::getOptions() {
  std::list<std::string> result = std::list<std::string>();
  for(auto pair : options) {
    result.push_back(pair.first);
  }
  return result;
}


Speech * Speech::getNext(const std::string response, Database * database) {
  try {
    return (Speech *) database->getSpeech(options.at(response));
  } catch (const std::out_of_range &e) {
    return nullptr;
  }
}

std::string Speech::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, type);
  String::insert_bool(ss, empty);
  String::insert_bool(ss, constant);
  std::stringstream * ss_options = new std::stringstream();
  for(auto pair : options) {
    String::insert(ss_options, pair.first);
    String::insert(ss_options, pair.second);
  }
  String::insert(ss, ss_options->str());
  delete ss_options;
  std::string result = ss->str();
  delete ss;
  return result;
}

Speech * Speech::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int type = String::extract_int(ss);
  bool empty = String::extract_bool(ss);
  bool constant = String::extract_bool(ss);
  std::stringstream * ss_options = new std::stringstream(String::extract(ss));
  std::map<const std::string, const std::string> * options = new std::map<const std::string, const std::string>();
  while(ss_options->rdbuf()->in_avail() != 0) {
    options->insert(std::make_pair((const std::string) String::extract(ss_options), (const std::string) String::extract(ss_options)));
  }
  delete ss_options;
  delete ss;
  Speech * result = new Speech(name, type, empty, constant, *options);
  delete options;
  return result;
}
