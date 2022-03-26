#ifndef _PROJECTILESKILL_H_
#define _PROJECTILESKILL_H_

#include "data/skills/PseudoSkill.h"

class ProjectileSkill : public PseudoSkill {
  public:
    ProjectileSkill(std::string name):PseudoSkill(name) {
      // TODO XML_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
    void desactivate(Character * owner, Character * target, Adventure * adventure);
  private:
    Projectile * projectile;
};

#endif // _PROJECTILESKILL_H_
