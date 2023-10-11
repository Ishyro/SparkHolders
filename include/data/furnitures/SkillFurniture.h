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
      const bool untraversable,
      const bool opaque,
      const bool solid,
      const int light,
      Skill * skill
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
    void activate(Character * user, bool remote);

  private:
    Skill * skill;
};

#endif // _SKILL_FURNITURE_H_
