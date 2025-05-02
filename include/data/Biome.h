#ifndef _BIOME_H_
#define _BIOME_H_

#include <string>

#include "util/MathUtil.h"

#include "Values.h"

class Biome {
  public:
    const std::string name;
    const float temperature;
    const float humidity;
    const MathUtil::Vector3i origin;
    const MathUtil::Vector3i end;
    Biome(
      const std::string name,
      const float temperature,
      const float humidity,
      std::list<BlocksChunk *> chunk_collection
    ):
      name(name),
      temperature(temperature),
      humidity(humidity),
      chunk_collection(chunk_collection),
      origin(MathUtil::Vector3i(0, 0, 0)),
      end(MathUtil::Vector3i(0, 0, 0))
    {}
    Biome(
      Biome * biome,
      const MathUtil::Vector3i origin,
      const MathUtil::Vector3i end
    ):
      name(biome->name),
      temperature(biome->temperature),
      humidity(biome->humidity),
      chunk_collection(biome->chunk_collection),
      origin(origin),
      end(end)
    {}
    BlocksChunk * getChunk(std::array<BlocksChunk *, 6> neighbors);
  private:
    std::list<BlocksChunk *> chunk_collection;
};

#endif // _BIOME_H_
