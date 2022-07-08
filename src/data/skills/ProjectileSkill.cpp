#include "data/skills/ProjectileSkill.h"

void ProjectileSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge, int map_id, int x, int y) {
  int overcharge_power = overcharge_power_type == NO_OVERCHARGE ? 1 : overcharge;
  int overcharge_area = overcharge_area_type == NO_OVERCHARGE ? 1 : overcharge;
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
  Projectile * to_add = new Projectile(projectile, realDamages, owner->getCurrentMapId(), proj_x, proj_y, x, y, (Character *) target, owner, owner->getOrientation(), overcharge, overcharge_area);
  adventure->getWorld()->getMap(owner->getCurrentMapId())->addProjectile(to_add);
}

int ProjectileSkill::getPower() {
  int power = 0;
  for(Effect * effect : effects) {
    if(effect->power != 0) {
      power += effect->power;
    } else {
      power += effect->getRawDamage();
    }
  }
  return power + projectile->getRawDamage();
}

void ProjectileSkill::setProjectile(Projectile * projectile) { this->projectile = projectile; }
