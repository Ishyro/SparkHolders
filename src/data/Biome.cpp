#include <algorithm>

#include "data/Biome.h"

#include "data/BlocksChunk.h"

#include "util/Random.h"

#include <iostream>

BlocksChunk * Biome::getChunk(std::array<BlocksChunk *, 6> neighbors) {
  std::list<BlocksChunk *> candidates = chunk_collection;
  for(int direction = 0; direction < 6; direction++) {
    if(neighbors[direction] != nullptr) {
      std::list<BlocksChunk *>::iterator to_remvove = std::remove_if(
        candidates.begin(), 
        candidates.end(),
        [&](BlocksChunk * c) { return !checkChunk(direction, neighbors[direction], c); }
      );
      candidates.erase(to_remvove, candidates.end());
    }
  }
  if(!candidates.empty()) {
    int64_t indice = Random::nextWorld() % candidates.size();
    for(int64_t i = 0; i < indice; i++) {
      candidates.pop_front();
    }
    return candidates.front();
  }
  else {
    return nullptr;
  }
}

bool Biome::checkChunk(int32_t direction, BlocksChunk * neighbor, BlocksChunk * to_check) {
  switch(direction) {
    case EAST:
      return satisfySide(to_check->getSide(EAST), neighbor->getSide(WEST));
    case NORTH:
      return satisfySide(to_check->getSide(NORTH), neighbor->getSide(SOUTH));
    case WEST:
      return satisfySide(to_check->getSide(WEST), neighbor->getSide(EAST));
    case SOUTH:
      return satisfySide(to_check->getSide(SOUTH), neighbor->getSide(NORTH));
    case UP:
      return satisfySide(to_check->getSide(UP), neighbor->getSide(DOWN));
    case DOWN:
      return satisfySide(to_check->getSide(DOWN), neighbor->getSide(UP));
  }
}

// probably more complex in the futur
bool Biome::satisfySide(int32_t side1, int32_t side2) {
  return side1 == side2;
}
