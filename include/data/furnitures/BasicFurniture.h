#ifndef _BASIC_FURNITURE_H_
#define _BASIC_FURNITURE_H_

#include <string>

#include "data/furnitures/Furniture.h"

#include "Values.h"

class BasicFurniture : public Furniture {
  public:
    
    BasicFurniture(
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
      const int32_t x,
      const int32_t y,
      const int32_t z,
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
    int32_t getLight();
};

#endif // _BASIC_FURNITURE_H_
