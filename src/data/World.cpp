#include "data/Character.h"
#include "data/Projectile.h"
#include "data/Block.h"

#include "data/items/Item.h"

#include "data/BlocksChunk.h"
#include "data/Region.h"
#include "data/Biome.h"

#include "data/World.h"

void World::setBiome(Biome * biome) {
  biomes.push_back(biome);
}

Biome * World::getBiome(MathUtil::Vector3i coord) {
  for(Biome * biome : biomes) {
    if(biome->origin <= coord && coord <= (biome->end)) {
      return biome;
    }
  }
  return nullptr;
}

void World::setChunk(MathUtil::Vector3i coord, BlocksChunk * chunk) {
  MathUtil::Vector3i safe_coord = BlocksChunk::getChunkId(coord);
  std::map<const MathUtil::Vector3i, BlocksChunk *>::iterator it = chunks.find(safe_coord);
  if(it != chunks.end()) {
    chunks.erase(it);
  }
  chunks.insert(std::make_pair(safe_coord, chunk));
}

void World::setBlock(MathUtil::Vector3i coord, Block * block) {
  MathUtil::Vector3i chunk_id = BlocksChunk::getChunkId(coord);
  std::map<const MathUtil::Vector3i, BlocksChunk *>::iterator it = chunks.find(chunk_id);
  if(it != chunks.end()) {
    BlocksChunk * chunk = it->second;
    if(!it->second->instanciated) {
      chunk = new BlocksChunk(chunk);
      chunks.erase(it);
      chunks.insert(std::make_pair(chunk_id, chunk));
    }
    chunk->setBlock(coord, block);
  }
}

void World::generateWorld() {
  for(Biome * biome : biomes) {
    MathUtil::Vector3i chunk_id;
    for(chunk_id.z = biome->end.z; chunk_id.z >= biome->origin.z; chunk_id.z -= CHUNK_SIZE) {
      for(chunk_id.y = biome->end.y; chunk_id.y >= biome->origin.y; chunk_id.y -= CHUNK_SIZE) {
        for(chunk_id.x = biome->origin.x; chunk_id.x <= biome->end.x; chunk_id.x += CHUNK_SIZE) {
          std::map<const MathUtil::Vector3i, BlocksChunk *>::iterator it = chunks.find(chunk_id);
          if(it == chunks.end()) {
            std::array<BlocksChunk *, 6> neighbors = std::array<BlocksChunk *, 6>();
            neighbors.fill(nullptr);
            // EAST
            it = chunks.find(chunk_id + MathUtil::Vector3i(CHUNK_SIZE, 0, 0));
            if(it != chunks.end()) {
              neighbors[EAST] = it->second;
            }
            // NORTH
            it = chunks.find(chunk_id + MathUtil::Vector3i(0, CHUNK_SIZE, 0));
            if(it != chunks.end()) {
              neighbors[NORTH] = it->second;
            }
            // WEST
            it = chunks.find(chunk_id + MathUtil::Vector3i(-CHUNK_SIZE, 0, 0));
            if(it != chunks.end()) {
              neighbors[WEST] = it->second;
            }
            // SOUTH
            it = chunks.find(chunk_id + MathUtil::Vector3i(0, -CHUNK_SIZE, 0));
            if(it != chunks.end()) {
              neighbors[SOUTH] = it->second;
            }
            // UP
            it = chunks.find(chunk_id + MathUtil::Vector3i(0, 0, CHUNK_SIZE));
            if(it != chunks.end()) {
              neighbors[UP] = it->second;
            }
            // DOWN
            it = chunks.find(chunk_id + MathUtil::Vector3i(0, 0, -CHUNK_SIZE));
            if(it != chunks.end()) {
              neighbors[DOWN] = it->second;
            }
            BlocksChunk * chunk = biome->getChunk(neighbors);
            if(chunk != nullptr) {
              for(Furniture * furniture : chunk->getFurnitures()) {
                switch(furniture->type) {
                  case FURNITURE_BASIC:
                    addFurniture(new BasicFurniture( (BasicFurniture *) furniture, chunk_id, 0));
                    break;
                  case FURNITURE_CONTAINER:
                    addFurniture(new ContainerFurniture( (ContainerFurniture *) furniture, chunk_id, 0));
                    break;
                  case FURNITURE_CRAFTING:
                    addFurniture(new CraftingFurniture( (CraftingFurniture *) furniture, chunk_id, 0));
                    break;
                  case FURNITURE_LINKED:
                    addFurniture(new LinkedFurniture( (LinkedFurniture *) furniture, chunk_id, 0));
                    break;
                  case FURNITURE_SKILL:
                    addFurniture(new SkillFurniture( (SkillFurniture *) furniture, chunk_id, 0));
                    break;
                  case FURNITURE_SWITCH:
                    addFurniture(new SwitchFurniture( (SwitchFurniture *) furniture, chunk_id, 0));
                    break;
                }
              }
              chunks.insert(std::make_pair(chunk_id, chunk));
            }
          }
        }
      }
    }
  }
}

