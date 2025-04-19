#ifndef _SIMPLE_SKILL_H_
#define _SIMPLE_SKILL_H_

#include "data/skills/PseudoSkill.h"

class SimpleSkill : public PseudoSkill {
  public:
    const int32_t hit_order;
    const float status_power;
    const float pierce_power;
    const MathUtil::Vector3 size;
    SimpleSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      int32_t scaling_type,
      std::array<float, DAMAGE_TYPE_NUMBER> damage_multipliers,
      std::list<Effect *> effects,
      int32_t hit_order,
      float status_power,
      float pierce_power
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, scaling_type, damage_multipliers, effects),
      hit_order(hit_order),
      status_power(status_power),
      pierce_power(pierce_power),
      size(0.F, 0.F, 0.F)
    {}
    SimpleSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      int32_t scaling_type,
      std::array<float, DAMAGE_TYPE_NUMBER> damage_multipliers,
      std::list<Effect *> effects,
      int32_t hit_order,
      float status_power,
      float pierce_power,
      float sizeX,
      float sizeY,
      float sizeZ
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, scaling_type, damage_multipliers, effects),
      hit_order(hit_order),
      status_power(status_power),
      pierce_power(pierce_power),
      size(sizeX, sizeY, sizeZ)
    {}
    void activate(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state);
    bool canCast(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range);
    float getPower();
    float getDamageFromType(int32_t dammage_type, Character * owner, float overcharge);
};

#endif // _SIMPLE_SKILL_H_
