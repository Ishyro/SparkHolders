#include "data/skills/ProjectileSkill.h"

#include "util/String.h"
#include "util/MapUtil.h"

void ProjectileSkill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  int realDamages[DAMAGE_TYPE_NUMBER];
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    realDamages[damage_type] = projectile->getDamageFromType(damage_type) * overcharge_power;
  }
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    for(Effect * effect : effects) {
      realDamages[damage_type] += effect->getDamageFromType(damage_type) * overcharge_power;
    }
  }
  Projectile * to_add = new Projectile(
    projectile,
    realDamages,
    adventure->getWorld(),
    owner->getCurrentMapId(),
    target,
    owner,
    overcharge_power,
    overcharge_duration,
    overcharge_range,
    true
  );
  adventure->getWorld()->getMap(owner->getCurrentMapId())->addProjectile(to_add);
}

bool ProjectileSkill::canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  return true;
}

int ProjectileSkill::getPower() {
  int power = projectile->getRawDamage();
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

int ProjectileSkill::getDamageFromType(int damage_type, int overcharge_power) {
  int damage = projectile->getDamageFromType(damage_type) * overcharge_power;
  for(Effect * e : effects) {
    if(e->type == DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type) * overcharge_power;
    }
  }
  return damage;
}

float ProjectileSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}

void ProjectileSkill::setProjectile(Projectile * projectile) { this->projectile = projectile; }
Projectile * ProjectileSkill::getProjectile() { return projectile; }
