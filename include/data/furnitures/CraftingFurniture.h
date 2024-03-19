#ifndef _CRAFTING_FURNITURE_H_
#define _CRAFTING_FURNITURE_H_

#include <string>

#include "data/furnitures/ActivableFurniture.h"

#include "Values.h"

class CraftingFurniture : public ActivableFurniture  {
  public:
    CraftingFurniture(
      const std::string name,
      const int type,
      const int sizeX,
      const int sizeY,
      const int sizeZ,
      const bool unwalkable,
      const bool opaque,
      const bool solid,
      const int light,
      const float activation_time,
      const float fire_size,
      const float fire_posX,
      const float fire_posY,
      const float fire_posZ
    ):
      ActivableFurniture(
        name,
        type,
        sizeX,
        sizeY,
        sizeZ,
        unwalkable,
        opaque,
        solid,
        light,
        activation_time,
        fire_size,
        fire_posX,
        fire_posY,
        fire_posZ
      )
    {}
    CraftingFurniture(
      CraftingFurniture * furniture,
      const int x,
      const int y,
      const int z,
      const float orientation,
      const bool isLocked,
      const std::string key_name
    ):
      ActivableFurniture(
        furniture,
        x,
        y,
        z,
        orientation,
        isLocked,
        key_name
      )
    {}
    CraftingFurniture(CraftingFurniture * furniture, Map * map):
      ActivableFurniture(furniture, map)
    {}
    bool getUnwalkable();
    bool getOpaque();
    bool getSolid();
    int getLight();
    void activate(Character * user, bool remote);
};

#endif // _CRAFTING_FURNITURE_H_
