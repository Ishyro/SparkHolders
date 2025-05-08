#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <random>

#include "Values.h"

namespace Random {
  void setWorldGenerationSeed(uint32_t seed);
  void setCraftSeed(uint32_t seed);
  void setChoiceSeed(uint32_t seed);
  uint32_t getWorldGenerationSeed();
  int64_t nextWorld();
  int64_t nextCraft();
  int64_t nextChoice();
};

#endif // _RANDOM_H_
