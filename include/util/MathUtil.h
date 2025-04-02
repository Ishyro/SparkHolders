#ifndef _MAP_UTIL_H_
#define _MAP_UTIL_H_

#include <vector>
#include <list>
#include <string>
#include <cmath>
#include <tuple>

#include "Values.h"

namespace MathUtil {
  struct Pair {
    float x;
    float y;
    int32_t score;
    Pair(){}
    bool operator < (const Pair& p) const { return score < p.score; }
    bool operator == (const Pair& p) const { return std::floor(x) == std::floor(p.x) && std::floor(y == p.y); }
    bool operator != (const Pair& p) const { return std::floor(x) != std::floor(p.x) || std::floor(y) != std::floor(p.y); }
  };

  struct Vector3 {
    float x;
    float y;
    float z;
    Vector3() {}
    Vector3(float x, float y, float z): x(x), y(y), z(z) {}
    Vector3(const Vector3& coords): x(coords.x), y(coords.y), z(coords.z) {}
    bool operator == (const Vector3& v) const { return z == v.z && y == v.y && x == v.x; }
    Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    float dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
    Vector3 cross(const Vector3& other) const {
      return Vector3(
          y * other.z - z * other.y,
          z * other.x - x * other.z,
          x * other.y - y * other.x
      );
    }
  };
  
  struct Vector3i {
    int64_t x;
    int64_t y;
    int64_t z;
    Vector3i() {}
    Vector3i(int32_t x, int32_t y, int32_t z): x( (int64_t) x), y( (int64_t) y), z( (int64_t) z) {}
    Vector3i(int64_t x, int64_t y, int64_t z): x(x), y(y), z(z) {}
    Vector3i(const Vector3i& coords): x(coords.x), y(coords.y), z(coords.z) {}
    Vector3i(float x, float y, float z): x( (int64_t) std::floor(x)), y( (int64_t) std::floor(y)), z( (int64_t) std::floor(z)) {}
    Vector3i(Vector3 coords): x( (int64_t) std::floor(coords.x)), y( (int64_t) std::floor(coords.y)), z( (int64_t) std::floor(coords.z)) {}
    bool operator < (const Vector3i& v) const { return std::tie(z, y, x) < std::tie(v.z, v.y, v.x); }
    bool operator == (const Vector3i& v) const { return z == v.z && y == v.y && x == v.x; }
  };

  struct Coords {
    Vector3i longitude;
    Vector3i latitude;
    Coords() {}
    Coords(Vector3i longitude, Vector3i latitude): longitude(longitude), latitude(latitude) {}
  };

  struct Hitbox {
    int64_t type;
    Vector3 origin;
    Hitbox(int64_t type, Vector3 origin): type(type), origin(origin) {}
  };

  struct HitboxBall : public Hitbox {
    float radius;
    HitboxBall(int64_t type, Vector3 origin, float radius): Hitbox(type, origin), radius(radius) {}
  };

  struct HitboxAABB : public Hitbox {
    float sizeX;
    float sizeY;
    float sizeZ;
    HitboxAABB(int64_t type, Vector3 origin, float sizeX, float sizeY, float sizeZ): Hitbox(type, origin), sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {}
  };

  struct HitboxOBB : public HitboxAABB {
    Vector3 x_axis;
    Vector3 y_axis;
    Vector3 z_axis;
    HitboxOBB(int64_t type, Vector3 origin, float sizeX, float sizeY, float sizeZ):
      HitboxAABB(type, origin, sizeX, sizeY, sizeZ), x_axis(Vector3(1, 0, 0)), y_axis(Vector3(0, 1, 0)), z_axis(Vector3(0, 0, 1)) {}
    HitboxOBB(HitboxOBB * hitbox):
      HitboxAABB(hitbox->type, hitbox->origin, hitbox->sizeX, hitbox->sizeY, hitbox->sizeZ), x_axis(hitbox->x_axis), y_axis(hitbox->y_axis), z_axis(hitbox->z_axis) {}
    std::vector<Vector3> getCorners() const {
        std::vector<Vector3> corners(8);
        Vector3 halfExtents(sizeX / 2, sizeY / 2, sizeZ / 2);

        corners[0] = origin + x_axis * halfExtents.x + y_axis * halfExtents.y + z_axis * halfExtents.z;
        corners[1] = origin + x_axis * halfExtents.x + y_axis * halfExtents.y - z_axis * halfExtents.z;
        corners[2] = origin + x_axis * halfExtents.x - y_axis * halfExtents.y + z_axis * halfExtents.z;
        corners[3] = origin + x_axis * halfExtents.x - y_axis * halfExtents.y - z_axis * halfExtents.z;
        corners[4] = origin - x_axis * halfExtents.x + y_axis * halfExtents.y + z_axis * halfExtents.z;
        corners[5] = origin - x_axis * halfExtents.x + y_axis * halfExtents.y - z_axis * halfExtents.z;
        corners[6] = origin - x_axis * halfExtents.x - y_axis * halfExtents.y + z_axis * halfExtents.z;
        corners[7] = origin - x_axis * halfExtents.x - y_axis * halfExtents.y - z_axis * halfExtents.z;

        return corners;
    }
    void applyMove(Vector3 coord, float thetaX, float thetaY, float thetaZ) {
      origin = coord;
      origin.z += sizeZ * .5F;
      float radX = thetaX * M_PI / 180.F;
      float radY = thetaY * M_PI / 180.F;
      float radZ = thetaZ * M_PI / 180.F;
      float cosX = std::cos(radX), sinX = std::sin(radX);
      float cosY = std::cos(radY), sinY = std::sin(radY);
      float cosZ = std::cos(radZ), sinZ = std::sin(radZ);

      float rotX[3][3] = {
        {1, 0, 0},
        {0, cosX, -sinX},
        {0, sinX, cosX}
      };
      float rotY[3][3] = {
        {cosY, 0, sinY},
        {0, 1, 0},
        {-sinY, 0, cosY}
      };
      float rotZ[3][3] = {
        {cosZ, -sinZ, 0},
        {sinZ, cosZ, 0},
        {0, 0, 1}
      };
      float result[3][3] = {0};

      // R = Rz * Ry
      float temp[3][3] = {0};
      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          for (int k = 0; k < 3; ++k) {
            temp[i][j] += rotZ[i][k] * rotY[k][j];
          }
        }
      }

