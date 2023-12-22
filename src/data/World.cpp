#include "data/Character.h"
#include "data/Projectile.h"
#include "data/Block.h"
#include "data/Map.h"

#include "data/items/Item.h"

#include "data/World.h"

#include "data/BlocksChunk.h"
#include "data/Region.h"

#include <iostream>

void World::addMap(Map * map) {
  MapUtil::Vector3i coord = MapUtil::Vector3i();
  for(coord.z = map->offsetZ; coord.z < map->offsetZ + map->sizeZ; coord.z++) {
    for(coord.y = map->offsetY; coord.y < map->offsetY + map->sizeY; coord.y++) {
      for(coord.x = map->offsetX; coord.x < map->offsetX + map->sizeX; coord.x++) {
        Block * block = map->getBlock(coord);
        setBlock(coord, block);
      }
    }
  }
  for(Furniture * furniture : map->getFurnitures()) {
    getChunk(furniture->getCoord())->addFurniture(furniture);
  }
}

void World::setBlock(MapUtil::Vector3i coord, Block * block) {
  getChunk(coord)->setBlock(coord, block);
}

void World::addMapLink(MapLink * link) {
  links.push_back(link);
}

Map * World::getMap(long map_id) { return maps.at(map_id); }

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

MapLink * World::getMapLink(int x, int y, long mapId) {
  for(MapLink * link : links) {
    if((link->map1->id == mapId && link->x1 == x && link->y1 == y) ||
      (link->map2->id == mapId && link->x2 == x && link->y2 == y)) {
        return link;
    }
  }
  return nullptr;
}

std::list<Character *> World::getCharacters() { return characters; }

Character * World::getCharacter(long id) {
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

Furniture * World::getFurniture(long id) {
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

Block * World::getBlock(MapUtil::Vector3i coord) {
  return getChunk(coord)->getBlock(coord);
}

BlocksChunk * World::getChunk(MapUtil::Vector3 ori) {
  MapUtil::Vector3i coord = BlocksChunk::getChunkId(ori);
  std::map<const MapUtil::Vector3i, BlocksChunk *>::iterator it = chunks.find(coord);
  if(it != chunks.end()) {
    return it->second;
  }
  else {
    BlocksChunk * chunk = new BlocksChunk(coord, this);
    chunks.insert(std::make_pair(coord, chunk));
    return chunk;
  }
}

BlocksChunk * World::getChunk(MapUtil::Vector3i ori) {
  MapUtil::Vector3i coord = BlocksChunk::getChunkId(ori);
  std::map<const MapUtil::Vector3i, BlocksChunk *>::iterator it = chunks.find(coord);
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

void World::changeRegion(Character * character) {
  Region * old_region = character->getRegion();
  if(old_region != nullptr && old_region->removeCharacter(character)) {
    regions.erase(old_region->id);
  }
  MapUtil::Vector3i coord = BlocksChunk::getChunkId(character->getCoord());
  std::map<const MapUtil::Vector3i, Region *>::iterator it = regions.find(coord);
  if(it != regions.end()) {
    it->second->addCharacter(character);
  }
  else {
    std::array<BlocksChunk *, 27> result;
    MapUtil::Vector3i current;
    int n = 0;
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

/*
int World::getLight(int x, int y, int z) {
  for (auto pair : maps) {
    if(pair.second->offsetZ == z) {
      Block * block = pair.second->getBlock(x, y);
      if(block != nullptr && block->name != "TXT_VOID") {
        return pair.second->getLight(x, y);
      }
    }
  }
  return 0;
}
int World::getLight(float x, float y, float z) {
  for (auto pair : maps) {
    if(pair.second->offsetZ == z) {
      Block * block = pair.second->getBlock(x, y);
      if(block != nullptr && block->name != "TXT_VOID") {
        return pair.second->getLight(x, y);
      }
    }
  }
  return 0;
}

*/

float World::setPathToTarget(Region * region, float x, float y, Target * target) {
  float target_x;
  float target_y;
  switch(target->type) {
    case TARGET_TILE:
      target_x = target->x + 0.5F;
      target_y = target->y + 0.5F;
      break;
    case TARGET_COORDINATES:
      target_x = target->x;
      target_y = target->y;
      break;
    case TARGET_CHARACTER: {
      Character * other = getCharacter(target->id);
      target_x = other->getCoord().x;
      target_y = other->getCoord().y;
      break;
    }
    default: ;
  }
  return MapUtil::getOrientationToTarget(x, y, target_x, target_y);
}

float World::distance(long map_id, float x, float y, Target * target) {
  return 0.F;
}
