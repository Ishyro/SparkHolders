#ifndef _MAP_UTIL_H_
#define _MAP_UTIL_H_

#include <vector>
#include <list>
#include <string>
#include <cmath>
#include <tuple>

#include "Values.h"

namespace MathUtil {
  typedef struct Pair {
    float x;
    float y;
    int32_t score;
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
    int64_t x;
    int64_t y;
    int64_t z;
    bool operator < (const Vector3i& v) const { return std::tie(z, y, x) < std::tie(v.z, v.y, v.x); }
    bool operator == (const Vector3i& v) const { return z == v.z && y == v.y && x == v.x; }
  } Vector3i;

  typedef struct Coords {
    Vector3i longitude;
    Vector3i latitude;
  } Coords;

  Vector3 makeVector3(float x, float y, float z);
  Vector3 makeVector3(Vector3i coord_int);
  Vector3i makeVector3i(float x, float y, float z);
  Vector3i makeVector3i(Vector3 coord_float);

  Coords getCoords(Vector3i coord);
  Coords getCoords(Vector3 coord);
  int32_t getLight(MathUtil::Coords coords, Time time);

  float round(float var);
  MathUtil::Vector3 round(MathUtil::Vector3 var);
  float distanceSquare(float x1, float y1, float x2, float y2);
  float distance(float x1, float y1, float x2, float y2);
  float distance(Vector3 coord1, Vector3 coord2);
  double distanceSquare(MathUtil::Vector3 coord1, MathUtil::Vector3 coord2);
  float distance2(MathUtil::Vector3 coord1, MathUtil::Vector3 coord2);
  float getOrientationToTarget(float x1, float y1, float x2, float y2);
  float getOrientationToTarget(MathUtil::Vector3 coord);
  MathUtil::Vector3 selectClosestVector(MathUtil::Vector3 next, MathUtil::Vector3 dest, int32_t x_direction, int32_t y_direction, int32_t z_direction, float factor_x, float factor_y, float factor_z, float & range);

  std::vector<Pair> reconstruct_path(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest, int32_t offsetX, int32_t offsetY);
  float reconstruct_orientation(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest, int32_t offsetX, int32_t offsetY);
  std::list<Pair> getNeighboursTraversable(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY);
  std::list<Pair> getNeighboursNonSolid(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY);
  std::vector<Pair> getPathToTarget(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY, bool flying);
  float getOrientationToTarget(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY, bool flying);
  std::list<Pair> getPathFromCartesianEquation(float a, int32_t range);
  std::list<Pair> getPathFromOrientation(float x, float y, float orientation, float size, float range);
  std::string target_to_string(Target * target);
  Target * target_from_string(std::string to_read, Adventure * adventure);
};

typedef struct Hitbox {
  int32_t type;
  float x;
  float y;
  float z;
} Hitbox;

typedef struct Target {
  int32_t type;
  Character * character;
  MathUtil::Vector3 coord;
  Hitbox hitbox;
  Target * next;
} Target;

#endif // _MAP_UTIL_H_
