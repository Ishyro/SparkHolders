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

#include "util/MathUtil.h"

struct Attack {
  int32_t hit_order;
  MathUtil::Hitbox * hitbox;
  Character * owner;
  std::array<float, DAMAGE_TYPE_NUMBER> damages;
  std::list<Effect *> effects;
  float status_power;
  float pierce_power;
};

struct HitboxOwner {
  int32_t type;
  float distance;
  float angle;
  Character * owner;
};

namespace hitbox {
  bool left_to_right(const HitboxOwner * h1, const HitboxOwner * h2);
  bool right_to_left(const HitboxOwner * h1, const HitboxOwner * h2);
  bool front_to_back(const HitboxOwner * h1, const HitboxOwner * h2);
  bool back_to_front(const HitboxOwner * h1, const HitboxOwner * h2);
}

class Region {
  public:
    const MathUtil::Vector3i id;
    Region(MathUtil::Vector3i id, std::array<BlocksChunk *, 27> chunks):id(id),chunks(chunks) {}
    void addCharacter(Character * character);
    bool removeCharacter(Character * character);
    void addShield(Shield * shield);
    void removeShield(Shield * shield);
    Block * getBlock(MathUtil::Vector3 coord);
    Block * getBlock(MathUtil::Vector3i coord);
    std::map<const MathUtil::Vector3i, Block *> getBlocks();
    float getMoveCost(Character * c, MathUtil::Vector3 ori, MathUtil::Vector3 dest);
    bool tryMove(Character * c, MathUtil::Vector3 dest);
    void move(Character * character, World * world);
    void setSpeed(Character * character, MathUtil::Vector3 orientation);
    MathUtil::Vector3 getCoordsOnSlope(MathUtil::Vector3 dest, int32_t block_orientation);
    float getOrientationXOnSlope(float character_orientation, int32_t block_orientation);
    bool canSee(Character * watcher, Character * target);
    std::list<Character *> getCharacters();
    std::list<Character *> getCharacters(Character * character);
    std::list<Furniture *> getFurnitures();
    std::list<Furniture *> getFurnitures(Character * character);
    std::list<Shield *> getShields();
    Furniture * getFurniture(MathUtil::Vector3i coord);
    std::list<HitboxOwner *> sortHitboxes(Attack * origin);
  private:
    std::array<BlocksChunk *, 27> chunks;
};

#endif // _REGION_H_
