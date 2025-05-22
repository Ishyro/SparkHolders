#include "data/BlocksChunk.h"

Block * BlocksChunk::getBlock(MathUtil::Vector3i coord) {
  std::map<const MathUtil::Vector3i, Block *>::iterator it = outer_blocks.find(MathUtil::mod(coord, CHUNK_SIZE));
  if(it != outer_blocks.end()) {
    return it->second;
  }
  else {
    it = inner_blocks.find(MathUtil::mod(coord, CHUNK_SIZE));
    if(it != inner_blocks.end()) {
      return it->second;
    }
    return nullptr;
  }
}

void BlocksChunk::setBlock(MathUtil::Vector3i coord, Block * block) {
  MathUtil::Vector3i local = MathUtil::Vector3i(MathUtil::mod(coord, CHUNK_SIZE));
  std::map<const MathUtil::Vector3i, Block *>::iterator it = outer_blocks.find(local);
  if(it != outer_blocks.end()) {
    outer_blocks.erase(it);
  }
  if(block != nullptr) {
    outer_blocks.insert(std::make_pair(local, block));
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

std::map<const MathUtil::Vector3i, Block *> BlocksChunk::getOuterBlocks(MathUtil::Vector3i offset) {
  std::map<const MathUtil::Vector3i, Block *> result = std::map<const MathUtil::Vector3i, Block *>();
  for(std::pair<MathUtil::Vector3i, Block *> pair : outer_blocks) {
    result.insert(std::make_pair(pair.first + offset, pair.second));
  }
  return result;
}

std::map<const MathUtil::Vector3i, Block *> BlocksChunk::getBlocks(MathUtil::Vector3i offset) {
  std::map<const MathUtil::Vector3i, Block *> result = std::map<const MathUtil::Vector3i, Block *>();
  for(std::pair<MathUtil::Vector3i, Block *> pair : inner_blocks) {
    result.insert(std::make_pair(pair.first + offset, pair.second));
  }
  for(std::pair<MathUtil::Vector3i, Block *> pair : outer_blocks) {
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

void BlocksChunk::computeOuterBlocks() {
  for(std::pair<MathUtil::Vector3i, Block *> pair : outer_blocks) {
    inner_blocks.insert(pair);
  }
  outer_blocks.clear();
  std::list<MathUtil::Vector3i> blocks_to_outer = std::list<MathUtil::Vector3i>();
  for(std::pair<MathUtil::Vector3i, Block *> pair : inner_blocks) {
    // add the block only if it is in contact with air
    if(inner_blocks.find(MathUtil::Vector3i(pair.first.x - 1, pair.first.y, pair.first.z)) == inner_blocks.end() ||
      inner_blocks.find(MathUtil::Vector3i(pair.first.x + 1, pair.first.y, pair.first.z)) == inner_blocks.end() ||
      inner_blocks.find(MathUtil::Vector3i(pair.first.x, pair.first.y - 1, pair.first.z)) == inner_blocks.end() ||
      inner_blocks.find(MathUtil::Vector3i(pair.first.x, pair.first.y + 1, pair.first.z)) == inner_blocks.end() ||
      inner_blocks.find(MathUtil::Vector3i(pair.first.x, pair.first.y, pair.first.z - 1)) == inner_blocks.end() ||
      inner_blocks.find(MathUtil::Vector3i(pair.first.x, pair.first.y, pair.first.z + 1)) == inner_blocks.end()
    ) {
      blocks_to_outer.push_back(pair.first);
      outer_blocks.insert(pair);
    }
  }
  for(MathUtil::Vector3i coord : blocks_to_outer) {
    inner_blocks.erase(coord);
  }
}

ChunkSide BlocksChunk::getSide(int32_t side) { return sides[side]; }

std::list<Furniture *> BlocksChunk::getFurnitures() { return furnitures; }

MathUtil::Vector3i BlocksChunk::getChunkId(MathUtil::Vector3 ori) {
  return getChunkId(MathUtil::Vector3i(ori));
}

MathUtil::Vector3i BlocksChunk::getChunkId(MathUtil::Vector3i ori) {
  return ori - MathUtil::mod(ori, CHUNK_SIZE);
}

void BlocksChunk::addFurniture(Furniture * furniture) { furnitures.push_back(furniture); }