std::list<Character *> World::getCharacters() { 
  std::list<Character *> result = std::list<Character *>();
  for(std::pair<MathUtil::Vector3i, std::list<Character *>> pair : characters) {
    for(Character * character : pair.second) {
      result.push_back(character);
    }
  }
  return result;
}

Character * World::getCharacter(int64_t id) {
  if(id != 0) {
    for(Character * character : getCharacters()) {
      if(character->id == id) {
        return character;
      }
    }
  }
  return nullptr;
}

std::list<Character *> World::computeTarget(Character * origin, MathUtil::Target * target) {
  std::list<Character *> targets = std::list<Character *>();
  if(!targets.empty()) {
    delete target;
    target = new MathUtil::Target();
    target->type = TARGET_MULTIPLE;
    MathUtil::Target * iter = target;
    for(Character * character : targets) {
      MathUtil::Target * next;
      next = new MathUtil::Target();
      next->type = TARGET_CHARACTER;
      next->character = character;
      next->next = nullptr;
      iter->next = next;
      iter = next;
    }
  }
  return targets;
}

Furniture * World::getFurniture(MathUtil::Vector3i furniture_coord) {
  MathUtil::Vector3i chunk_coord = BlocksChunk::getChunkId(furniture_coord);
  for(Furniture * furniture : getFurnitures(chunk_coord)) {
    if(furniture->getCoord() == furniture_coord) {
      return furniture;
    }
  }
  return nullptr;
}

std::list<Furniture *> World::getFurnituresInChunk(MathUtil::Vector3i furniture_coord) {
  MathUtil::Vector3i chunk_coord = BlocksChunk::getChunkId(furniture_coord);
  std::map<const MathUtil::Vector3i, std::list<Furniture *>>::iterator it = furnitures.find(chunk_coord);
  if(it != furnitures.end()) {
    return it->second;
  }
  return std::list<Furniture *>();
}

Block * World::getBlock(MathUtil::Vector3i coord) {
  return getChunk(coord)->getBlock(coord);
}

int32_t World::getLightening(MathUtil::Vector3i coord) {
  return getChunk(coord)->lightening;
}

BlocksChunk * World::getChunk(MathUtil::Vector3 ori) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(ori);
  std::map<const MathUtil::Vector3i, BlocksChunk *>::iterator it = chunks.find(coord);
  if(it != chunks.end()) {
    return it->second;
  }
  else {
   return nullptr;
  }
}

BlocksChunk * World::getChunk(MathUtil::Vector3i ori) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(ori);
  std::map<const MathUtil::Vector3i, BlocksChunk *>::iterator it = chunks.find(coord);
  if(it != chunks.end()) {
    return it->second;
  }
  else {
    return nullptr;
  }
}

void World::addCharacter(Character * character) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(character->getCoord());
  std::map<const MathUtil::Vector3i, std::list<Character *>>::iterator it = characters.find(coord);
  if(it == characters.end()) {
    characters.insert(std::make_pair(coord, std::list<Character *>()));
  }
  characters.at(coord).push_back(character);
  changeRegion(character);
}

