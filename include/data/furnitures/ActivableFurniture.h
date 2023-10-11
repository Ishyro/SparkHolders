#ifndef _ACTIVABLE_FURNITURE_H_
#define _ACTIVABLE_FURNITURE_H_

#include <string>

#include "data/furnitures/Furniture.h"

#include "Values.h"

namespace furniture {
  static long id_cpt = 0;
}

class ActivableFurniture : public Furniture {
  public:
    const long id;
    const bool isLocked;
    const std::string key_name;
    
    ActivableFurniture(
      const std::string name,
      const int type,
      const int sizeX,
      const int sizeY,
      const bool untraversable,
      const bool opaque,
      const bool solid,
      const int light
    ):
      Furniture(
        name,
        type,
        sizeX,
        sizeY,
        untraversable,
        opaque,
        solid,
        light
      ),
      id(0),
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
      id(0),
      isLocked(isLocked),
      key_name(key_name)
    {}
    
    ActivableFurniture(ActivableFurniture * furniture, Map * map):
      Furniture(furniture, map),
      id(++furniture::id_cpt),
      isLocked(furniture->isLocked),
      key_name(furniture->key_name)
    {}
    bool tryActivate(Character * user);
    virtual void activate(Character * user, bool remote) = 0;
};

#endif // _ACTIVABLE_FURNITURE_H_
