#include "data/Biome.h"

#include "data/BlocksChunk.h"

BlocksChunk * Biome::getChunk(std::array<BlocksChunk *, 6> neighbors) {
  // nice generation
  return chunk_collection.front();
}