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
      effects(effects)
    {}

    virtual void activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) = 0;
    virtual bool canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) = 0;
    virtual int getPower() = 0;
    virtual int getDamageFromType(int dammage_type, int overcharge_power) = 0;
    virtual float getDamageReductionFromType(int dammage_type, int overcharge_power) = 0;
    int getManaCost(int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range);
};

#endif // _PSEUDO_SKILL_H_
