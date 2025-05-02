#ifndef _SWITCH_FURNITURE_H_
#define _SWITCH_FURNITURE_H_

#include <string>

#include "data/furnitures/ActivableFurniture.h"

#include "Values.h"

class SwitchFurniture : public ActivableFurniture  {
  public:
    SwitchFurniture(
      const std::string name,
      const int32_t type,
      const int32_t sizeX,
      const int32_t sizeY,
      const int32_t sizeZ,
      const bool unwalkable_on,
      const bool opaque_on,
      const bool solid_on,
      const int32_t light_on,
      const float activation_time,
      const float fire_size,
      const float fire_posX,
      const float fire_posY,
      const float fire_posZ,
      const bool unwalkable_off,
      const bool opaque_off,
      const bool solid_off,
      const int32_t light_off
    ):
      ActivableFurniture(
        name,
        type,
        sizeX,
        sizeY,
        sizeZ,
        unwalkable_on,
        opaque_on,
        solid_on,
        light_on,
        activation_time,
        fire_size,
        fire_posX,
        fire_posY,
        fire_posZ
      ),
      unwalkable_off(unwalkable_off),
      opaque_off(opaque_off),
      solid_off(solid_off),
      light_off(light_off),
      isOn(true)
    {}
    SwitchFurniture(
      SwitchFurniture * furniture,
      const int32_t x,
      const int32_t y,
      const int32_t z,
      const float orientation_z,
      const bool isLocked,
      const std::string key_name
    ):
      ActivableFurniture(
        furniture,
        x,
        y,
        z,
        orientation_z,
        isLocked,
        key_name
      ),
      unwalkable_off(furniture->unwalkable_off),
      opaque_off(furniture->opaque_off),
      solid_off(furniture->solid_off),
      light_off(furniture->light_off),
      isOn(furniture->isOn)
    {}
    SwitchFurniture(SwitchFurniture * furniture, MathUtil::Vector3i offset, int64_t rotation):
      ActivableFurniture(furniture, offset, rotation),
      orientation_off(furniture->orientation_off),
      unwalkable_off(furniture->unwalkable_off),
      opaque_off(furniture->opaque_off),
      solid_off(furniture->solid_off),
      light_off(furniture->light_off),
      isOn(furniture->isOn)
    {}
    float getOrientationZ();
    bool getUnwalkable();
    bool getOpaque();
    bool getSolid();
    int32_t getLight();
    bool getIsOn();
    void setIsOn(bool isOn);
    void activate(Character * user, bool remote);

  private:
    float orientation_off;
    bool unwalkable_off;
    bool opaque_off;
    bool solid_off;
    int32_t light_off;
    bool isOn;
};

#endif // _SWITCH_FURNITURE_H_
