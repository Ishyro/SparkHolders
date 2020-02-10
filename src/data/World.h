#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <string>
#include <map>

#include "src/data/Map.h"

class World {
  public:
    const std::string name;
    World(std::string name) {
      // TODO XML_READER
    }
    void addMap(Map * map);
    void addMapLink(MapLink * link);
    Map * getMap(int map_id);
  private:
    std::map<long, Map *> maps;
    std::list<MapLink *> links;
};

#endif // _WORLD_H_
