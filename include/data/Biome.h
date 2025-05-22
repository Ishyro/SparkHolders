#ifndef _BIOME_H_
#define _BIOME_H_

#include <string>

#include "util/MathUtil.h"

#include "Values.h"

class Biome {
  public:
    const std::string name;
    const int32_t altitude_max_diff;
    const float altitude_variance;
    const float temperature;
    const float humidity;
    const MathUtil::Vector3i origin;
    const MathUtil::Vector3i end;
    Biome(
      const std::string name,
      const int32_t altitude_max_diff,
      const float altitude_variance,
      const float temperature,
      const float humidity,
      std::list<BlocksChunk *> chunk_collection
    ):
      name(name),
      altitude_max_diff(altitude_max_diff),
      altitude_variance(altitude_variance),
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
      altitude_max_diff(biome->altitude_max_diff),
      altitude_variance(biome->altitude_variance),
      temperature(biome->temperature),
      humidity(biome->humidity),
      chunk_collection(biome->chunk_collection),
      origin(
        std::min(origin.x, end.x),
        std::min(origin.y, end.y),
        std::min(origin.z, end.z)
      ),
      end(
        std::max(origin.x, end.x),
        std::max(origin.y, end.y),
        std::max(origin.z, end.z)
      )
    {}
    BlocksChunk * getChunk(std::array<BlocksChunk *, 10> neighbors);
    bool checkChunk(int32_t direction, BlocksChunk * neighbor, BlocksChunk * to_check);
    std::pair<int32_t, int32_t> getAltitudeDiff(std::array<BlocksChunk *, 10> neighbors);
    float getWeight(BlocksChunk * chunk);
    bool satisfySide(ChunkSide side1, ChunkSide side2);
  private:
    std::list<BlocksChunk *> chunk_collection;
};

#endif // _BIOME_H_
