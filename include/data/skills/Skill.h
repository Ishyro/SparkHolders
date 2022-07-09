#ifndef _SKILL_H_
#define _SKILL_H_

#include <list>

#include "Values.h"

#include "data/Adventure.h"
#include "data/Character.h"
#include "data/skills/PseudoSkill.h"

class Skill {
  public:
    const std::string name;
    const int target_type;
    const int overcharge_power_type;
    const int overcharge_duration_type;
    const int overcharge_area_type;
    const int range;
    const std::list<PseudoSkill *> skills;
    Skill(
      std::string name,
      int target_type,
      int overcharge_power_type,
      int overcharge_duration_type,
      int overcharge_area_type,
      int range,
      std::list<PseudoSkill *> skills
    ):
      name(name),
      target_type(target_type),
      overcharge_power_type(overcharge_power_type),
      overcharge_duration_type(overcharge_duration_type),
      overcharge_area_type(overcharge_area_type),
      range(range),
      skills(skills)
    {}

    void activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_area, int map_id = 0, int x = 0, int y = 0);
    int getManaCost(int overcharge_power, int overcharge_duration, int overcharge_area);
    int getPower();
    std::string to_string();
    static Skill * from_string(std::string to_read);
};

#endif // _SKILL_H_
