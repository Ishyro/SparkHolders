#include "data/skills/SimpleSkill.h"

void SimpleSkill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  for(Effect * effect : effects) {
    Effect * to_add = new Effect(effect, overcharge_power, overcharge_duration);
    if(target_type != SELF) {
      if(target->type == CHARACTER) {
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

bool SimpleSkill::canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  return true;
}

int SimpleSkill::getPower() {
  int power = 0;
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

int SimpleSkill::getDamageFromType(int damage_type, int overcharge_power) {
  int damage = 0;
  for(Effect * e : effects) {
    if(e->type == DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type) * overcharge_power;
    }
  }
  return damage;
}

float SimpleSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  float reduction = 0.;
  for(Effect * e : effects) {
    if(e->type == DAMAGE_REDUCTION) {
      reduction += e->getDamageReductionFromType(damage_type) * overcharge_power;
    }
  }
  return reduction;
}
