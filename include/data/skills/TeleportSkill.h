#ifndef _TELEPORT_SKILL_H_
#define _TELEPORT_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TeleportSkill : public PseudoSkill {
  public:
    const int32_t apparition_type;
    const int32_t movement_type;
    TeleportSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      int32_t scaling_type,
      std::array<float, DAMAGE_TYPE_NUMBER> damage_multipliers,
      std::list<Effect *> effects,
      int32_t apparition_type,
      int32_t movement_type
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, scaling_type, damage_multipliers, effects),
      apparition_type(apparition_type),
      movement_type(movement_type)
    {}
    void activate(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state);
    bool canCast(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range);
    float getPower();
    float getDamageFromType(int32_t dammage_type, Character * owner, float overcharge);
};

#endif // _TELEPORT_SKILL_H_
