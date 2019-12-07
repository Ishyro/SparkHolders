#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <string>

#include "src/data/Map.h"

class World {
  public:
    const std::string name;
    World(std::string name);
    bool addMap(Map map);
    bool changeMap(std::string name);
    long getTick();
    void incrTick();
  private:
    Map * current;
    long tick;
    std::list<Map *> maps;
    std::list<MapLink *> links;
};

#endif // _WORLD_H_
