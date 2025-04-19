#ifndef _FURNITURE_H_
#define _FURNITURE_H_

#include <string>

#include "util/MathUtil.h"

#include "Values.h"

namespace furniture {
  static int64_t id_counter = 0;
}

class Furniture {
  public:
    const int64_t id;
    const std::string name;
    const int32_t type;
    const int32_t sizeX;
    const int32_t sizeY;
    const int32_t sizeZ;
    const float fire_size;
    const float fire_posX;
    const float fire_posY;
    const float fire_posZ;
    Furniture(
      const std::string name,
      const int32_t type,
      const int32_t sizeX,
      const int32_t sizeY,
      const int32_t sizeZ,
      const bool unwalkable,
      const bool opaque,
      const bool solid,
      const int32_t light,
      const float fire_size,
      const float fire_posX,
      const float fire_posY,
      const float fire_posZ
    ):
      id(0),
      name(name),
      type(type),
      sizeX(sizeX),
      sizeY(sizeY),
      sizeZ(sizeZ),
      unwalkable(unwalkable),
      opaque(opaque),
      solid(solid),
      light(light),
      fire_size(fire_size),
      fire_posX(fire_posX),
      fire_posY(fire_posY),
      fire_posZ(fire_posZ)
    {}
    Furniture(
      Furniture * furniture,
      const int32_t x,
      const int32_t y,
      const int32_t z,
      const float orientation_z
    ):
      id(0),
      name(furniture->name),
      type(furniture->type),
      sizeX(furniture->sizeX),
      sizeY(furniture->sizeY),
      sizeZ(furniture->sizeZ),
      unwalkable(furniture->unwalkable),
      opaque(furniture->opaque),
      solid(furniture->solid),
      light(furniture->light),
      fire_size(furniture->fire_size),
      fire_posX(furniture->fire_posX),
      fire_posY(furniture->fire_posY),
      fire_posZ(furniture->fire_posZ),
      coord(MathUtil::Vector3i(x, y, z)),
      orientation_z(orientation_z)
    {
      hitbox = new MathUtil::HitboxOBB(HITBOX_OBB, MathUtil::Vector3(x, y, z), sizeX, sizeY, sizeZ);
      center = MathUtil::Vector3( (float) x + (float) sizeX * .5F, (float) y + (float) sizeY * .5F, (float) z + (float) sizeZ * .5F);
      hitbox->applyMove(center, 0.F, 0.F, orientation_z);
    }
    Furniture(Furniture * furniture, Map * map):
      id(++furniture::id_counter),
      name(furniture->name),
      type(furniture->type),
      sizeX(furniture->sizeX),
      sizeY(furniture->sizeY),
      sizeZ(furniture->sizeZ),
      orientation_z(furniture->orientation_z),
      unwalkable(furniture->unwalkable),
      opaque(furniture->opaque),
      solid(furniture->solid),
      light(furniture->light),
      fire_size(furniture->fire_size),
      fire_posX(furniture->fire_posX),
      fire_posY(furniture->fire_posY),
      fire_posZ(furniture->fire_posZ)
    {
      hitbox = new MathUtil::HitboxOBB(HITBOX_OBB, MathUtil::Vector3(furniture->coord.x, furniture->coord.y, furniture->coord.z), sizeX, sizeY, sizeZ);
      init(map, furniture->coord);
    }
    void init(Map * map, MathUtil::Vector3i coord);
    MathUtil::Vector3i getCoord();
    MathUtil::Vector3 getCenter();
    MathUtil::HitboxOBB * getHitbox();
    float getOrientationZ();
    virtual bool getUnwalkable() = 0;
    virtual bool getOpaque() = 0;
    virtual bool getSolid() = 0;
    virtual int32_t getLight() = 0;

  protected:
    MathUtil::Vector3i coord;
    MathUtil::Vector3 center;
    MathUtil::HitboxOBB * hitbox;
    float orientation_z;
    bool unwalkable;
    bool opaque;
    bool solid;
    int32_t light;
};

#endif // _FURNITURE_H_