void World::eraseCharacter(Character * character) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(character->getCoord());
  characters.at(coord).remove(character);
  if(characters.at(coord).empty()) {
    characters.erase(coord);
  }
  delete character;
}

void World::addShield(Character * character, Shield * shield) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(character->getCoord());
  std::map<const MathUtil::Vector3i, std::list<Shield *>>::iterator it = shields.find(coord);
  if(it == shields.end()) {
    shields.insert(std::make_pair(coord, std::list<Shield *>()));
  }
  shields.at(coord).push_back(shield);
}

void World::eraseShield(Character * character, Shield * shield) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(character->getCoord());
  shields.at(coord).remove(shield);
  if(shields.at(coord).empty()) {
    shields.erase(coord);
  }
  delete shield;
}

void World::addFurniture(Furniture * furniture) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(furniture->getCoord());
  std::map<const MathUtil::Vector3i, std::list<Furniture *>>::iterator it = furnitures.find(coord);
  if(it == furnitures.end()) {
    furnitures.insert(std::make_pair(coord, std::list<Furniture *>()));
  }
  furnitures.at(coord).push_back(furniture);
}

void World::eraseFurniture(Furniture * furniture) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(furniture->getCoord());
  furnitures.at(coord).remove(furniture);
  if(furnitures.at(coord).empty()) {
    furnitures.erase(coord);
  }
  delete furniture;
}

void World::checkRegion(Character * character, MathUtil::Vector3 ori, MathUtil::Vector3 dest) {
  if(floor(ori.x / CHUNK_SIZE) != floor(dest.x / CHUNK_SIZE)
    || floor(ori.y / CHUNK_SIZE) != floor(dest.y / CHUNK_SIZE)
    || floor(ori.z / CHUNK_SIZE) != floor(dest.z / CHUNK_SIZE)) {
    changeRegion(character);
  }
}

void World::changeRegion(Character * character) {
  Region * old_region = character->getRegion();
  if(old_region != nullptr) {
    characters.at(old_region->id).remove(character);
    if(characters.at(old_region->id).empty()) {
      characters.erase(old_region->id);
      regions.erase(old_region->id);
    }
  }
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(character->getCoord());
  std::map<const MathUtil::Vector3i, Region *>::iterator it = regions.find(coord);
  if(it != regions.end()) {
    character->setRegion(it->second);
  }
  else {
    std::array<BlocksChunk *, 27> result;
    MathUtil::Vector3i current;
    int32_t n = 0;
    for(current.z = coord.z - CHUNK_SIZE; current.z <= coord.z + CHUNK_SIZE; current.z += CHUNK_SIZE) {
      for(current.y = coord.y - CHUNK_SIZE; current.y <= coord.y + CHUNK_SIZE; current.y += CHUNK_SIZE) {
        for(current.x = coord.x - CHUNK_SIZE; current.x <= coord.x + CHUNK_SIZE; current.x += CHUNK_SIZE) {
          result[n++] = getChunk(current);
        }
      }
    }
    Region * region = new Region(coord, result);
    character->setRegion(region);
    std::map<const MathUtil::Vector3i, std::list<Character *>>::iterator it = characters.find(coord);
    if(it == characters.end()) {
      characters.insert(std::make_pair(coord, std::list<Character *>()));
    }
    characters.at(coord).push_back(character);
    regions.insert(std::make_pair(coord, region));
  }
}

float World::distance(int64_t map_id, float x, float y, MathUtil::Target * target) {
  return 0.F;
}

std::list<Character *> World::getCharacters(MathUtil::Vector3i region_id) { 
  std::list<Character *> result = std::list<Character *>();
  MathUtil::Vector3i current;
  for(current.z = region_id.z - CHUNK_SIZE; current.z <= region_id.z + CHUNK_SIZE; current.z += CHUNK_SIZE) {
    for(current.y = region_id.y - CHUNK_SIZE; current.y <= region_id.y + CHUNK_SIZE; current.y += CHUNK_SIZE) {
      for(current.x = region_id.x - CHUNK_SIZE; current.x <= region_id.x + CHUNK_SIZE; current.x += CHUNK_SIZE) {
        std::map<const MathUtil::Vector3i, std::list<Character *>>::iterator it = characters.find(current);
        if(it != characters.end()) {
          for(Character * character : it->second) {
            result.push_back(character);
          }
        }
      }
    }
  }
  return result;
}

