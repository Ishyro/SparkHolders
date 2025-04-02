#ifndef _PROJECTILE_SKILL_H_
#define _PROJECTILE_SKILL_H_

#include "data/Projectile.h"

#include "data/skills/PseudoSkill.h"

class ProjectileSkill : public PseudoSkill {
  public:
    ProjectileSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      int32_t scaling_type,
      std::array<float, DAMAGE_TYPE_NUMBER> damage_multipliers,
      std::list<Effect *> effects,
      Projectile * projectile
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, scaling_type, damage_multipliers, effects),
      projectile(projectile)
    {}
    void activate(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state);
    bool canCast(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range);
    float getPower();
    float getDamageFromType(int32_t dammage_type, Character * owner, float overcharge);
    Projectile * getProjectile();
  private:
    Projectile * projectile;
};

#endif // _PROJECTILE_SKILL_H_
