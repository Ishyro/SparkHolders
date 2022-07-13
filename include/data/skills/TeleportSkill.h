#ifndef _TELEPORT_SKILL_H_
#define _TELEPORT_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TeleportSkill : public PseudoSkill {
  using PseudoSkill::PseudoSkill;
  public:
    void activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
    int getApparitionType();
    void setApparitionType(int apparition_type);
  private:
    int apparition_type;
};

#endif // _TELEPORT_SKILL_H_
