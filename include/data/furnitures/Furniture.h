#ifndef _FURNITURE_H_
#define _FURNITURE_H_

#include <string>

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
    const float fire_size;
    const float fire_posX;
    const float fire_posY;
    const float fire_posZ;
    Furniture(
      const std::string name,
      const int type,
      const int sizeX,
      const int sizeY,
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
      unwalkable(furniture->unwalkable),
      opaque(furniture->opaque),
      solid(furniture->solid),
      light(furniture->light),
      fire_size(furniture->fire_size),
      fire_posX(furniture->fire_posX),
      fire_posY(furniture->fire_posY),
      fire_posZ(furniture->fire_posZ),
      x(x),
      y(y),
      z(z),
      orientation(orientation)
    {}
    Furniture(Furniture * furniture, Map * map):
      id(++furniture::id_cpt),
      name(furniture->name),
      type(furniture->type),
      sizeX(furniture->sizeX),
      sizeY(furniture->sizeY),
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
      init(map, furniture->x, furniture->y);
    }
    void init(Map * map, int x, int y);
    int getX();
    int getY();
    int getZ();
    float getOrientation();
    bool getUnwalkable();
    bool getOpaque();
    bool getSolid();
    int getLight();

  protected:
    int x;
    int y;
    int z;
    float orientation;
    bool unwalkable;
    bool opaque;
    bool solid;
    int light;
};

#endif // _FURNITURE_H_
