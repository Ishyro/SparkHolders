#include "data/skills/TeleportSkill.h"

#include "util/MapUtil.h"

void TeleportSkill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  // Target 1 will always be a Character
  // other may be owner
  Character * other = adventure->getCharacter(target->id);
  // Target 2 will always be a Coordinate
  // maybe swap character teleport in the future ?
  switch(apparition_type) {
    case APPARITION_SOFT:
      adventure->softMoveCharacterToMap(other, MapUtil::makeVector3(target->x, target->y, target->z));
      break;
    case APPARITION_HARD:
      adventure->hardMoveCharacterToMap(other, MapUtil::makeVector3(target->x, target->y, target->z));
      break;
    default:
      ;
  }
}

// TODO
bool TeleportSkill::canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  // Target 1 will always be a Character
  // other may be owner
  Character * other = adventure->getCharacter(target->id);
  if(MapUtil::distance(owner->getCoord(), MapUtil::makeVector3(target->x, target->y, target->z)) > range * overcharge_range) {
    return false;
  }
  switch(movement_type) {
    case MOVEMENT_TELEPORT:
      if(range * overcharge_range >= MapUtil::distance(owner->getCoord(), MapUtil::makeVector3(target->next->x, target->next->y, target->next->z))) {
        return true;
      }
      else {
        return false;
      }
      /*
      TODO
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
      */
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
