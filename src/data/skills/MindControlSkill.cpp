#include "data/skills/MindControlSkill.h"

void MindControlSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {}

bool MindControlSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  return true;
}

int32_t MindControlSkill::getPower() {
  return 0;
}

int32_t MindControlSkill::getDamageFromType(int32_t damage_type, int32_t overcharge_power) {
  return 0;
}

float MindControlSkill::getDamageReductionFromType(int32_t damage_type, int32_t overcharge_power) {
  return 0.;
}