      // R = (Rz * Ry) * Rx
      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          for (int k = 0; k < 3; ++k) {
            result[i][j] += temp[i][k] * rotX[k][j];
          }
        }
      }
      
      x_axis = Vector3(result[0][0] * x_axis.x + result[0][1] * x_axis.y + result[0][2] * x_axis.z,
                       result[1][0] * x_axis.x + result[1][1] * x_axis.y + result[1][2] * x_axis.z,
                       result[2][0] * x_axis.x + result[2][1] * x_axis.y + result[2][2] * x_axis.z);

      y_axis = Vector3(result[0][0] * y_axis.x + result[0][1] * y_axis.y + result[0][2] * y_axis.z,
                       result[1][0] * y_axis.x + result[1][1] * y_axis.y + result[1][2] * y_axis.z,
                       result[2][0] * y_axis.x + result[2][1] * y_axis.y + result[2][2] * y_axis.z);

      z_axis = Vector3(result[0][0] * z_axis.x + result[0][1] * z_axis.y + result[0][2] * z_axis.z,
                       result[1][0] * z_axis.x + result[1][1] * z_axis.y + result[1][2] * z_axis.z,
                       result[2][0] * z_axis.x + result[2][1] * z_axis.y + result[2][2] * z_axis.z);
    }
  };

  struct Target {
    int32_t type;
    int64_t id; //client side only
    Character * character;
    Furniture * furniture;
    Vector3 coord;
    Target * next;
    Target() {}
    Target(int32_t type, Character * character, Furniture * furniture, Vector3 coord, Target * next):
      type(type), id(0), character(character), furniture(furniture), coord(coord), next(next) {}
  };

  Coords getCoords(Vector3i coord);
  Coords getCoords(Vector3 coord);
  int32_t getLight(Coords coords, Time time);

  float round(float var);
  Vector3 round(Vector3 var);
  float distanceSquare(float x1, float y1, float x2, float y2);
  float distance(float x1, float y1, float x2, float y2);
  float distance(Vector3 coord1, Vector3 coord2);
  double distanceSquare(Vector3 coord1, Vector3 coord2);
  float distance2(Vector3 coord1, Vector3 coord2);
  float getOrientationToTarget(float x1, float y1, float x2, float y2);
  float getOrientationToTarget(Vector3 coord);
  Vector3 selectClosestVector(Vector3 next, Vector3 dest, int32_t x_direction, int32_t y_direction, int32_t z_direction, float factor_x, float factor_y, float factor_z, float & range);

  std::vector<Pair> reconstruct_path(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest, int32_t offsetX, int32_t offsetY);
  float reconstruct_orientation(std::vector<std::vector<Pair>> cameFrom, Pair start, Pair dest, int32_t offsetX, int32_t offsetY);
  std::list<Pair> getNeighboursTraversable(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY);
  std::list<Pair> getNeighboursNonSolid(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY);
  std::vector<Pair> getPathToTarget(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY, bool flying);
  float getOrientationToTarget(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY, bool flying);
  std::list<Pair> getPathFromCartesianEquation(float a, int32_t range);
  std::list<Pair> getPathFromOrientation(float x, float y, float orientation_z, float size, float range);
  std::string target_to_string(Target * target);
  Target * target_from_string(std::string to_read, Adventure * adventure);
  bool collide(Hitbox * box1, Hitbox * box2);
  bool collideBalls(HitboxBall * box1, HitboxBall * box2);
  bool collideAABB(HitboxAABB * box1, HitboxAABB * box2);
  bool collideOBB(HitboxOBB * box1, HitboxOBB * box2);
  bool collideBallAABB(HitboxBall * box1, HitboxAABB * box2);
  bool collideBallOBB(HitboxBall * box1, HitboxOBB * box2);
  bool collideAABBOBB(HitboxAABB * box1, HitboxOBB * box2);
};

#endif // _MAP_UTIL_H_
