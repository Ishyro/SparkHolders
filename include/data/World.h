#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <string>
#include <map>
#include <set>

typedef struct MapLink {
    int x1;
    int y1;
    int z1;
    int x2;
    int y2;
    int z2;
    int type;
    Map * map1;
    Map * map2;
} MapLink;

class World {
  public:
    const std::string name;
    World(std::string name):name(name) {
      maps = std::map<const long, Map *>();
      regions = std::map<const long, std::set<long>>();
      neighbours = std::map<const long, std::set<long>>();
      links = std::list<MapLink *>();
    }
    void addMap(Map * map, Tile * hole);
    void addMapLink(MapLink * link);
    void addToNeighbours(long map_id1, long map_id2);
    std::set<long> getRegion(long map_id);
    std::set<long> getNeighbours(long map_id);
    Map * getMap(long map_id);
    Map * getMap(std::string name);
    std::list<Map *> getMaps();
    MapLink * getMapLink(int x, int y, long mapId);
    std::list<Character *> getCharacters();
    Character * getCharacter(long id);
    Furniture * getFurniture(long id);
    Map * getMap(int x, int y, int z);
    Map * getMap(float x, float y, float z);
    Tile * getTile(int x, int y, int z);
    Tile * getTile(float x, float y, float z);
    int getLight(int x, int y, int z);
    int getLight(float x, float y, float z);
    float setPathToTarget(long map_id, float x, float y, Target * target);
    float distance(long map_id, float x, float y, Target * target);
  private:
    std::map<const long, Map *> maps;
    std::map<const long, std::set<long>> regions;
    std::map<const long, std::set<long>> neighbours;
    std::list<MapLink *> links;
};

#endif // _WORLD_H_
