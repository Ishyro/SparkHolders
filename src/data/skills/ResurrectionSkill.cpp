#include "data/skills/ResurrectionSkill.h"

void ResurrectionSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {}

bool ResurrectionSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  return true;
}

int32_t ResurrectionSkill::getPower() {
  return 0;
}

int32_t ResurrectionSkill::getDamageFromType(int32_t damage_type, int32_t overcharge_power) {
  return 0;
}

float ResurrectionSkill::getDamageReductionFromType(int32_t damage_type, int32_t overcharge_power) {
  return 0.;
}
