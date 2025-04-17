#include "data/BlocksChunk.h"

Block * BlocksChunk::getBlock(MathUtil::Vector3i coord) {
  std::map<const MathUtil::Vector3i, LightenedBlock *>::iterator it = blocks.find(coord);
  if(it != blocks.end()) {
    return it->second->raw_block;
  }
  else {
    return nullptr;
  }
}

int32_t BlocksChunk::getLightening(MathUtil::Vector3i coord) {
  std::map<const MathUtil::Vector3i, LightenedBlock *>::iterator it = blocks.find(coord);
  if(it != blocks.end()) {
    return it->second->lightening;
  }
  else {
    return LIGHTENING_NO;
  }
}

void BlocksChunk::setBlock(MathUtil::Vector3i coord, Block * raw_block, int32_t lightening) {
  std::map<const MathUtil::Vector3i, LightenedBlock *>::iterator it = blocks.find(coord);
  if(it != blocks.end()) {
    blocks.erase(it);
  }
  if(raw_block != nullptr) {
    LightenedBlock * block = new LightenedBlock();
    block->raw_block = raw_block;
    block->lightening = lightening;
    blocks.insert(std::make_pair(coord, block));
  }
}

std::map<const MathUtil::Vector3i, Block *> BlocksChunk::getBlocks() {
  std::map<const MathUtil::Vector3i, Block *> result = std::map<const MathUtil::Vector3i, Block *>();
  for(auto pair : blocks) {
     result.insert(std::make_pair(pair.first, pair.second->raw_block));
  }
  return result;
}

std::list<Character *> BlocksChunk::getCharacters() { return characters; }
std::list<Furniture *> BlocksChunk::getFurnitures() { return furnitures; }
std::list<Shield *> BlocksChunk::getShields() { return shields; }

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

void BlocksChunk::addCharacter(Character * character) { characters.push_back(character); }
void BlocksChunk::addFurniture(Furniture * furniture) { furnitures.push_back(furniture); }
void BlocksChunk::addShield(Shield * shield) { shields.push_back(shield); }

bool BlocksChunk::removeCharacter(Character * character) {
  characters.remove(character);
  return characters.empty();
}

bool BlocksChunk::removeShield(Shield * shield) {
  shields.remove(shield);
  return characters.empty();
}
