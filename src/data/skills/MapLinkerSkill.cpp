#include "data/skills/MapLinkerSkill.h"

void MapLinkerSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {}

bool MapLinkerSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {
  return true;
}

float MapLinkerSkill::getPower() {
  return 0.F;
}

float MapLinkerSkill::getDamageFromType(int32_t damage_type, Character * owner, float overcharge) {
  return 0.F;
}

float MapLinkerSkill::getDamageReductionFromType(int32_t damage_type, float overcharge) {
  return 0.F;
}
