#ifndef _ACTIVABLE_FURNITURE_H_
#define _ACTIVABLE_FURNITURE_H_

#include <string>

#include "data/furnitures/Furniture.h"

#include "Values.h"

class ActivableFurniture : public Furniture {
  public:
    const bool isLocked;
    const std::string key_name;
    
    ActivableFurniture(
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
      Furniture(
        name,
        type,
        sizeX,
        sizeY,
        unwalkable,
        opaque,
        solid,
        light,
        fire_size,
        fire_posX,
        fire_posY,
        fire_posZ
      ),
      isLocked(false),
      key_name("")
    {}
    ActivableFurniture(
      ActivableFurniture * furniture,
      const int x,
      const int y,
      const int z,
      const float orientation,
      const bool isLocked,
      const std::string key_name
    ):
      Furniture(
        furniture,
        x,
        y,
        z,
        orientation
      ),
      isLocked(isLocked),
      key_name(key_name)
    {}
    
    ActivableFurniture(ActivableFurniture * furniture, Map * map):
      Furniture(furniture, map),
      isLocked(furniture->isLocked),
      key_name(furniture->key_name)
    {}
    bool tryActivate(Character * user);
    virtual void activate(Character * user, bool remote) = 0;
};

#endif // _ACTIVABLE_FURNITURE_H_
