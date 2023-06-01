#ifndef _PROJECTILE_SKILL_H_
#define _PROJECTILE_SKILL_H_

#include "data/Projectile.h"

#include "data/skills/PseudoSkill.h"

class ProjectileSkill : public PseudoSkill {
  public:
    ProjectileSkill(
      std::string name,
      int skill_type,
      int target_type,
      int mana_cost,
      std::list<Effect *> effects,
      Projectile * projectile
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, effects),
      projectile(projectile)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
    Projectile * getProjectile();
  private:
    Projectile * projectile;
};

#endif // _PROJECTILE_SKILL_H_
