#ifndef _LINKED_FURNITURE_H_
#define _LINKED_FURNITURE_H_

#include <string>

#include "data/furnitures/ActivableFurniture.h"

#include "Values.h"

class LinkedFurniture : public ActivableFurniture  {
  public:
    LinkedFurniture(
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
      ActivableFurniture(
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
      )
    {}
    LinkedFurniture(
      LinkedFurniture * furniture,
      const int x,
      const int y,
      const int z,
      const float orientation,
      const bool isLocked,
      const std::string key_name,
      ActivableFurniture * linked_furniture
    ):
      ActivableFurniture(
        furniture,
        x,
        y,
        z,
        orientation,
        isLocked,
        key_name
      ),
      linked_furniture(linked_furniture)
    {}
    LinkedFurniture(LinkedFurniture * furniture, Map * map):
      ActivableFurniture(furniture, map)
    {
      init(map, furniture->linked_furniture);
    }
    void init(Map * map, ActivableFurniture * linked_furniture);
    Furniture * getLinkedFurniture();
    void setLinkedFurniture(ActivableFurniture * linked_furniture);
    void activate(Character * user, bool remote);

  private:
    ActivableFurniture * linked_furniture;
};

#endif // _LINKED_FURNITURE_H_
