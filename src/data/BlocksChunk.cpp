#include "data/BlocksChunk.h"

Block * BlocksChunk::getBlock(MathUtil::Vector3i coord) {
  std::map<const MathUtil::Vector3i, Block *>::iterator it = blocks.find(MathUtil::mod(coord, CHUNK_SIZE));
  if(it != blocks.end()) {
    return it->second;
  }
  else {
    return nullptr;
  }
}

void BlocksChunk::setBlock(MathUtil::Vector3i coord, Block * block) {
  MathUtil::Vector3i local = MathUtil::Vector3i(MathUtil::mod(coord, CHUNK_SIZE));
  std::map<const MathUtil::Vector3i, Block *>::iterator it = blocks.find(local);
  if(it != blocks.end()) {
    blocks.erase(it);
  }
  if(block != nullptr) {
    blocks.insert(std::make_pair(local, block));
  }
  if(instanciated) {
    it = diffs.find(local);
    if(it != diffs.end()) {
      diffs.erase(it);
    }
    if(block != nullptr) {
      diffs.insert(std::make_pair(local, block));
    }
  }
}

std::map<const MathUtil::Vector3i, Block *> BlocksChunk::getBlocks(MathUtil::Vector3i offset) {
  std::map<const MathUtil::Vector3i, Block *> result = std::map<const MathUtil::Vector3i, Block *>();
  for(std::pair<MathUtil::Vector3i, Block *> pair : blocks) {
    result.insert(std::make_pair(pair.first + offset, pair.second));
  }
  return result;
}

std::map<const MathUtil::Vector3i, Block *> BlocksChunk::getDiffs(MathUtil::Vector3i offset) {
  std::map<const MathUtil::Vector3i, Block *> result = std::map<const MathUtil::Vector3i, Block *>();
  for(std::pair<MathUtil::Vector3i, Block *> pair : diffs) {
    result.insert(std::make_pair(pair.first + offset, pair.second));
  }
  return result;
}

int32_t BlocksChunk::getSide(int32_t side) { return sides[side]; }

std::list<Furniture *> BlocksChunk::getFurnitures() { return furnitures; }

MathUtil::Vector3i BlocksChunk::getChunkId(MathUtil::Vector3 ori) {
  return getChunkId(MathUtil::Vector3i(ori));
}

MathUtil::Vector3i BlocksChunk::getChunkId(MathUtil::Vector3i ori) {
  return ori - MathUtil::mod(ori, CHUNK_SIZE);
}

void BlocksChunk::addFurniture(Furniture * furniture) { furnitures.push_back(furniture); }
