#ifndef _CHANNELED_SKILL_H_
#define _CHANNELED_SKILL_H_

#include "data/skills/PseudoSkill.h"

class ChanneledSkill : public PseudoSkill {
  public:
    ChanneledSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      std::list<Effect *> effects,
      PseudoSkill * real_skill
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, effects),
      real_skill(real_skill)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range);
    int32_t getPower();
    int32_t getDamageFromType(int32_t dammage_type, int32_t overcharge_power);
    float getDamageReductionFromType(int32_t dammage_type, int32_t overcharge_power);
    PseudoSkill * getRealSkill();
  private:
    PseudoSkill * real_skill;
};

#endif // _CHANNELED_SKILL_H_
