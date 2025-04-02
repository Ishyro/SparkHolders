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
    const int32_t level;
    const int32_t school;
    const std::string attributes;
    const int32_t target_type;
    const bool instant;
    const bool blockable;
    const bool toggle;
    const int32_t overcharge_power_type;
    const int32_t overcharge_duration_type;
    const int32_t overcharge_range_type;
    const float range;
    const float time;
    const std::list<PseudoSkill *> skills;
    Skill(
      std::string name,
      int32_t level,
      int32_t school,
      std::string attributes,
      int32_t target_type,
      bool instant,
      bool blockable,
      bool toggle,
      int32_t overcharge_power_type,
      int32_t overcharge_duration_type,
      int32_t overcharge_range_type,
      float range,
      float time,
      std::list<PseudoSkill *> skills
    ):
      name(name),
      level(level),
      school(school),
      attributes(attributes),
      target_type(target_type),
      instant(instant),
      blockable(blockable),
      toggle(toggle),
      overcharge_power_type(overcharge_power_type),
      overcharge_duration_type(overcharge_duration_type),
      overcharge_range_type(overcharge_range_type),
      range(range),
      time(time),
      skills(skills)
    {}

    void activate(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t mana_spent, bool blocked);
    bool canCast(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t mana_spent);
    void computeOvercharges(int32_t mana_spent, float & overcharge_power, float & overcharge_duration, float & overcharge_range);
    int32_t getManaCost();
    float getTime(Character * owner);
    float getPower();
    float getDamageFromType(int32_t dammage_type, Character * owner, float overcharge);
    Block * isEatingSkill();
    float isTeleportSkill();
};

#endif // _SKILL_H_
