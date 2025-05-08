#include "util/Random.h"

namespace Random {
  uint32_t world_seed;
  uint32_t craft_seed;
  uint32_t choice_seed;
  std::mt19937 world_rng;
  std::mt19937 craft_rng;
  std::mt19937 choice_rng;
  std::uniform_int_distribution<> distrib(0, INT32_MAX);

  void setWorldGenerationSeed(uint32_t seed) {
    world_seed = seed;
    world_rng = std::mt19937(world_seed);
  }

  void setCraftSeed(uint32_t seed) {
    craft_seed = seed;
    craft_rng = std::mt19937(world_seed);
  }
  
  void setChoiceSeed(uint32_t seed) {
    choice_seed = seed;
    choice_rng = std::mt19937(world_seed);
  }

  uint32_t getWorldGenerationSeed() { return world_seed; }

  int64_t nextWorld() { return distrib(world_rng); }
  int64_t nextCraft() { return distrib(craft_rng); }
  int64_t nextChoice() { return distrib(choice_rng); }
};

