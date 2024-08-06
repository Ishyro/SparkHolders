#include <fstream>
#include <iostream>
#include <stdexcept>
#include <regex>

#include "clients/EnglishKeyHolder.h"

EnglishKeyHolder::EnglishKeyHolder(std::list<std::string> files) {
  strings = std::map<const std::string, const std::string>();

  for(std::string path : files) {
    loadValuesFromFile(path + "/ENGLISH_KEYS.data", &strings);
  }
}
const std::string EnglishKeyHolder::getEnglishFromKey(std::string key) {
  try {
    return strings.at(key);
  } catch (const std::out_of_range &e) {
    return key;
  }
}

 void EnglishKeyHolder::loadValuesFromFile(const std::string fileName, std::map<const std::string, const std::string> * map) {
  std::fstream file;
  std::string os_fileName = std::regex_replace(fileName, std::regex("/"), PATH_DELIMITER);
  file.open(os_fileName, std::ios::in);
  if(!file) {
    return;
  }
  std::string line;
  std::string delimiter = "=";

  while(getline(file, line, '%') && line != "") {
    while(line != "" && std::isspace(line.at(0))) {
      line = line.substr(1, line.length());
    }
    if(line != "" && line.at(0) != '#') {
      std::string key = line.substr(0, line.find(delimiter));
      while(key.length() > 0 && std::isspace(key.at(key.length() - 1))) {
        key = key.substr(0, key.length() - 1);
      }
      if(line.length() - 1 > line.find(delimiter)) {
        std::string value = line.substr(line.find(delimiter) + 1, line.length() - 1);
        while(value.length() > 0 && std::isspace(value.at(0))) {
          value = value.substr(1, value.length() - 1);
        }
        map->insert(std::make_pair(key,value));
      }
      else {
        map->insert(std::make_pair(key,""));
      }
    }
  }
  file.close();
}
