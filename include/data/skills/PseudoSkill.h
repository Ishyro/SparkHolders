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
    const int32_t scalling_type;
    const std::list<Effect *> effects;
    PseudoSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      int32_t scalling_type,
      std::array<float, DAMAGE_TYPE_NUMBER> damage_multipliers,
      std::list<Effect *> effects
    ):
      name(name),
      skill_type(skill_type),
      target_type(target_type),
      mana_cost(mana_cost),
      scalling_type(scalling_type),
      effects(effects)
    {
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damage_multipliers[i] = damage_multipliers[i];
      }
    }

    virtual void activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state) = 0;
    virtual bool canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) = 0;
    virtual float getPower() = 0;
    virtual float getDamageFromType(int32_t dammage_type, Character * owner, float overcharge) = 0;
  protected:
    std::array<float, DAMAGE_TYPE_NUMBER> damage_multipliers;
};

#endif // _PSEUDO_SKILL_H_
