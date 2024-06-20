#include "data/skills/ProjectileSkill.h"

#include "util/String.h"
#include "util/MathUtil.h"

void ProjectileSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  std::array<int32_t, DAMAGE_TYPE_NUMBER> realDamages;
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

bool ProjectileSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  return true;
}

int32_t ProjectileSkill::getPower() {
  int32_t power = projectile->getRawDamage();
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

int32_t ProjectileSkill::getDamageFromType(int32_t damage_type, int32_t overcharge_power) {
  int32_t damage = projectile->getDamageFromType(damage_type) * overcharge_power;
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type) * overcharge_power;
    }
  }
  return damage;
}

float ProjectileSkill::getDamageReductionFromType(int32_t damage_type, int32_t overcharge_power) {
  return 0.;
}

Projectile * ProjectileSkill::getProjectile() { return projectile; }
