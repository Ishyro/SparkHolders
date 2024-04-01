#include "data/skills/SimpleSkill.h"

void SimpleSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  for(Effect * effect : effects) {
    Effect * to_add = new Effect(effect, overcharge_power, overcharge_duration);
    if(target_type != TARGET_SELF) {
      if(target->type == TARGET_CHARACTER) {
        to_add->activate(adventure->getCharacter(target->id));
      }
      else {
        // TODO
      }
    }
    else {
      to_add->activate(owner);
    }
  }
}

bool SimpleSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  return true;
}

int32_t SimpleSkill::getPower() {
  int32_t power = 0;
  for(Effect * effect : effects) {
    if(effect->power != 0) {
      power += effect->power;
    }
    else {
      power += effect->getRawDamage();
    }
  }
  return power;
}

int32_t SimpleSkill::getDamageFromType(int32_t damage_type, int32_t overcharge_power) {
  int32_t damage = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type) * overcharge_power;
    }
  }
  return damage;
}

float SimpleSkill::getDamageReductionFromType(int32_t damage_type, int32_t overcharge_power) {
  float reduction = 0.;
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_REDUCTION) {
      reduction += e->getDamageReductionFromType(damage_type) * overcharge_power;
    }
  }
  return reduction;
}
