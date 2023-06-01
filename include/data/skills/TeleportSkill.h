#ifndef _TELEPORT_SKILL_H_
#define _TELEPORT_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TeleportSkill : public PseudoSkill {
  public:
    const int apparition_type;
    const int movement_type;
    TeleportSkill(
      std::string name,
      int skill_type,
      int target_type,
      int mana_cost,
      std::list<Effect *> effects,
      int apparition_type,
      int movement_type
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, effects),
      apparition_type(apparition_type),
      movement_type(movement_type)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
};

#endif // _TELEPORT_SKILL_H_
