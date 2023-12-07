#include "data/Region.h"

#include <iostream>

void Region::addCharacter(Character * character) {
  characters.push_back(character);
  character->setRegion(this);
}

bool Region::removeCharacter(Character * character) {
  characters.remove(character);
  return characters.empty();
}

Block * Region::getBlock(MapUtil::Vector3 coord) {
  return getBlock(MapUtil::makeVector3i(coord));
}

Block * Region::getBlock(MapUtil::Vector3i coord) {
  int a = (int) std::floor((coord.x - id.x) / CHUNK_SIZE) + 1;
  int b = (int) std::floor((coord.y - id.y) / CHUNK_SIZE) + 1;
  int c = (int) std::floor((coord.z - id.z) / CHUNK_SIZE) + 1;
  return chunks[a + b * 3 + c * 9]->getBlock(coord);
}

bool Region::canSee(Character * watcher, Character * target) {
  if(target->isInvisible() || target->isEtheral()) {
    return false;
  }
  MapUtil::Vector3 watcher_head = watcher->getCoord();
  watcher_head.z += watcher->getHeight();
  MapUtil::Vector3 target_head = target->getCoord();
  target_head.z += target->getHeight();
  float distance = MapUtil::distance(watcher_head, target_head);
  MapUtil::Vector3 direction = MapUtil::makeVector3((watcher_head.x - target_head.x) / distance, (watcher_head.y - target_head.y) / distance, (watcher_head.y - target_head.y) / distance);
  for(int step = 1; step < distance; step++) {
    MapUtil::Vector3 coord = MapUtil::makeVector3(watcher_head.x + step * direction.x, watcher_head.y + step * direction.y, watcher_head.z + step * direction.z);
    Block * block = getBlock(coord);
    if(block != nullptr && block->opaque) {
      return false;
    }
  }
  return true;
}

std::list<Character *> Region::getCharacters() {
  std::list<Character *> result = std::list<Character *>();
  for(Character * c : characters) {
    result.push_back(c);
  }
  return result;
}

std::list<Character *> Region::getCharacters(Character * character) {
  std::list<Character *> result = std::list<Character *>();
  for(Character * c : characters) {
    if(canSee(character, c)) {
      result.push_back(c);
    }
  }
  return result;
}

std::list<Furniture *> Region::getFurnitures() {
  std::list<Furniture *> result = std::list<Furniture *>();
  for(BlocksChunk * chunk : chunks) {
    for(Furniture * furniture : chunk->getFurnitures()) {
      result.push_front(furniture);
    }
  }
  return result;
}

std::list<Furniture *> Region::getFurnitures(Character * character) {
  std::list<Furniture *> result = std::list<Furniture *>();
  for(BlocksChunk * chunk : chunks) {
    for(Furniture * furniture : chunk->getFurnitures()) {
      result.push_front(furniture);
    }
  }
  return result;
}

std::map<const MapUtil::Vector3i, Block *> Region::getBlocks() {
  std::map<const MapUtil::Vector3i, Block *> result = std::map<const MapUtil::Vector3i, Block *>();
  for(BlocksChunk * chunk : chunks) {
    for(auto pair : chunk->getBlocks()) {
      result.insert(pair);
    }
  }
  return result;
}

Furniture * Region::getFurniture(MapUtil::Vector3i coord) {
  for(BlocksChunk * chunk : chunks) {
    for(Furniture * furniture : chunk->getFurnitures()) {
      if(furniture->getCoord() == coord) {
        return furniture;
      }
    }
  }
  return nullptr;
}
