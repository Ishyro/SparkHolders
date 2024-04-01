#ifndef _CONTAINER_FURNITURE_H_
#define _CONTAINER_FURNITURE_H_

#include <string>
#include <list>

#include "data/furnitures/ActivableFurniture.h"

#include "Values.h"

class ContainerFurniture : public ActivableFurniture  {
  public:
    ContainerFurniture(
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
    ContainerFurniture(
      ContainerFurniture * furniture,
      const int32_t x,
      const int32_t y,
      const int32_t z,
      const float orientation,
      const bool isLocked,
      const std::string key_name,
      const int64_t gold,
      std::list<Item *> loot
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
      gold(gold),
      loot(std::list<Item *>())
    {
      for(Item * item : loot) {
        this->loot.push_back(item);
      }
    }
    ContainerFurniture(ContainerFurniture * furniture, Map * map):
      ActivableFurniture(furniture, map),
      gold(furniture->gold),
      loot(std::list<Item *>())
    {
      // TODO
      for(Item * item : loot) {
        this->loot.push_back(item);
      }
    }
    bool getUnwalkable();
    bool getOpaque();
    bool getSolid();
    int32_t getLight();
    void activate(Character * user, bool remote);

  private:
    int64_t gold;
    std::list<Item *> loot;
};

#endif // _CONTAINER_FURNITURE_H_
