#include "data/skills/SummonSkill.h"

void SummonSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {}

int SummonSkill::getPower() {
  return 0;
}

int SummonSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float SummonSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}
