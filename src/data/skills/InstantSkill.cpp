#include "data/skills/InstantSkill.h"

void InstantSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {}

int InstantSkill::getPower() {
  return 0;
}

int InstantSkill::getDamageFromType(int damage_type, int overcharge_power) {
  int damage = 0;
  for(Effect * e : effects) {
    if(e->type == DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type);
    }
  }
  return damage;
}

float InstantSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  float reduction = 0.;
  for(Effect * e : effects) {
    if(e->type == DAMAGE_REDUCTION) {
      reduction += e->getDamageReductionFromType(damage_type);
    }
  }
  return reduction;
}
