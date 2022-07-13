#ifndef _RESURRECTION_SKILL_H_
#define _RESURRECTION_SKILL_H_

#include "data/skills/PseudoSkill.h"

class ResurrectionSkill : public PseudoSkill {
  using PseudoSkill::PseudoSkill;
  public:
    void activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge_power, int overcharge_duration, int overcharge_area, int map_id, int x, int y, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
};

#endif // _RESURRECTION_SKILL_H_
