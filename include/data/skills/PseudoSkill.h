#ifndef _PSEUDO_SKILL_H_
#define _PSEUDO_SKILL_H_

#include <list>
#include <cmath>

#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Effect.h"
#include "data/Map.h"
#include "data/World.h"

#include "Values.h"

class PseudoSkill {
  public:
    const std::string name;
    const int skill_type;
    const int target_type;
    const int mana_cost;
    const std::list<Effect *> effects;
    const int power;
    PseudoSkill(
      std::string name,
      int skill_type,
      int target_type,
      int mana_cost,
      std::list<Effect *> effects
    ):
      name(name),
      skill_type(skill_type),
      target_type(target_type),
      mana_cost(mana_cost),
      effects(effects),
      power(0)
    {}
    PseudoSkill(
      std::string name,
      int skill_type,
      int target_type,
      int mana_cost,
      std::list<Effect *> effects,
      int power
    ):
      name(name),
      skill_type(skill_type),
      target_type(target_type),
      mana_cost(mana_cost),
      effects(effects),
      power(power)
    {}

    virtual void activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge, int map_id = 0, int x = 0, int y = 0) = 0;
    virtual int getPower() = 0;
    int getManaCost(int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge);
    std::string to_string();
    static PseudoSkill * from_string(std::string to_read);
};

#endif // _PSEUDO_SKILL_H_
