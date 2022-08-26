#include "data/skills/TeleportSkill.h"

void TeleportSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {
  if(target != nullptr && std::max(abs(target->getX()) - x, abs(target->getY() - y)) <= range * overcharge_range) {
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
  } else if(owner != nullptr && std::max(abs(owner->getX()) - x, abs(owner->getY() - y)) <= range * overcharge_range) {
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
}

bool TeleportSkill::canCast(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {
  return true;
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
