#include "data/BlocksChunk.h"

#include <iostream>

Block * BlocksChunk::getBlock(MapUtil::Vector3i coord) {
  std::map<const MapUtil::Vector3i, Block *>::iterator it = blocks.find(coord);
  if(it != blocks.end()) {
    return it->second;
  }
  else {
    return nullptr;
  }
}

void BlocksChunk::setBlock(MapUtil::Vector3i coord, Block * block) {
  std::map<const MapUtil::Vector3i, Block *>::iterator it = blocks.find(coord);
  if(it != blocks.end()) {
    blocks.erase(it);
  }
  if(block != nullptr) {
    blocks.insert(std::make_pair(coord, block));
  }
}

std::map<const MapUtil::Vector3i, Block *> BlocksChunk::getBlocks() { return blocks; }
std::list<Furniture *> BlocksChunk::getFurnitures() { return furnitures; }

MapUtil::Vector3i BlocksChunk::getChunkId(MapUtil::Vector3 ori) {
  MapUtil::Vector3i coord = MapUtil::Vector3i();
  coord.x = (int) std::floor(ori.x) - (((int) (std::floor(ori.x)) % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  coord.y = (int) std::floor(ori.y) - (((int) (std::floor(ori.y)) % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  coord.z = (int) std::floor(ori.z) - (((int) (std::floor(ori.z)) % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  return coord;
}

MapUtil::Vector3i BlocksChunk::getChunkId(MapUtil::Vector3i ori) {
  MapUtil::Vector3i coord = MapUtil::Vector3i();
  coord.x = ori.x - ((ori.x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  coord.y = ori.y - ((ori.y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  coord.z = ori.z - ((ori.z % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  return coord;
}

void BlocksChunk::addFurniture(Furniture * furniture) { furnitures.push_back(furniture); }
