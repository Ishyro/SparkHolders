#include <algorithm>

#include "data/Biome.h"

#include "data/BlocksChunk.h"

#include "util/Random.h"
#include "util/Logger.h"

#include <iostream>

BlocksChunk * Biome::getChunk(std::array<BlocksChunk *, 10> neighbors) {
  LOGGER_TRACE("neighbors:");
  for(int i = 0; i < 10; i++) {
    if(neighbors[i] != nullptr) {
      LOGGER_TRACE(std::to_string(i) + std::string(": ") + neighbors[i]->name);
    }
  }
  std::list<BlocksChunk *> candidates = chunk_collection;
  for(int direction = 0; direction < 6; direction++) {
    if(neighbors[direction] != nullptr) {
      std::list<BlocksChunk *>::iterator to_remvove = std::remove_if(
        candidates.begin(), 
        candidates.end(),
        [&](BlocksChunk * c) { return !checkChunk(direction, c, neighbors[direction]); }
      );
      candidates.erase(to_remvove, candidates.end());
    }
    else if(direction == UP) {
      // chunks touching the void must be "opened" toward the void
      std::list<BlocksChunk *>::iterator to_remvove = std::remove_if(
        candidates.begin(), 
        candidates.end(),
        [](BlocksChunk * c) { return c->getSide(UP).type != SIDE_OPENING; }
      );
      candidates.erase(to_remvove, candidates.end());
    }
  }
  // remove chunks forbbiden by altitude
  LOGGER_TRACE("before: " + std::to_string(candidates.size()));
  for(BlocksChunk * chunk : candidates) {
    LOGGER_TRACE(chunk->name);
  }
  std::pair<int32_t, int32_t> min_max = getAltitudeDiff(neighbors);
  // remove only if the neighbors themselves respect the altitude diff 
  if(min_max.second - min_max.first <= altitude_max_diff) {
    if(min_max != std::make_pair(15, 0)) {
      std::list<BlocksChunk *>::iterator to_remvove = std::remove_if(
        candidates.begin(), 
        candidates.end(),
        [&](BlocksChunk * c) {
          int32_t min = std::min(std::min(std::min(c->getSide(EAST).start_y, c->getSide(NORTH).start_y), c->getSide(WEST).start_y), c->getSide(SOUTH).start_y);
          int32_t max = std::max(std::max(std::max(c->getSide(EAST).start_y, c->getSide(NORTH).start_y), c->getSide(WEST).start_y), c->getSide(SOUTH).start_y);
          return std::max(min_max.second, max) - std::min(min_max.first, min) > altitude_max_diff;
          //return std::abs(min_max.first - min) > altitude_max_diff || std::abs(max - min_max.second) > altitude_max_diff;
        }
      );
      candidates.erase(to_remvove, candidates.end());
    }
  }
  LOGGER_TRACE("after: " + std::to_string(candidates.size()));
  for(BlocksChunk * chunk : candidates) {
    LOGGER_TRACE(chunk->name);
  }
  if(!candidates.empty()) {
    float total_weight = 0.F;
    for(BlocksChunk * chunk : candidates) {
      total_weight += getWeight(chunk);
    }
    if(total_weight == 0.F) {
      return nullptr;
    }
    float rand_weight = (float) (Random::nextWorld() % ( (int64_t) total_weight));
    float weight = 0.F;
    BlocksChunk * result = nullptr;
    do {
      result = candidates.front();
      candidates.pop_front();
      weight += getWeight(result);
    } while(weight < rand_weight);
    LOGGER_TRACE("keep: " + result->name);
    return result;
  }
  else {
    return nullptr;
  }
}

bool Biome::checkChunk(int32_t direction, BlocksChunk * to_check, BlocksChunk * neighbor) {
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

std::pair<int32_t, int32_t> Biome::getAltitudeDiff(std::array<BlocksChunk *, 10> neighbors) {
  int32_t min = 15;
  int32_t max = 0;
  for(int direction = 0; direction < 10; direction++) {
    if(direction == UP) {
      // skip UP and DOWN
      direction += 2;
    }
    if(neighbors[direction] != nullptr) {
      min = std::min(std::min(std::min(std::min(neighbors[direction]->getSide(EAST).start_y, neighbors[direction]->getSide(NORTH).start_y), neighbors[direction]->getSide(WEST).start_y), neighbors[direction]->getSide(SOUTH).start_y), min);
      max = std::max(std::max(std::max(std::max(neighbors[direction]->getSide(EAST).start_y, neighbors[direction]->getSide(NORTH).start_y), neighbors[direction]->getSide(WEST).start_y), neighbors[direction]->getSide(SOUTH).start_y), max);
    }
  }
  return std::make_pair(min, max);
}

float Biome::getWeight(BlocksChunk * chunk) {
  if(chunk->getSide(EAST).start_y == chunk->getSide(NORTH).start_y && chunk->getSide(EAST).start_y == chunk->getSide(WEST).start_y && chunk->getSide(EAST).start_y == chunk->getSide(SOUTH).start_y) {
    return chunk->weight;
  }
  else {
    return chunk->weight * altitude_variance;
  }
}

// probably more complex in the futur
bool Biome::satisfySide(ChunkSide side1, ChunkSide side2) {
  bool first_check = side1 == side2;
  if(first_check) {
    if(side1.subtype == SIDE_SLOPE && side2.subtype == SIDE_SLOPE) {
      return false;
    }
    return true;
  }
  else {
    return false;
  }
}
