#include "data/skills/ChanneledSkill.h"

void ChanneledSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {}

bool ChanneledSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  return true;
}

int32_t ChanneledSkill::getPower() {
  return 0;
}

int32_t ChanneledSkill::getDamageFromType(int32_t damage_type, int32_t overcharge_power) {
  int32_t damage = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type);
    }
  }
  return damage;
}

float ChanneledSkill::getDamageReductionFromType(int32_t damage_type, int32_t overcharge_power) {
  float reduction = 0.;
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_REDUCTION) {
      reduction += e->getDamageReductionFromType(damage_type);
    }
  }
  return reduction;
}

PseudoSkill * ChanneledSkill::getRealSkill() { return real_skill; }
