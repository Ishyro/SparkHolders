#ifndef _REGION_H_
#define _REGION_H_

#include <algorithm>
#include <list>
#include <array>
#include <set>
#include <map>
#include <vector>
#include <string>

#include "data/Character.h"
#include "data/BlocksChunk.h"

#include "data/furnitures/Furniture.h"
#include "data/furnitures/BasicFurniture.h"
#include "data/furnitures/ContainerFurniture.h"
#include "data/furnitures/CraftingFurniture.h"
#include "data/furnitures/LinkedFurniture.h"
#include "data/furnitures/SkillFurniture.h"
#include "data/furnitures/SwitchFurniture.h"

#include "util/MapUtil.h"

#include <iostream>

class Region {
  public:
    const MapUtil::Vector3i id;
    Region(MapUtil::Vector3i id, std::array<BlocksChunk *, 27> chunks):id(id),chunks(chunks) {}
    void addCharacter(Character * character);
    bool removeCharacter(Character * character);
    Block * getBlock(MapUtil::Vector3 coord);
    Block * getBlock(MapUtil::Vector3i coord);
    std::map<const MapUtil::Vector3i, Block *> getBlocks();
    float getMoveCost(Character * c, MapUtil::Vector3 ori, MapUtil::Vector3 dest);
    bool tryMove(Character * c, MapUtil::Vector3 dest);
    float move(Character * c, float orientation, MapUtil::Vector3 dest, float ap, World * world);
    bool canSee(Character * watcher, Character * target);
    std::list<Character *> getCharacters();
    std::list<Character *> getCharacters(Character * character);
    std::list<Furniture *> getFurnitures();
    std::list<Furniture *> getFurnitures(Character * character);
    Furniture * getFurniture(MapUtil::Vector3i coord);
  private:
    std::array<BlocksChunk *, 27> chunks;
    std::list<Character *> characters;
};

#endif // _REGION_H_
