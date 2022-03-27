#include "utils/FileOpener.h"

namespace fileOpener {

  std::map<const std::string,std::string> getValuesFromIni(std::string fileName) {
    std::map<const std::string,std::string> result = std::map<const std::string,std::string>();
    std::fstream file;
    file.open(fileName, std::ios::in);
    if (!file) {
      std::cout << "ERROR FILE" << std::endl;
    }
    std::string line;
    std::string delimiter = "=";
    while(getline(file,line)) {
      if (line.at(0) != '#') {
        while(std::isspace(line.at(0))) {
          line = line.substr(1, line.length());
        }
        std::string key = line.substr(0, line.find(delimiter));
        while(std::isspace(key.at(key.length()))) {
          key = key.substr(0, key.length() - 1);
        }
        std::string value = line.substr(line.find(delimiter), line.length());
        while(std::isspace(value.at(0))) {
          value = value.substr(1, value.length());
        }
        while(std::isspace(value.at(value.length()))) {
          value = value.substr(0, value.length() - 1);
        }
        result.insert(std::make_pair(key,value));
      }
    }
    file.close();
    return result;
  }

  Map getValuesFromMapFile(std::string fileName, World world) {
    std::fstream file;
    file.open(fileName, std::ios::in);
    if (!file) {
      std::cout << "ERROR FILE" << std::endl;
    }
    std::string line;
    std::string name;
    getline(file,name);
    getline(file,line);
    const long sizeX = (long) stoi(line);
    getline(file,line);
    const long sizeY = (long) stoi(line);
    getline(file,line);
    bool outside;
    std::istringstream is(line);
    is >> std::boolalpha >> outside;

    Map result = Map(name, sizeX, sizeY, outside);

    for(int x = 0; x < sizeX; x++) {
      getline(file,line);
      std::istringstream is(line);
      for(int y = 0; y < sizeY; y++) {
        std::string tile;
        getline(is,tile,' ');
        result.setTile(x,y,world.getTile(tile));
      }
    }
    file.close();
    return result;
  }
}
