#ifndef _TILES_BLOCK_H_
#define _TILES_BLOCK_H_

#include <algorithm>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <string>

#include "data/Character.h"
#include "data/Database.h"
#include "data/Projectile.h"
#include "data/Block.h"
#include "data/World.h"

#include "data/furnitures/Furniture.h"
#include "data/furnitures/BasicFurniture.h"
#include "data/furnitures/ContainerFurniture.h"
#include "data/furnitures/CraftingFurniture.h"
#include "data/furnitures/LinkedFurniture.h"
#include "data/furnitures/SkillFurniture.h"
#include "data/furnitures/SwitchFurniture.h"

#include "util/MathUtil.h"

#include "Values.h"

class BlocksChunk {

  struct LightenedBlock {
    Block * raw_block;
    int32_t lightening;
  };

  public:
    const MathUtil::Vector3i id;
    BlocksChunk(const MathUtil::Vector3i id, World * world):id(id) {
      MathUtil::Vector3i coord;
      for(coord.z = id.z; coord.z < id.z + CHUNK_SIZE; coord.z++) {
        for(coord.y = id.y; coord.y < id.y + CHUNK_SIZE; coord.y++) {
          for(coord.x = id.x; coord.x < id.x + CHUNK_SIZE; coord.x++) {
            Block * raw_block = nullptr; // world->getBlock(x, y, z);
            int32_t lightening = LIGHTENING_DARK;
            LightenedBlock * block = new LightenedBlock();
            block->raw_block = raw_block;
            block->lightening = lightening;
            if(block->raw_block != nullptr) {
              blocks.insert(std::make_pair(coord, block));
            }
          }
        }
      }
    }
    Block * getBlock(MathUtil::Vector3i coord);
    int32_t getLightening(MathUtil::Vector3i coord);
    void setBlock(MathUtil::Vector3i coord, Block * block, int32_t lightening);
    std::map<const MathUtil::Vector3i, Block *> getBlocks();
    std::list<Character *> getCharacters();
    std::list<Furniture *> getFurnitures();
    static MathUtil::Vector3i getChunkId(MathUtil::Vector3 ori);
    static MathUtil::Vector3i getChunkId(MathUtil::Vector3i ori);
    void addCharacter(Character * character);
    void addFurniture(Furniture * furniture);
    bool removeCharacter(Character * character);
  private:
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Furniture *> furnitures;
    std::list<Loot *> loots;
    std::map<const MathUtil::Vector3i, LightenedBlock *> blocks;
};

#endif // _TILES_BLOCK_H_