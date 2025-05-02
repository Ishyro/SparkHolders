#include "data/skills/Skill.h"

#include "data/skills/BlockSwapSkill.h"

#include "util/String.h"

void Skill::activate(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t mana_spent) {
  float overcharge_power;
  float overcharge_duration;
  float overcharge_range;
  computeOvercharges(mana_spent, overcharge_power, overcharge_duration, overcharge_range);
  bool toggle_state = true;
  if(toggle) {
    toggle_state = owner->setToggled(this);
  }
  owner->payMana(mana_spent);
  for(PseudoSkill * skill : skills) {
    skill->activate(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, range, toggle_state);
  }
}

bool Skill::canCast(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t mana_spent) {
  // can always cancel a toggled spell
  if(owner->getToggled(this)) {
    return true;
  }
  float overcharge_power;
  float overcharge_duration;
  float overcharge_range;
  computeOvercharges(mana_spent, overcharge_power, overcharge_duration, overcharge_range);
  if(getManaCost() > owner->getChanneledMana() || mana_spent > owner->getChanneledMana()) {
    return false;
  }
  for(PseudoSkill * skill : skills) {
    if(!skill->canCast(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, range)) {
      return false;
    }
  }
  return true;
}

void Skill::computeOvercharges(int32_t mana_spent, float & overcharge_power, float & overcharge_duration, float & overcharge_range) {
  // TODO
  overcharge_power = 1;
  overcharge_duration = 1;
  overcharge_range = 1;
}

int32_t Skill::getManaCost() {
  int32_t mana_cost = 0;
  for(PseudoSkill * skill : skills) {
    mana_cost += skill->mana_cost;
  }
  return mana_cost;
}

float Skill::getTime(Character * owner) {
  int32_t scaling_type = SKILL_SCALE_NONE;
  for(PseudoSkill * pseudo_skill : skills) {
    if(pseudo_skill->scaling_type != SKILL_SCALE_NONE) {
      if(scaling_type == SKILL_SCALE_NONE) {
        scaling_type = pseudo_skill->scaling_type;
      }
      else {
        // both scaling is max scalling
        scaling_type = SKILL_SCALE_BOTH_WEAPONS;
        break;
      }
    }
  }
  switch(scaling_type) {
    case SKILL_SCALE_NONE:
      return time;
    case SKILL_SCALE_MAIN_WEAPON:
      return owner->getGear()->getWeapon_1()->strike_time;
    case SKILL_SCALE_SUB_WEAPON:
      return owner->getGear()->getWeapon_2()->strike_time;
    case SKILL_SCALE_BOTH_WEAPONS:
      return owner->getGear()->getWeapon_1()->strike_time + owner->getGear()->getWeapon_2()->strike_time;
    default:
      return time;
  }
}

float Skill::getPower() {
  float power = 0;
  for(PseudoSkill * skill : skills) {
    power += skill->getPower();
  }
  return power;
}

float Skill::getDamageFromType(int32_t damage_type, Character * owner, float overcharge) {
  float damage = 0;
  for(PseudoSkill * s : skills) {
    damage += s->getDamageFromType(damage_type, owner, overcharge);
  }
  return damage;
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
        if(effect->type == EFFECT_HUNGER) {
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


float Skill::isTeleportSkill() {
  for(PseudoSkill * s : skills) {
    if(s->skill_type == SKILL_TELEPORT) {
      return range;
    }
  }
  return 0.F;
}
