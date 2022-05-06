#include "data/skills/Skill.h"

void Skill::activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id, long x, long y) {
  owner->payMana(getManaCost(overcharge));
  for(auto effect : effects) {
    effect->activate(owner, target, adventure, overcharge, map_id, x, y);
  }
}

long Skill::getManaCost(long overcharge) {
  long mana_cost = 0L;
  for(auto effect : effects) {
    mana_cost += effect->getManaCost(overcharge);
  }
  return mana_cost;
}


std::string Skill::to_string() {
  return "";
}

Skill * Skill::from_string(std::string to_read) {
  return nullptr;
}
