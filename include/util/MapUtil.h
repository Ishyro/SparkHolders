#ifndef _MAP_UTIL_H_
#define _MAP_UTIL_H_

#include <vector>
#include <list>
#include <cmath>

#include "Values.h"

namespace MapUtil {
  typedef struct Pair {
    float x;
    float y;
    int score;
    bool operator < (const Pair& p) const { return score < p.score; }
    bool operator == (const Pair& p) const { return  std::floor(x) == std::floor(p.x) && std::floor(y == p.y); }
    bool operator != (const Pair& p) const { return std::floor(x) != std::floor(p.x) || std::floor(y) != std::floor(p.y); }
  } Pair;

  float round(float var);
  float distanceSquare(float x1, float y1, float x2, float y2);
  float distance(float x1, float y1, float x2, float y2);
  float getOrientationToTarget(float x1, float y1, float x2, float y2);

  std::vector<Pair> reconstruct_path(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest, int offsetX, int offsetY);
  float reconstruct_orientation(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest, int offsetX, int offsetY);
  std::list<Pair> getNeighboursTraversable(Map * map, int startX, int startY, int destX, int destY);
  std::list<Pair> getNeighboursNonSolid(Map * map, int startX, int startY, int destX, int destY);
  std::vector<Pair> getPathToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying);
  float getOrientationToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying);
  std::list<Pair> getPathFromCartesianEquation(float a, int range);
  std::list<Pair> getPathFromOrientation(float x, float y, float orientation, float size, float range);
};

#endif // _MAP_UTIL_H_
