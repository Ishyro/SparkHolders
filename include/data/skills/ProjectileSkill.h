#ifndef _PROJECTILESKILL_H_
#define _PROJECTILESKILL_H_

#include "data/skills/PseudoSkill.h"

class ProjectileSkill : public PseudoSkill {
  public:
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
  private:
    Projectile * projectile;
};

#endif // _PROJECTILESKILL_H_