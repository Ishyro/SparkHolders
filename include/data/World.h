#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <string>
#include <map>

class World {
  public:
    const std::string name;
    World(std::string name):name(name) {
      maps = std::map<const long, Map *>();
      links = std::list<MapLink *>();
    }
    void addMap(Map * map);
    void addMapLink(MapLink * link);
    Map * getMap(long map_id);
    Map * getMap(std::string name);
    std::map<const long, Map *> getMaps();
  private:
    std::map<const long, Map *> maps;
    std::list<MapLink *> links;
};

#endif // _WORLD_H_
