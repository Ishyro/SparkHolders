#include "data/skills/MindControlSkill.h"

void MindControlSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {}

bool MindControlSkill::canCast(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {
  return true;
}

int MindControlSkill::getPower() {
  return 0;
}

int MindControlSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float MindControlSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}
