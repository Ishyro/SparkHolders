#ifndef _BASIC_FURNITURE_H_
#define _BASIC_FURNITURE_H_

#include <string>

#include "data/furnitures/Furniture.h"

#include "Values.h"

class BasicFurniture : public Furniture {
  public:
    
    BasicFurniture(
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
      Furniture(
        name,
        type,
        sizeX,
        sizeY,
        sizeZ,
        unwalkable,
        opaque,
        solid,
        light,
        fire_size,
        fire_posX,
        fire_posY,
        fire_posZ
      )
    {}
    BasicFurniture(
      BasicFurniture * furniture,
      const int x,
      const int y,
      const int z,
      const float orientation
    ):
      Furniture(
        furniture,
        x,
        y,
        z,
        orientation
      )
    {}
    
    BasicFurniture(BasicFurniture * furniture, Map * map):
      Furniture(furniture, map)
    {}
    bool getUnwalkable();
    bool getOpaque();
    bool getSolid();
    int getLight();
};

#endif // _BASIC_FURNITURE_H_
