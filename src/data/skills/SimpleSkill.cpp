#include "data/skills/SimpleSkill.h"

void SimpleSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge, int map_id, int x, int y) {
  int overcharge_power = overcharge_power_type == NO_OVERCHARGE ? 1 : overcharge;
  int overcharge_duration = overcharge_duration_type == NO_OVERCHARGE ? 1 : overcharge;
  for(Effect * effect : effects) {
    Effect * to_add = new Effect(effect, overcharge_power, overcharge_duration);
    to_add->activate(target);
  }
}
