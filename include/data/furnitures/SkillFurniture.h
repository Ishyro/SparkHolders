#ifndef _SKILL_FURNITURE_H_
#define _SKILL_FURNITURE_H_

#include <string>

#include "data/furnitures/ActivableFurniture.h"

#include "Values.h"

class SkillFurniture : public ActivableFurniture  {
  public:
    SkillFurniture(
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
      skill(furniture->skill)
    {}
    SkillFurniture(SkillFurniture * furniture, Map * map):
      ActivableFurniture(furniture, map),
      skill(furniture->skill)
    {}
    bool getUnwalkable();
    bool getOpaque();
    bool getSolid();
    int getLight();
    void activate(Character * user, bool remote);

  private:
    Skill * skill;
};

#endif // _SKILL_FURNITURE_H_
