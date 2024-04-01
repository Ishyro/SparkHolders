#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <string>
#include <map>
#include <set>
#include <array>

#include "util/MathUtil.h"

typedef struct MapLink {
    int32_t x1;
    int32_t y1;
    int32_t z1;
    int32_t x2;
    int32_t y2;
    int32_t z2;
    int32_t type;
    Map * map1;
    Map * map2;
} MapLink;

class World {
  public:
    const std::string name;
    World(std::string name):name(name) {
      maps = std::map<const int64_t, Map *>();
      chunks = std::map<const MathUtil::Vector3i, BlocksChunk *>();
      regions = std::map<const MathUtil::Vector3i, Region *>();
      links = std::list<MapLink *>();
    }
    void addMap(Map * map);
    void setBlock(MathUtil::Vector3i coord, Block * block);
    void addMapLink(MapLink * link);
    Map * getMap(int64_t map_id);
    Map * getMap(std::string name);
    std::list<Map *> getMaps();
    MapLink * getMapLink(int32_t x, int32_t y, int64_t mapId);
    std::list<Character *> getCharacters();
    Character * getCharacter(int64_t id);
    Furniture * getFurniture(int64_t id);
    Map * getMap(int32_t x, int32_t y, int32_t z);
    Map * getMap(float x, float y, float z);
    Block * getBlock(MathUtil::Vector3i coord);
    BlocksChunk * getChunk(MathUtil::Vector3 coord);
    BlocksChunk * getChunk(MathUtil::Vector3i ori);
    void addCharacter(Character * character);
    void checkRegion(Character * character, MathUtil::Vector3 ori, MathUtil::Vector3 dest);
    void changeRegion(Character * character);
    int32_t getLight(int32_t x, int32_t y, int32_t z);
    int32_t getLight(float x, float y, float z);
    float setPathToTarget(Region * region, float x, float y, Target * target);
    float distance(int64_t map_id, float x, float y, Target * target);
  private:
    std::map<const int64_t, Map *> maps;
    std::map<const MathUtil::Vector3i, BlocksChunk *> chunks;
    std::map<const MathUtil::Vector3i, Region *> regions;
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Furniture *> furnitures;
    std::list<Loot *> loots;
    std::list<MapLink *> links;
};

#endif // _WORLD_H_
