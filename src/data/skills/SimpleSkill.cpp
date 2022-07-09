#include "data/skills/SimpleSkill.h"

void SimpleSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge_power, int overcharge_duration, int overcharge_area, int map_id, int x, int y) {
  for(Effect * effect : effects) {
    Effect * to_add = new Effect(effect, overcharge_power, overcharge_duration);
    to_add->activate(target);
  }
}

int SimpleSkill::getPower() {
  int power = 0;
  for(Effect * effect : effects) {
    if(effect->power != 0) {
      power += effect->power;
    } else {
      power += effect->getRawDamage();
    }
  }
  return power;
}
