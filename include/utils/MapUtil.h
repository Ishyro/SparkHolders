#ifndef _MAP_UTIL_H_
#define _MAP_UTIL_H_

#include <vector>
#include <list>

#include "Values.h"

namespace MapUtil {
  typedef struct Pair {
    int x;
    int y;
    float dx;
    float dy;
    int score;
    bool operator < (const Pair& p) const { return score < p.score; }
    bool operator == (const Pair& p) const { return x == p.x && y == p.y; }
    bool operator != (const Pair& p) const { return x != p.x || y != p.y; }
  } Pair;

  float round(float var);
  int distance(int x1, int y1, int x2, int y2);
  float distance(int x1, int y1, float dx1, float dy1, int x2, int y2, float dx2, float dy2);
  float distanceSquare(int x1, int y1, float dx1, float dy1, int x2, int y2, float dx2, float dy2);
  float getOrientationToTarget(int x1, int y1, float dx1, float dy1, int x2, int y2, float dx2, float dy2);

  std::vector<Pair> reconstruct_path(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest);
  float reconstruct_orientation(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest);
  std::list<Pair> getNeighboursTraversable(Map * map, int startX, int startY, int destX, int destY);
  std::list<Pair> getNeighboursNonSolid(Map * map, int startX, int startY, int destX, int destY);
  std::vector<Pair> getPathToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying);
  float getOrientationToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying);
  std::list<Pair> getPathFromCartesianEquation(float a, int range);
  std::list<Pair> getPathFromOrientation(int x, int y, float dx, float dy, float orientation, float range);
};

#endif // _MAP_UTIL_H_
