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
    const int level;
    const std::string attributes;
    const int target_type;
    const bool is_instant;
    const int overcharge_power_type;
    const int overcharge_duration_type;
    const int overcharge_range_type;
    const int range;
    const int time;
    const std::list<PseudoSkill *> skills;
    Skill(
      std::string name,
      int level,
      std::string attributes,
      int target_type,
      bool is_instant,
      int overcharge_power_type,
      int overcharge_duration_type,
      int overcharge_range_type,
      int range,
      int time,
      std::list<PseudoSkill *> skills
    ):
      name(name),
      level(level),
      attributes(attributes),
      target_type(target_type),
      is_instant(is_instant),
      overcharge_power_type(overcharge_power_type),
      overcharge_duration_type(overcharge_duration_type),
      overcharge_range_type(overcharge_range_type),
      range(range),
      time(time),
      skills(skills)
    {}

    void activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id = 0, int x = 0, int y = 0);
    bool canCast(Character * owner, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y);
    int getManaCost(int overcharge_power, int overcharge_duration, int overcharge_range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
    Tile * isEatingSkill();
    int isTeleportSkill();
    std::string to_string();
    static Skill * from_string(std::string to_read);
};

#endif // _SKILL_H_
