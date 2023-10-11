#ifndef _SWITCH_FURNITURE_H_
#define _SWITCH_FURNITURE_H_

#include <string>

#include "data/furnitures/ActivableFurniture.h"

#include "Values.h"

class SwitchFurniture : public ActivableFurniture  {
  public:
    SwitchFurniture(
      const std::string name,
      const int type,
      const int sizeX,
      const int sizeY,
      const bool untraversable_on,
      const bool opaque_on,
      const bool solid_on,
      const int light_on,
      const bool untraversable_off,
      const bool opaque_off,
      const bool solid_off,
      const int light_off
    ):
      ActivableFurniture(
        name,
        type,
        sizeX,
        sizeY,
        untraversable_on,
        opaque_on,
        solid_on,
        light_on
      ),
      untraversable_off(untraversable_off),
      opaque_off(opaque_off),
      solid_off(solid_off),
      light_off(light_off)
    {}
    SwitchFurniture(
      SwitchFurniture * furniture,
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
      ),
      untraversable_off(furniture->untraversable_off),
      opaque_off(furniture->opaque_off),
      solid_off(furniture->solid_off),
      light_off(furniture->light_off)
    {}
    SwitchFurniture(SwitchFurniture * furniture, Map * map):
      ActivableFurniture(furniture, map),
      orientation_off(furniture->orientation_off),
      untraversable_off(furniture->untraversable_off),
      opaque_off(furniture->opaque_off),
      solid_off(furniture->solid_off),
      light_off(furniture->light_off),
      isOn(furniture->isOn)
    {}
    float getOrientation();
    bool getUntraversable();
    bool getOpaque();
    bool getSolid();
    int getLight();
    bool getIsOn();
    void activate(Character * user, bool remote);

  private:
    float orientation_off;
    bool untraversable_off;
    bool opaque_off;
    bool solid_off;
    int light_off;
    bool isOn = true;
};

#endif // _SWITCH_FURNITURE_H_
