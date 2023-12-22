#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <string>
#include <map>
#include <set>
#include <array>

#include "util/MapUtil.h"

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
      chunks = std::map<const MapUtil::Vector3i, BlocksChunk *>();
      regions = std::map<const MapUtil::Vector3i, Region *>();
      links = std::list<MapLink *>();
    }
    void addMap(Map * map);
    void setBlock(MapUtil::Vector3i coord, Block * block);
    void addMapLink(MapLink * link);
    Map * getMap(long map_id);
    Map * getMap(std::string name);
    std::list<Map *> getMaps();
    MapLink * getMapLink(int x, int y, long mapId);
    std::list<Character *> getCharacters();
    Character * getCharacter(long id);
    Furniture * getFurniture(long id);
    Map * getMap(int x, int y, int z);
    Map * getMap(float x, float y, float z);
    Block * getBlock(MapUtil::Vector3i coord);
    BlocksChunk * getChunk(MapUtil::Vector3 coord);
    BlocksChunk * getChunk(MapUtil::Vector3i ori);
    void addCharacter(Character * character);
    void changeRegion(Character * character);
    int getLight(int x, int y, int z);
    int getLight(float x, float y, float z);
    float setPathToTarget(Region * region, float x, float y, Target * target);
    float distance(long map_id, float x, float y, Target * target);
  private:
    std::map<const long, Map *> maps;
    std::map<const MapUtil::Vector3i, BlocksChunk *> chunks;
    std::map<const MapUtil::Vector3i, Region *> regions;
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Furniture *> furnitures;
    std::list<Loot *> loots;
    std::list<MapLink *> links;
};

#endif // _WORLD_H_
