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
    const int32_t skill_type;
    const int32_t target_type;
    const int32_t mana_cost;
    const std::list<Effect *> effects;
    PseudoSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      std::list<Effect *> effects
    ):
      name(name),
      skill_type(skill_type),
      target_type(target_type),
      mana_cost(mana_cost),
      effects(effects)
    {}

    virtual void activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) = 0;
    virtual bool canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) = 0;
    virtual int32_t getPower() = 0;
    virtual int32_t getDamageFromType(int32_t dammage_type, int32_t overcharge_power) = 0;
    virtual float getDamageReductionFromType(int32_t dammage_type, int32_t overcharge_power) = 0;
    int32_t getManaCost(int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range);
};

#endif // _PSEUDO_SKILL_H_
