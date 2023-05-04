#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <string>
#include <map>

typedef struct MapLink {
    int x1;
    int y1;
    int x2;
    int y2;
    int type;
    Map * map1;
    Map * map2;
} MapLink;

class World {
  public:
    const std::string name;
    World(std::string name):name(name) {
      maps = std::map<const int, Map *>();
      links = std::list<MapLink *>();
    }
    void addMap(Map * map);
    void addMapLink(MapLink * link);
    Map * getMap(int map_id);
    Map * getMap(std::string name);
    std::list<Map *> getMaps();
    MapLink * getMapLink(int y, int x, int mapId);
    std::list<Character *> getCharacters();
    Character * getCharacter(long id);
    float setPathToTarget(int map_id, float x, float y, Target * target);
    float distance(int map_id, float x, float y, Target * target);
  private:
    std::map<const int, Map *> maps;
    std::list<MapLink *> links;
};

#endif // _WORLD_H_
