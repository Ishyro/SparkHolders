#include "data/skills/TeleportSkill.h"

#include "util/MathUtil.h"

void TeleportSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state) {
  // Target 1 will always be a Character
  // other may be owner
  Character * other = adventure->getCharacter(target->id);
  // Target 2 will always be a Coordinate
  // maybe swap character teleport in the future ?
  switch(apparition_type) {
    case APPARITION_SOFT:
      adventure->softMoveCharacterToMap(other, target->coord);
      break;
    case APPARITION_HARD:
      adventure->hardMoveCharacterToMap(other, target->coord);
      break;
    default:
      ;
  }
}

// TODO
bool TeleportSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {
  // Target 1 will always be a Character
  // other may be owner
  Character * other = adventure->getCharacter(target->id);
  if(MathUtil::distance(owner->getCoord(), target->coord) > range * overcharge_range) {
    return false;
  }
  switch(movement_type) {
    case MOVEMENT_TELEPORT:
      if(range * overcharge_range >= MathUtil::distance(owner->getCoord(), target->next->coord)) {
        return true;
      }
      else {
        return false;
      }
      /*
      TODO
    case WALKING:
      if(!owner->isFlying()) {
        if(range * overcharge_range >= (int32_t) MathUtil::getPathToTarget(adventure->getWorld()->getMap(to_move->getCurrentMapId()), to_move->getX(), to_move->getY(), x, y, false).size()) {
          return true;
        }
        else {
          return false;
        }
      }
    case FLYING:
      if(range * overcharge_range >= (int32_t) MathUtil::getPathToTarget(adventure->getWorld()->getMap(to_move->getCurrentMapId()), to_move->getX(), to_move->getY(), x, y, true).size()) {
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

float TeleportSkill::getPower() {
  return 0.F;
}

float TeleportSkill::getDamageFromType(int32_t damage_type, Character * owner, float overcharge) {
  return 0.F;
}
