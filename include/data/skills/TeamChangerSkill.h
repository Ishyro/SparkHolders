#ifndef _TEAM_CHANGER_SKILL_H_
#define _TEAM_CHANGER_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TeamChangerSkill : public PseudoSkill {
  using PseudoSkill::PseudoSkill;
  public:
    void activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
};

#endif // _TEAM_CHANGER_SKILL_H_
