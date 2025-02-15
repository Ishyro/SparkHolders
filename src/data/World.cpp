#include "data/Character.h"
#include "data/Projectile.h"
#include "data/Block.h"
#include "data/Map.h"

#include "data/items/Item.h"

#include "data/World.h"

#include "data/BlocksChunk.h"
#include "data/Region.h"

void World::addMap(Map * map) {
  int32_t lightening = map->lightening;
  MathUtil::Vector3i coord = MathUtil::Vector3i();
  for(coord.z = map->offsetZ; coord.z < map->offsetZ + map->sizeZ; coord.z++) {
    for(coord.y = map->offsetY; coord.y < map->offsetY + map->sizeY; coord.y++) {
      for(coord.x = map->offsetX; coord.x < map->offsetX + map->sizeX; coord.x++) {
        Block * block = map->getBlock(coord);
        setBlock(coord, block, lightening);
      }
    }
  }
  for(Furniture * furniture : map->getFurnitures()) {
    getChunk(furniture->getCoord())->addFurniture(furniture);
  }
}

void World::setBlock(MathUtil::Vector3i coord, Block * block, int32_t lightening) {
  getChunk(coord)->setBlock(coord, block, lightening);
}

void World::addMapLink(MapLink * link) {
  links.push_back(link);
}

Map * World::getMap(int64_t map_id) { return maps.at(map_id); }

Map * World::getMap(std::string name) {
    for(auto pair = maps.begin(); pair != maps.end(); pair++) {
      if(pair->second->name == name) {
        return pair->second;
      }
    }
    return nullptr;
}

std::list<Map *> World::getMaps() {
  std::list<Map *> to_return = std::list<Map *>();
  for(auto pair : maps) {
    to_return.push_back(pair.second);
  }
  return to_return;
}

MapLink * World::getMapLink(int32_t x, int32_t y, int64_t mapId) {
  for(MapLink * link : links) {
    if((link->map1->id == mapId && link->x1 == x && link->y1 == y) ||
      (link->map2->id == mapId && link->x2 == x && link->y2 == y)) {
        return link;
    }
  }
  return nullptr;
}

std::list<Character *> World::getCharacters() { return characters; }

Character * World::getCharacter(int64_t id) {
  if(id != 0) {
    for(auto pair : regions) {
      for(Character * character : pair.second->getCharacters()) {
        if(character->id == id) {
          return character;
        }
      }
    }
  }
  return nullptr;
}

std::list<Character *> World::computeTarget(Character * origin, Target * target) {
  std::list<Character *> targets = std::list<Character *>();
  if(!targets.empty()) {
    delete target;
    target = new Target();
    target->type = TARGET_MULTIPLE;
    Target * iter = target;
    for(Character * character : targets) {
      Target * next;
      next = new Target();
      next->type = TARGET_CHARACTER;
      next->character = character;
      next->next = nullptr;
      iter->next = next;
      iter = next;
    }
  }
  return targets;
}

Furniture * World::getFurniture(int64_t id) {
  if(id != 0) {
    for(auto pair : chunks) {
      for(Furniture * furniture : pair.second->getFurnitures()) {
        if(furniture->id == id) {
          return furniture;
        }
      }
    }
  }
  return nullptr;
}

Block * World::getBlock(MathUtil::Vector3i coord) {
  return getChunk(coord)->getBlock(coord);
}

int32_t World::getLightening(MathUtil::Vector3i coord) {
  MathUtil::Vector3i to_check = coord;
  int32_t result = getChunk(to_check)->getLightening(to_check);
  while( (result = getChunk(to_check)->getLightening(to_check)) == LIGHTENING_NO) {
    to_check.z--;
  }
  return result;
}

BlocksChunk * World::getChunk(MathUtil::Vector3 ori) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(ori);
  std::map<const MathUtil::Vector3i, BlocksChunk *>::iterator it = chunks.find(coord);
  if(it != chunks.end()) {
    return it->second;
  }
  else {
    BlocksChunk * chunk = new BlocksChunk(coord, this);
    chunks.insert(std::make_pair(coord, chunk));
    return chunk;
  }
}

BlocksChunk * World::getChunk(MathUtil::Vector3i ori) {
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(ori);
  std::map<const MathUtil::Vector3i, BlocksChunk *>::iterator it = chunks.find(coord);
  if(it != chunks.end()) {
    return it->second;
  }
  else {
    BlocksChunk * chunk = new BlocksChunk(coord, this);
    chunks.insert(std::make_pair(coord, chunk));
    return chunk;
  }
}

void World::addCharacter(Character * character) {
  characters.push_back(character);
  changeRegion(character);
}

void World::removeCharacter(Character * character) {
  characters.remove(character);
  character->getRegion()->removeCharacter(character);
  delete character;
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
  if(old_region != nullptr && old_region->removeCharacter(character)) {
    regions.erase(old_region->id);
  }
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(character->getCoord());
  std::map<const MathUtil::Vector3i, Region *>::iterator it = regions.find(coord);
  if(it != regions.end()) {
    it->second->addCharacter(character);
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
    region->addCharacter(character);
    regions.insert(std::make_pair(coord, region));
  }
}

float World::setPathToTarget(Region * region, float x, float y, Target * target) {
  float target_x;
  float target_y;
  switch(target->type) {
    case TARGET_BLOCK:
      target_x = target->coord.x + 0.5F;
      target_y = target->coord.y + 0.5F;
      break;
    case TARGET_COORDINATES:
      target_x = target->coord.x;
      target_y = target->coord.y;
      break;
    case TARGET_CHARACTER: {
      Character * other = target->character;
      target_x = other->getCoord().x;
      target_y = other->getCoord().y;
      break;
    }
    default: ;
  }
  return MathUtil::getOrientationToTarget(x, y, target_x, target_y);
}

float World::distance(int64_t map_id, float x, float y, Target * target) {
  return 0.F;
}
