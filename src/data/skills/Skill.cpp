#include "data/Map.h"

#include "data/skills/Skill.h"

#include "data/skills/TileSwapSkill.h"

#include "util/String.h"

void Skill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range) {
  owner->payMana(getManaCost(overcharge_power, overcharge_duration, overcharge_range));
  for(PseudoSkill * skill : skills) {
    skill->activate(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, range);
  }
}
bool Skill::canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range) {
  // using mana beyond flow is forbidden for instant skills
  if(is_instant) {
    if(getManaCost(overcharge_power, overcharge_duration, overcharge_range) > owner->getAvaillableMana(false)) {
      return false;
    }
  }
  else {
    if(getManaCost(overcharge_power, overcharge_duration, overcharge_range) >= owner->getAvaillableMana(true)) {
      return false;
    }
  }
  for(PseudoSkill * skill : skills) {
    if(!skill->canCast(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, range)) {
      return false;
    }
  }
  return true;
}

int Skill::getManaCost(int overcharge_power, int overcharge_duration, int overcharge_range) {
  int mana_cost = 0;
  for(PseudoSkill * skill : skills) {
    mana_cost += skill->getManaCost(overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range);
  }
  return mana_cost;
}

int Skill::getPower() {
  int power = 0;
  for(PseudoSkill * skill : skills) {
    power += skill->getPower();
  }
  return power;
}

int Skill::getDamageFromType(int damage_type, int overcharge_power) {
  int damage = 0;
  for(PseudoSkill * s : skills) {
    damage += s->getDamageFromType(damage_type, overcharge_power);
  }
  return damage;
}

float Skill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  float reduction = 0.;
  for(PseudoSkill * s : skills) {
    reduction += s->getDamageReductionFromType(damage_type, overcharge_power);
  }
  return reduction;
}

Tile * Skill::isEatingSkill() {
  bool is_eating_skill = false;
  Tile * target = nullptr;
  for(PseudoSkill * s : skills) {
    if(s->skill_type == TILE_SWAP_SKILL) {
      target = ( (TileSwapSkill *) s)->getCurrentTile();
    }
    if(s->skill_type == SIMPLE_SKILL) {
      for(Effect * effect : s->effects) {
        if(effect->type == SATIETY) {
          is_eating_skill = true;
        }
      }
    }
  }
  if(is_eating_skill) {
    return target;
  }
  return nullptr;
}


int Skill::isTeleportSkill() {
  for(PseudoSkill * s : skills) {
    if(s->skill_type == TELEPORT_SKILL) {
      return range;
    }
  }
  return 0;
}
