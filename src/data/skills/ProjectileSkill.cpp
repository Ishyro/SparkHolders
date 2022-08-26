#include "data/skills/ProjectileSkill.h"

#include "utils/String.h"

void ProjectileSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {
  int realDamages[DAMAGE_TYPE_NUMBER];
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    realDamages[damage_type] = projectile->getDamageFromType(damage_type) * overcharge_power;
  }
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    for(Effect * effect : effects) {
      realDamages[damage_type] += effect->getDamageFromType(damage_type) * overcharge_power;
    }
  }
  int proj_x = owner->getX();
  int proj_y = owner->getY();
  switch(owner->getOrientation()) {
    case NORTH:
      proj_y++;
      break;
    case NORTH_EAST:
      proj_y++;
      proj_x++;
      break;
    case EAST:
      proj_x++;
      break;
    case SOUTH_EAST:
      proj_y--;
      proj_x++;
      break;
    case SOUTH:
      proj_y--;
      break;
    case SOUTH_WEST:
      proj_y--;
      proj_x--;
      break;
    case WEST:
      proj_x--;
      break;
    case NORTH_WEST:
      proj_y++;
      proj_x--;
      break;
  }
  Projectile * to_add = new Projectile(projectile, realDamages, owner->getCurrentMapId(), proj_x, proj_y, x, y, (Character *) target, owner, owner->getOrientation(), overcharge_power, overcharge_duration, overcharge_range);
  adventure->getWorld()->getMap(owner->getCurrentMapId())->addProjectile(to_add);
}

bool ProjectileSkill::canCast(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {
  return true;
}

int ProjectileSkill::getPower() {
  int power = projectile->getRawDamage();
  for(Effect * effect : effects) {
    if(effect->power != 0) {
      power += effect->power;
    } else {
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
