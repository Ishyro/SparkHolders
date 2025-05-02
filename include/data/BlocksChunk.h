#ifndef _BLOCKS_CHUNK_H_
#define _BLOCKS_CHUNK_H_

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
  public:
    const std::string name;
    const int32_t lightening;
    const bool instanciated;
    BlocksChunk(const std::string name, const int32_t lightening) : name(name), lightening(lightening), instanciated(false) {}
    BlocksChunk(BlocksChunk * chunk, const std::string name, const int64_t rotation, Database * database):
      name(name),
      lightening(chunk->lightening),
      furnitures(chunk->furnitures),
      instanciated(false)
    {
      for(std::pair<MathUtil::Vector3i, Block *> pair : chunk->blocks) {
        MathUtil::Vector3i coord = pair.first;
        switch(rotation) {
          case 90:
            coord.x = CHUNK_SIZE - 1 - pair.first.y;
            coord.y = pair.first.x;
            break;
          case 180:
            coord.x = CHUNK_SIZE - 1 - pair.first.x;
            coord.y = CHUNK_SIZE - 1 - pair.first.y;
            break;
          case 270:
            coord.x = pair.first.y;
            coord.y = CHUNK_SIZE - 1 - pair.first.x;
            break;
          default: ;
        }
        Block * block = pair.second;
        // oriented block
        if(block->name.find('#') != std::string::npos) {
          int32_t orientation_z = rotation + block->orientation_z;
          orientation_z = orientation_z >= 360.F ? orientation_z - 360.F : orientation_z;
          switch(orientation_z) {
            case 0:
              block = (Block *) database->getBlock(block->name.substr(0, block->name.find('#')) + "#EAST");
              break;
            case 90:
              block = (Block *) database->getBlock(block->name.substr(0, block->name.find('#')) + "#NORTH");
              break;
            case 180:
              block = (Block *) database->getBlock(block->name.substr(0, block->name.find('#')) + "#WEST");
              break;
            case 270:
              block = (Block *) database->getBlock(block->name.substr(0, block->name.find('#')) + "#SOUTH");
              break;
          }
        }
        blocks.insert(std::make_pair(coord, block));
      }
    }
    BlocksChunk(BlocksChunk * chunk):
      name(chunk->name),
      lightening(chunk->lightening),
      instanciated(true),
      blocks(chunk->blocks)
    {}
    Block * getBlock(MathUtil::Vector3i coord);
    void setBlock(MathUtil::Vector3i coord, Block * block);
    std::map<const MathUtil::Vector3i, Block *> getBlocks(MathUtil::Vector3i offset);
    std::map<const MathUtil::Vector3i, Block *> getDiffs(MathUtil::Vector3i offset);
    std::list<Furniture *> getFurnitures();
    static MathUtil::Vector3i getChunkId(MathUtil::Vector3 ori);
    static MathUtil::Vector3i getChunkId(MathUtil::Vector3i ori);
    void addFurniture(Furniture * furniture);
  private:
    std::list<Furniture *> furnitures;
    std::array<int32_t, 6> sides;
    std::map<const MathUtil::Vector3i, Block *> blocks;
    std::map<const MathUtil::Vector3i, Block *> diffs;
};

#endif // _BLOCKS_CHUNK_H_