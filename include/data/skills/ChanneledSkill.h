#ifndef _CHANNELED_SKILL_H_
#define _CHANNELED_SKILL_H_

#include "data/skills/PseudoSkill.h"

class ChanneledSkill : public PseudoSkill {
  public:
    ChanneledSkill(
      std::string name,
      int skill_type,
      int target_type,
      int mana_cost,
      std::list<Effect *> effects,
      PseudoSkill * real_skill
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, effects),
      real_skill(real_skill)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
    PseudoSkill * getRealSkill();
  private:
    PseudoSkill * real_skill;
};

#endif // _CHANNELED_SKILL_H_
