#ifndef _MAP_UTIL_H_
#define _MAP_UTIL_H_

#include <vector>
#include <list>

#include "Values.h"

namespace MapUtil {
  typedef struct Pair {
    int x;
    int y;
    int score;
    bool operator < (const Pair& p) const { return score < p.score; }
    bool operator == (const Pair& p) const { return x == p.x && y == p.y; }
    bool operator != (const Pair& p) const { return x != p.x || y != p.y; }
  } Pair;

  int distance(int x1, int y1, int x2, int y2);
  int getOrientationToTarget(int x1, int y1, int x2, int y2);
  Pair getNextPairFromOrientation(int orientation, int x, int y);
  int getDirectOrientationToTarget(int x, int y);
  std::vector<Pair> reconstruct_path(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest);
  int reconstruct_orientation(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest);
  std::list<Pair> getNeighbours(Map * map, int startX, int startY, int destX, int destY);
  std::vector<Pair> getPathToTarget(Map * map, int startX, int startY, int destX, int destY);
  int getOrientationToTarget(Map * map, int startX, int startY, int destX, int destY);
  std::list<Pair> getPathFromCartesianEquationFloor(float a, int range);
  std::list<Pair> getPathFromCartesianEquationCeil(float a, int range);
};

#endif // _MAP_UTIL_H_