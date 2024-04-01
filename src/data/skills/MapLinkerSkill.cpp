#include "data/skills/MapLinkerSkill.h"

void MapLinkerSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {}

bool MapLinkerSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  return true;
}

int32_t MapLinkerSkill::getPower() {
  return 0;
}

int32_t MapLinkerSkill::getDamageFromType(int32_t damage_type, int32_t overcharge_power) {
  return 0;
}

float MapLinkerSkill::getDamageReductionFromType(int32_t damage_type, int32_t overcharge_power) {
  return 0.;
}
