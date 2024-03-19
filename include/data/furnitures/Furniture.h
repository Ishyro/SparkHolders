#ifndef _FURNITURE_H_
#define _FURNITURE_H_

#include <string>

#include "util/MapUtil.h"

#include "Values.h"

namespace furniture {
  static long id_cpt = 0;
}

class Furniture {
  public:
    const long id;
    const std::string name;
    const int type;
    const int sizeX;
    const int sizeY;
    const int sizeZ;
    const float fire_size;
    const float fire_posX;
    const float fire_posY;
    const float fire_posZ;
    Furniture(
      const std::string name,
      const int type,
      const int sizeX,
      const int sizeY,
      const int sizeZ,
      const bool unwalkable,
      const bool opaque,
      const bool solid,
      const int light,
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
      const int x,
      const int y,
      const int z,
      const float orientation
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
      coord(MapUtil::makeVector3i(x, y, z)),
      orientation(orientation)
    {}
    Furniture(Furniture * furniture, Map * map):
      id(++furniture::id_cpt),
      name(furniture->name),
      type(furniture->type),
      sizeX(furniture->sizeX),
      sizeY(furniture->sizeY),
      sizeZ(furniture->sizeZ),
      orientation(furniture->orientation),
      unwalkable(furniture->unwalkable),
      opaque(furniture->opaque),
      solid(furniture->solid),
      light(furniture->light),
      fire_size(furniture->fire_size),
      fire_posX(furniture->fire_posX),
      fire_posY(furniture->fire_posY),
      fire_posZ(furniture->fire_posZ)
    {
      init(map, furniture->coord);
    }
    void init(Map * map, MapUtil::Vector3i coord);
    MapUtil::Vector3i getCoord();
    float getOrientation();
    virtual bool getUnwalkable() = 0;
    virtual bool getOpaque() = 0;
    virtual bool getSolid() = 0;
    virtual int getLight() = 0;

  protected:
    MapUtil::Vector3i coord;
    float orientation;
    bool unwalkable;
    bool opaque;
    bool solid;
    int light;
};

#endif // _FURNITURE_H_
