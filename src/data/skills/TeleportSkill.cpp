#include "data/skills/TeleportSkill.h"

void TeleportSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge_power, int overcharge_duration, int overcharge_area, int map_id, int x, int y) {}

int TeleportSkill::getPower() {
  return 0;
}

int TeleportSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float TeleportSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}
