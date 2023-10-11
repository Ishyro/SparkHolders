#ifndef _FURNITURE_H_
#define _FURNITURE_H_

#include <string>

#include "Values.h"

class Furniture {
  public:
    const std::string name;
    const int type;
    const int sizeX;
    const int sizeY;
    Furniture(
      const std::string name,
      const int type,
      const int sizeX,
      const int sizeY,
      const bool untraversable,
      const bool opaque,
      const bool solid,
      const int light
    ):
      name(name),
      type(type),
      sizeX(sizeX),
      sizeY(sizeY),
      opaque(opaque),
      solid(solid),
      light(light)
    {}
    Furniture(
      Furniture * furniture,
      const int x,
      const int y,
      const int z,
      const float orientation
    ):
      name(furniture->name),
      type(furniture->type),
      sizeX(furniture->sizeX),
      sizeY(furniture->sizeY),
      untraversable(furniture->untraversable),
      opaque(furniture->opaque),
      solid(furniture->solid),
      light(furniture->light),
      x(x),
      y(y),
      z(z),
      orientation(orientation)
    {}
    Furniture(Furniture * furniture, Map * map):
      name(furniture->name),
      type(furniture->type),
      sizeX(furniture->sizeX),
      sizeY(furniture->sizeY),
      orientation(furniture->orientation),
      untraversable(furniture->untraversable),
      opaque(furniture->opaque),
      solid(furniture->solid),
      light(furniture->light)
    {
      init(map, x, y);
    }
    void init(Map * map, int x, int y);
    int getX();
    int getY();
    int getZ();
    float getOrientation();
    bool getUntraversable();
    bool getOpaque();
    bool getSolid();
    int getLight();

  protected:
    int x;
    int y;
    int z;
    float orientation;
    bool untraversable;
    bool opaque;
    bool solid;
    int light;
};

#endif // _FURNITURE_H_
