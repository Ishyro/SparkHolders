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

struct ChunkSide {
  int32_t type;
  int32_t subtype;
  int32_t start_x;
  int32_t start_y;
  int32_t end_x;
  int32_t end_y;
  // mirrored so x are inverted
  bool operator == (const ChunkSide& c) const { return type == c.type && start_x == c.start_x && start_y == c.start_y && end_x == c.end_x && end_y == c.end_y; }
};

class BlocksChunk {
  public:
    const std::string name;
    const int32_t lightening;
    const float weight;
    const bool instanciated;
    BlocksChunk(const std::string name, const int32_t lightening, const float weight, std::array<ChunkSide, 6> sides) : name(name), lightening(lightening), weight(weight), sides(sides), instanciated(false) {}
    BlocksChunk(BlocksChunk * chunk, const std::string name, const int64_t rotation, Database * database):
      name(name),
      lightening(chunk->lightening),
      weight(chunk->weight),
      furnitures(chunk->furnitures),
      instanciated(false)
    {
      sides[UP] = chunk->sides[UP];
      sides[DOWN] = chunk->sides[DOWN];
      switch(rotation) {
        case 0:
          sides[EAST] = chunk->sides[EAST];
          sides[NORTH] = chunk->sides[NORTH];
          sides[WEST] = chunk->sides[WEST];
          sides[SOUTH] = chunk->sides[SOUTH];
          break;
        case 90:
          sides[EAST] = chunk->sides[SOUTH];
          sides[NORTH] = chunk->sides[EAST];
          sides[WEST] = chunk->sides[NORTH];
          sides[SOUTH] = chunk->sides[WEST];
          break;
        case 180:
          sides[EAST] = chunk->sides[WEST];
          sides[NORTH] = chunk->sides[SOUTH];
          sides[WEST] = chunk->sides[EAST];
          sides[SOUTH] = chunk->sides[NORTH];
          break;
        case 270:
          sides[EAST] = chunk->sides[NORTH];
          sides[NORTH] = chunk->sides[WEST];
          sides[WEST] = chunk->sides[SOUTH];
          sides[SOUTH] = chunk->sides[EAST];
          break;
        default: ;
      }
      for(std::pair<MathUtil::Vector3i, Block *> pair : chunk->inner_blocks) {
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
        inner_blocks.insert(std::make_pair(coord, block));
      }
      for(std::pair<MathUtil::Vector3i, Block *> pair : chunk->outer_blocks) {
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
        outer_blocks.insert(std::make_pair(coord, block));
      }
    }
    BlocksChunk(BlocksChunk * chunk):
      name(chunk->name),
      lightening(chunk->lightening),
      weight(chunk->weight),
      sides(chunk->sides),
      instanciated(true),
      inner_blocks(chunk->inner_blocks),
      outer_blocks(chunk->outer_blocks)
    {}
    Block * getBlock(MathUtil::Vector3i coord);
    void setBlock(MathUtil::Vector3i coord, Block * block);
    std::map<const MathUtil::Vector3i, Block *> getOuterBlocks(MathUtil::Vector3i offset);
    std::map<const MathUtil::Vector3i, Block *> getBlocks(MathUtil::Vector3i offset);
    std::map<const MathUtil::Vector3i, Block *> getDiffs(MathUtil::Vector3i offset);
    void computeOuterBlocks();
    ChunkSide getSide(int32_t side);
    std::list<Furniture *> getFurnitures();
    static MathUtil::Vector3i getChunkId(MathUtil::Vector3 ori);
    static MathUtil::Vector3i getChunkId(MathUtil::Vector3i ori);
    void addFurniture(Furniture * furniture);
  private:
    std::list<Furniture *> furnitures;
    std::array<ChunkSide, 6> sides;
    std::map<const MathUtil::Vector3i, Block *> inner_blocks;
    std::map<const MathUtil::Vector3i, Block *> outer_blocks;
    std::map<const MathUtil::Vector3i, Block *> diffs;
};

#endif // _BLOCKS_CHUNK_H_