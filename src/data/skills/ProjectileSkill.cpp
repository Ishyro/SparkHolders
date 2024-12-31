#include "data/skills/ProjectileSkill.h"

#include "data/items/Gear.h"

#include "util/String.h"
#include "util/MathUtil.h"

void ProjectileSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state) {
  std::array<float, DAMAGE_TYPE_NUMBER> realDamages;
  for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    realDamages[damage_type] = projectile->getDamageFromType(damage_type) * overcharge_power;
  }
  for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    for(Effect * effect : effects) {
      realDamages[damage_type] += effect->getDamageFromType(damage_type) * overcharge_power;
    }
  }
  Projectile * to_add = new Projectile(
    projectile,
    realDamages,
    adventure->getWorld(),
    0,
    target,
    owner,
    overcharge_power,
    overcharge_duration,
    overcharge_range,
    true
  );
  adventure->getWorld()->getMap(0)->addProjectile(to_add);
}

bool ProjectileSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {
  return true;
}

float ProjectileSkill::getPower() {
  float power = projectile->getRawDamage();
  for(Effect * effect : effects) {
    if(effect->power != 0) {
      power += effect->power;
    }
    else {
      power += effect->getRawDamage();
    }
  }
  return power;
}

float ProjectileSkill::getDamageFromType(int32_t damage_type, Character * owner, float overcharge) {
  float damage = projectile->getDamageFromType(damage_type);
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type);
    }
  }
  switch(scaling_type) {
    case SKILL_SCALE_NONE:
      return damage * overcharge;
    case SKILL_SCALE_MAIN_WEAPON:
      return (damage + owner->getGear()->getWeapon_1()->getDamageFromType(damage_type) + (projectile->getDamageFromType(DAMAGE_PHYSICAL) + owner->getGear()->getWeapon_1()->getDamageFromType(DAMAGE_PHYSICAL)) * damage_multipliers[damage_type]) * overcharge;
    case SKILL_SCALE_SUB_WEAPON:
      return (damage + owner->getGear()->getWeapon_2()->getDamageFromType(damage_type) + (projectile->getDamageFromType(DAMAGE_PHYSICAL) + owner->getGear()->getWeapon_2()->getDamageFromType(DAMAGE_PHYSICAL)) * damage_multipliers[damage_type]) * overcharge;
  }
}

Projectile * ProjectileSkill::getProjectile() { return projectile; }
