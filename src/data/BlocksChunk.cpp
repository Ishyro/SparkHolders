#include "data/BlocksChunk.h"

#include <iostream>

Block * BlocksChunk::getBlock(MathUtil::Vector3i coord) {
  std::map<const MathUtil::Vector3i, Block *>::iterator it = blocks.find(coord);
  if(it != blocks.end()) {
    return it->second;
  }
  else {
    return nullptr;
  }
}

void BlocksChunk::setBlock(MathUtil::Vector3i coord, Block * block) {
  std::map<const MathUtil::Vector3i, Block *>::iterator it = blocks.find(coord);
  if(it != blocks.end()) {
    blocks.erase(it);
  }
  if(block != nullptr) {
    blocks.insert(std::make_pair(coord, block));
  }
}

std::map<const MathUtil::Vector3i, Block *> BlocksChunk::getBlocks() { return blocks; }
std::list<Furniture *> BlocksChunk::getFurnitures() { return furnitures; }

MathUtil::Vector3i BlocksChunk::getChunkId(MathUtil::Vector3 ori) {
  MathUtil::Vector3i coord = MathUtil::Vector3i();
  coord.x = (int32_t) std::floor(ori.x) - (((int32_t) (std::floor(ori.x)) % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  coord.y = (int32_t) std::floor(ori.y) - (((int32_t) (std::floor(ori.y)) % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  coord.z = (int32_t) std::floor(ori.z) - (((int32_t) (std::floor(ori.z)) % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  return coord;
}

MathUtil::Vector3i BlocksChunk::getChunkId(MathUtil::Vector3i ori) {
  MathUtil::Vector3i coord = MathUtil::Vector3i();
  coord.x = ori.x - ((ori.x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  coord.y = ori.y - ((ori.y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  coord.z = ori.z - ((ori.z % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
  return coord;
}

void BlocksChunk::addFurniture(Furniture * furniture) { furnitures.push_back(furniture); }
