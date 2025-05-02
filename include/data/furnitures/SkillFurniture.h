#ifndef _SKILL_FURNITURE_H_
#define _SKILL_FURNITURE_H_

#include <string>

#include "data/furnitures/ActivableFurniture.h"

#include "Values.h"

class SkillFurniture : public ActivableFurniture  {
  public:
    SkillFurniture(
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
      const float fire_posZ,
      Skill * skill
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
      ),
      skill(skill)
    {}
    SkillFurniture(
      SkillFurniture * furniture,
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
      skill(furniture->skill)
    {}
    SkillFurniture(SkillFurniture * furniture, MathUtil::Vector3i offset, int64_t rotation):
      ActivableFurniture(furniture, offset, rotation),
      skill(furniture->skill)
    {}
    bool getUnwalkable();
    bool getOpaque();
    bool getSolid();
    int32_t getLight();
    void activate(Character * user, bool remote);

  private:
    Skill * skill;
};

#endif // _SKILL_FURNITURE_H_
