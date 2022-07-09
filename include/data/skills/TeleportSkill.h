#ifndef _TELEPORT_SKILL_H_
#define _TELEPORT_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TeleportSkill : public PseudoSkill {
  using PseudoSkill::PseudoSkill;
  public:
    void activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge_power, int overcharge_duration, int overcharge_area, int map_id = 0, int x = 0, int y = 0);
    int getPower();
};

#endif // _TELEPORT_SKILL_H_
