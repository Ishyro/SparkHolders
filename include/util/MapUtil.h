#ifndef _MAP_UTIL_H_
#define _MAP_UTIL_H_

#include <vector>
#include <list>
#include <string>
#include <cmath>
#include <tuple>

#include "Values.h"

namespace MapUtil {
  typedef struct Pair {
    float x;
    float y;
    int score;
    bool operator < (const Pair& p) const { return score < p.score; }
    bool operator == (const Pair& p) const { return std::floor(x) == std::floor(p.x) && std::floor(y == p.y); }
    bool operator != (const Pair& p) const { return std::floor(x) != std::floor(p.x) || std::floor(y) != std::floor(p.y); }
  } Pair;

  typedef struct Vector3 {
    float x;
    float y;
    float z;
    bool operator == (const Vector3& v) const { return z == v.z && y == v.y && x == v.x; }
    //bool operator != (const Vector3& v) const { return z != v.z || y != v.y || x != v.x; }
  } Vector3;
  
  typedef struct Vector3i {
    int x;
    int y;
    int z;
    bool operator < (const Vector3i& v) const { return std::tie(z, y, x) < std::tie(v.z, v.y, v.x); }
    bool operator == (const Vector3i& v) const { return z == v.z && y == v.y && x == v.x; }
  } Vector3i;

  Vector3 makeVector3(float x, float y, float z);
  Vector3 makeVector3(Vector3i coord_int);
  Vector3i makeVector3i(float x, float y, float z);
  Vector3i makeVector3i(Vector3 coord_float);

  float round(float var);
  MapUtil::Vector3 round(MapUtil::Vector3 var);
  float distanceSquare(float x1, float y1, float x2, float y2);
  float distance(float x1, float y1, float x2, float y2);
  float distance(Vector3 coord1, Vector3 coord2);
  double distanceSquare(MapUtil::Vector3 coord1, MapUtil::Vector3 coord2);
  float distance2(MapUtil::Vector3 coord1, MapUtil::Vector3 coord2);
  float getOrientationToTarget(float x1, float y1, float x2, float y2);
  MapUtil::Vector3 selectClosestVector(MapUtil::Vector3 next, MapUtil::Vector3 dest, int x_direction, int y_direction, int z_direction, float factor_x, float factor_y, float factor_z, float & range);

  std::vector<Pair> reconstruct_path(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest, int offsetX, int offsetY);
  float reconstruct_orientation(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest, int offsetX, int offsetY);
  std::list<Pair> getNeighboursTraversable(Map * map, int startX, int startY, int destX, int destY);
  std::list<Pair> getNeighboursNonSolid(Map * map, int startX, int startY, int destX, int destY);
  std::vector<Pair> getPathToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying);
  float getOrientationToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying);
  std::list<Pair> getPathFromCartesianEquation(float a, int range);
  std::list<Pair> getPathFromOrientation(float x, float y, float orientation, float size, float range);
  std::string target_to_string(Target * target);
  Target * target_from_string(std::string to_read);
};

typedef struct Target {
  int type;
  long id;
  MapUtil::Vector3 coord;
  Target * next;
} Target;

#endif // _MAP_UTIL_H_