std::list<Projectile *> World::getProjectiles(MathUtil::Vector3i region_id) { 
  std::list<Projectile *> result = std::list<Projectile *>();
  MathUtil::Vector3i current;
  for(current.z = region_id.z - CHUNK_SIZE; current.z <= region_id.z + CHUNK_SIZE; current.z += CHUNK_SIZE) {
    for(current.y = region_id.y - CHUNK_SIZE; current.y <= region_id.y + CHUNK_SIZE; current.y += CHUNK_SIZE) {
      for(current.x = region_id.x - CHUNK_SIZE; current.x <= region_id.x + CHUNK_SIZE; current.x += CHUNK_SIZE) {
        std::map<const MathUtil::Vector3i, std::list<Projectile *>>::iterator it = projectiles.find(current);
        if(it != projectiles.end()) {
          for(Projectile * projectile : it->second) {
            result.push_back(projectile);
          }
        }
      }
    }
  }
  return result;
}

std::list<Furniture *> World::getFurnitures(MathUtil::Vector3i region_id, int64_t sizeZ = 1, int64_t radius = 1) { 
  std::list<Furniture *> result = std::list<Furniture *>();
  MathUtil::Vector3i current;
  for(current.z = region_id.z - sizeZ * CHUNK_SIZE; current.z <= region_id.z + sizeZ * CHUNK_SIZE; current.z += CHUNK_SIZE) {
    for(current.y = region_id.y - radius * CHUNK_SIZE; current.y <= region_id.y + radius * CHUNK_SIZE; current.y += CHUNK_SIZE) {
      for(current.x = region_id.x - radius * CHUNK_SIZE; current.x <= region_id.x + radius * CHUNK_SIZE; current.x += CHUNK_SIZE) {
        std::map<const MathUtil::Vector3i, std::list<Furniture *>>::iterator it = furnitures.find(current);
        if(it != furnitures.end()) {
          for(Furniture * furniture : it->second) {
            result.push_back(furniture);
          }
        }
      }
    }
  }
  return result;
}

std::list<Shield *> World::getShields(MathUtil::Vector3i region_id) { 
  std::list<Shield *> result = std::list<Shield *>();
  MathUtil::Vector3i current;
  for(current.z = region_id.z - CHUNK_SIZE; current.z <= region_id.z + CHUNK_SIZE; current.z += CHUNK_SIZE) {
    for(current.y = region_id.y - CHUNK_SIZE; current.y <= region_id.y + CHUNK_SIZE; current.y += CHUNK_SIZE) {
      for(current.x = region_id.x - CHUNK_SIZE; current.x <= region_id.x + CHUNK_SIZE; current.x += CHUNK_SIZE) {
        std::map<const MathUtil::Vector3i, std::list<Shield *>>::iterator it = shields.find(current);
        if(it != shields.end()) {
          for(Shield * shield : it->second) {
            result.push_back(shield);
          }
        }
      }
    }
  }
  return result;
}

std::list<Loot *> World::getLoots(MathUtil::Vector3i region_id) { 
  std::list<Loot *> result = std::list<Loot *>();
  MathUtil::Vector3i current;
  for(current.z = region_id.z - CHUNK_SIZE; current.z <= region_id.z + CHUNK_SIZE; current.z += CHUNK_SIZE) {
    for(current.y = region_id.y - CHUNK_SIZE; current.y <= region_id.y + CHUNK_SIZE; current.y += CHUNK_SIZE) {
      for(current.x = region_id.x - CHUNK_SIZE; current.x <= region_id.x + CHUNK_SIZE; current.x += CHUNK_SIZE) {
        std::map<const MathUtil::Vector3i, std::list<Loot *>>::iterator it = loots.find(current);
        if(it != loots.end()) {
          for(Loot * loot : it->second) {
            result.push_back(loot);
          }
        }
      }
    }
  }
  return result;
}
