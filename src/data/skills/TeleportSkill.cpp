#include "data/skills/TeleportSkill.h"

#include "util/MapUtil.h"

void TeleportSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {
  if(target != nullptr && MapUtil::distance(owner->getX(), owner->getY(), x, y) <= range * overcharge_range) {
    switch(apparition_type) {
      case SOFT:
        adventure->softMoveCharacterToMap(target, map_id, y, x);
        break;
      case HARD:
        adventure->hardMoveCharacterToMap(target, map_id, y, x);
        break;
      default:
        ;
    }
  }
  else if(owner != nullptr && MapUtil::distance(owner->getX(), owner->getY(), x, y) <= range * overcharge_range) {
    switch(apparition_type) {
      case SOFT:
        adventure->softMoveCharacterToMap(owner, map_id, y, x);
        break;
      case HARD:
        adventure->hardMoveCharacterToMap(owner, map_id, y, x);
        break;
      default:
        ;
    }
  }
  for(Projectile * p : adventure->getWorld()->getMap(owner->getCurrentMapId())->getProjectiles()) {
    if(p->getTarget() == owner) {
      p->setLost(true);
    }
  }
}

// TODO
bool TeleportSkill::canCast(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {
  Character * to_move = target == nullptr ? owner : target;
  switch(movement_type) {
    case TELEPORT:
      if(range * overcharge_range >= MapUtil::distance(to_move->getX(), to_move->getY(), x, y)) {
        return true;
      }
      else {
        return false;
      }
    case WALKING:
      if(!owner->isFlying()) {
        if(range * overcharge_range >= (int) MapUtil::getPathToTarget(adventure->getWorld()->getMap(to_move->getCurrentMapId()), to_move->getX(), to_move->getY(), x, y, false).size()) {
          return true;
        }
        else {
          return false;
        }
      }
    case FLYING:
      if(range * overcharge_range >= (int) MapUtil::getPathToTarget(adventure->getWorld()->getMap(to_move->getCurrentMapId()), to_move->getX(), to_move->getY(), x, y, true).size()) {
        return true;
      }
      else {
        return false;
      }
    default:
      return false;
  }
}

int TeleportSkill::getPower() {
  return 0;
}

int TeleportSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float TeleportSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}

int TeleportSkill::getApparitionType() { return apparition_type; }
void TeleportSkill::setApparitionType(int apparition_type) { this->apparition_type = apparition_type; }
int TeleportSkill::getMovementType() { return movement_type; }
void TeleportSkill::setMovementType(int movement_type) { this->movement_type = movement_type; }
