#include "data/skills/TeamChangerSkill.h"

void TeamChangerSkill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {}

bool TeamChangerSkill::canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  return true;
}

int TeamChangerSkill::getPower() {
  return 0;
}

int TeamChangerSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float TeamChangerSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}
