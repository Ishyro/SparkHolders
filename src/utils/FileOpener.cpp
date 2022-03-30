#include "utils/FileOpener.h"

namespace FileOpener {

  int IntegerReader(std::string integer, Database * database) {
    return database->getTargetFromMacro(integer);
  }

  std::map<const std::string,std::string> getValuesFromIni(std::string fileName) {
    std::map<const std::string,std::string> result = std::map<const std::string,std::string>();
    std::fstream file;
    file.open(fileName, std::ios::in);
    if(!file) {
      std::cout << "ERROR FILE" << std::endl;
    }
    std::string line;
    std::string delimiter = "=";
    while(getline(file,line)) {
      if(line.at(0) != '#') {
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

  Tile * TileOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromIni(fileName);
    std::string name = values.at("name");
    std::istringstream is(values.at("untraversable"));
    int light = stoi(values.at("light"));
    bool untraversable;
    is >> std::boolalpha >> untraversable;
    Tile * tile = new Tile(name,untraversable,light);
    database->addTile(tile);
    return tile;
  }

  Map * MapOpener(std::string fileName, Database * database) {
    std::fstream file;
    file.open(fileName, std::ios::in);
    if(!file) {
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

    Map * map = new Map(name, sizeX, sizeY, outside);

    for(int x = 0; x < sizeX; x++) {
      getline(file,line);
      std::istringstream is(line);
      for(int y = 0; y < sizeY; y++) {
        std::string tile;
        getline(is,tile,' ');
        map->setTile(x,y,database->getTile(tile));
      }
    }

    for(int x = 0; x < sizeX; x++) {
      getline(file,line);
      std::istringstream is(line);
      for(int y = 0; y < sizeY; y++) {
        std::string wall;
        getline(is,wall,' ');
        if(wall != "none") {
          Character * c = new Character(database->getCharacter(wall), x, y, NORTH, map->id);
          c->applyAttributes(database->getAttributes(wall));
          map->addCharacter(c);
        }
      }
    }

    map->calculateLights();
    file.close();
    database->addMap(map);
    return map;
  }
}
