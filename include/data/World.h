#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <string>
#include <map>
#include <set>
#include <array>

#include "util/MathUtil.h"

struct Loot {
  int32_t type;
  float x;
  float y;
  float size;
  std::list<Item *> items;
};

class World {
  public:
    const std::string name;
    World(std::string name):name(name) {
      chunks = std::map<const MathUtil::Vector3i, BlocksChunk *>();
      regions = std::map<const MathUtil::Vector3i, Region *>();
    }
    void setBiome(Biome * biome);
    Biome * getBiome(MathUtil::Vector3i coord);
    void setChunk(MathUtil::Vector3i coord, BlocksChunk * chunk);
    void setBlock(MathUtil::Vector3i coord, Block * block);
    void generateWorld();
    std::list<Character *> getCharacters();
    Character * getCharacter(int64_t id);
    std::list<Character *> computeTarget(Character * origin, MathUtil::Target * target);
    Furniture * getFurniture(MathUtil::Vector3i furniture_coord);
    Block * getBlock(MathUtil::Vector3i coord);
    int32_t getLightening(MathUtil::Vector3i coord);
    BlocksChunk * getChunk(MathUtil::Vector3 coord);
    BlocksChunk * getChunk(MathUtil::Vector3i ori);
    void addCharacter(Character * character);
    void eraseCharacter(Character * character);
    void addShield(Character * character, Shield * shield);
    void eraseShield(Character * character, Shield * shield);
    void addFurniture(Furniture * furniture);
    void eraseFurniture(Furniture * furniture);
    void checkRegion(Character * character, MathUtil::Vector3 ori, MathUtil::Vector3 dest);
    void changeRegion(Character * character);
    float distance(int64_t map_id, float x, float y, MathUtil::Target * target);
    std::list<Character *> getCharacters(MathUtil::Vector3i chunk_id);
    std::list<Projectile *> getProjectiles(MathUtil::Vector3i chunk_id);
    std::list<Furniture *> getFurnitures(MathUtil::Vector3i chunk_id, int64_t sizeZ = 1, int64_t radius = 1);
    std::list<Shield *> getShields(MathUtil::Vector3i chunk_id);
    std::list<Loot *> getLoots(MathUtil::Vector3i chunk_id);
  private:
    std::list<Biome *> biomes;
    std::map<const MathUtil::Vector3i, BlocksChunk *> chunks;
    std::map<const MathUtil::Vector3i, Region *> regions;
    std::map<const MathUtil::Vector3i, std::list<Character *>> characters;
    std::map<const MathUtil::Vector3i, std::list<Projectile *>> projectiles;
    std::map<const MathUtil::Vector3i, std::list<Furniture *>> furnitures;
    std::map<const MathUtil::Vector3i, std::list<Shield *>> shields;
    std::map<const MathUtil::Vector3i, std::list<Loot *>> loots;
};

#endif // _WORLD_H_
