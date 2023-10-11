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
      const bool untraversable,
      const bool opaque,
      const bool solid,
      const int light
    ):
      ActivableFurniture(
        name,
        type,
        sizeX,
        sizeY,
        untraversable,
        opaque,
        solid,
        light
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
