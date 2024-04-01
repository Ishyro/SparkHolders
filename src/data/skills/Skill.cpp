#include "data/Map.h"

#include "data/skills/Skill.h"

#include "data/skills/BlockSwapSkill.h"

#include "util/String.h"

void Skill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range) {
  owner->payMana(getManaCost(overcharge_power, overcharge_duration, overcharge_range));
  for(PseudoSkill * skill : skills) {
    skill->activate(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, range);
  }
}
bool Skill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range) {
  if(getManaCost(overcharge_power, overcharge_duration, overcharge_range) > owner->getChanneledMana()) {
    return false;
  }
  for(PseudoSkill * skill : skills) {
    if(!skill->canCast(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, range)) {
      return false;
    }
  }
  return true;
}

int32_t Skill::getManaCost(int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range) {
  int32_t mana_cost = 0;
  for(PseudoSkill * skill : skills) {
    mana_cost += skill->getManaCost(overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range);
  }
  return mana_cost;
}

int32_t Skill::getPower() {
  int32_t power = 0;
  for(PseudoSkill * skill : skills) {
    power += skill->getPower();
  }
  return power;
}

int32_t Skill::getDamageFromType(int32_t damage_type, int32_t overcharge_power) {
  int32_t damage = 0;
  for(PseudoSkill * s : skills) {
    damage += s->getDamageFromType(damage_type, overcharge_power);
  }
  return damage;
}

float Skill::getDamageReductionFromType(int32_t damage_type, int32_t overcharge_power) {
  float reduction = 0.;
  for(PseudoSkill * s : skills) {
    reduction += s->getDamageReductionFromType(damage_type, overcharge_power);
  }
  return reduction;
}

Block * Skill::isEatingSkill() {
  bool is_eating_skill = false;
  Block * target = nullptr;
  for(PseudoSkill * s : skills) {
    if(s->skill_type == SKILL_TILE_SWAP) {
      target = ( (BlockSwapSkill *) s)->getCurrentBlock();
    }
    if(s->skill_type == SKILL_SIMPLE) {
      for(Effect * effect : s->effects) {
        if(effect->type == EFFECT_SATIETY) {
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


int32_t Skill::isTeleportSkill() {
  for(PseudoSkill * s : skills) {
    if(s->skill_type == SKILL_TELEPORT) {
      return range;
    }
  }
  return 0;
}
