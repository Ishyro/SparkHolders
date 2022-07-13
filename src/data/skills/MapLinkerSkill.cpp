#include "data/skills/MapLinkerSkill.h"

void MapLinkerSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {}

int MapLinkerSkill::getPower() {
  return 0;
}

int MapLinkerSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float MapLinkerSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}
