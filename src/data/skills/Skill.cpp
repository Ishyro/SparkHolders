#include "data/skills/Skill.h"

void Skill::activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id, long x, long y) {
  current_mana_cost = getManaCost(overcharge);
  this->overcharge = overcharge;
  if (cast_type == TOGGLE) {
    active = true;
  }
  for (auto effect : effects) {
    effect->activate(owner, target, adventure, overcharge, map_id, x, y);
  }
}

void Skill::desactivate(Character * owner, Character * target, Adventure * adventure) {
  current_mana_cost = 0L;
  active = false;
  for (auto effect : effects) {
    effect->desactivate(owner, target, adventure);
  }
  overcharge = 0L;
}

long Skill::getManaCost(long overcharge) {
  long mana_cost = 0L;
  for (auto effect : effects) {
    mana_cost += effect->getManaCost(overcharge);
  }
}

long Skill::drain() {
  if (cast_type == TOGGLE && active) {
    return current_mana_cost;
  }
  else {
    return 0L;
  }
}
