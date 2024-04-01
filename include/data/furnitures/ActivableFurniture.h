#ifndef _ACTIVABLE_FURNITURE_H_
#define _ACTIVABLE_FURNITURE_H_

#include <string>

#include "data/furnitures/Furniture.h"

#include "Values.h"

class ActivableFurniture : public Furniture {
  public:
    const float activation_time;
    const bool isLocked;
    const std::string key_name;
    
    ActivableFurniture(
      const std::string name,
      const int32_t type,
      const int32_t sizeX,
      const int32_t sizeY,
      const int32_t sizeZ,
      const bool unwalkable,
      const bool opaque,
      const bool solid,
      const int32_t light,
      const float activation_time,
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
      ),
      activation_time(activation_time),
      isLocked(false),
      key_name("")
    {}
    ActivableFurniture(
      ActivableFurniture * furniture,
      const int32_t x,
      const int32_t y,
      const int32_t z,
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
      activation_time(furniture->activation_time),
      isLocked(isLocked),
      key_name(key_name)
    {}
    
    ActivableFurniture(ActivableFurniture * furniture, Map * map):
      Furniture(furniture, map),
      activation_time(furniture->activation_time),
      isLocked(furniture->isLocked),
      key_name(furniture->key_name)
    {}
    bool tryActivate(Character * user);
    virtual void activate(Character * user, bool remote) = 0;
};

#endif // _ACTIVABLE_FURNITURE_H_
