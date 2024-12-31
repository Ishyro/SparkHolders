#include "data/skills/SimpleSkill.h"

#include "data/items/Gear.h"

void SimpleSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state) {
  for(Effect * effect : effects) {
    Effect * to_add = new Effect(effect, overcharge_power, overcharge_duration, scaling_type, damage_multipliers, owner);
    switch(target_type) {
      case TARGET_SELF:
        if(toggle_state) {
          to_add->activate(owner);
        }
        else {
          owner->removeSimilarEffect(to_add);
        }
        break;
      case TARGET_CHARACTER:
        if(toggle_state) {
          to_add->activate(target->character);
        }
        else {
          target->character->removeSimilarEffect(to_add);
        }
        break;
      case TARGET_MULTIPLE:
        if(toggle_state) {
          Target * iter = target->next;
          for(iter = target->next; iter != nullptr; iter = iter->next) {
            to_add->activate(iter->character);
          }
        }
        else {
          Target * iter = target->next;
          for(iter = target->next; iter != nullptr; iter = iter->next) {
            iter->character->removeSimilarEffect(to_add);
          }
        }
        break;
      //TODO
      default:
        break;
    }
    delete target;
  }
}

bool SimpleSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {
  return true;
}

float SimpleSkill::getPower() {
  float power = 0;
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

float SimpleSkill::getDamageFromType(int32_t damage_type, Character * owner, float overcharge) {
  float damage = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type);
    }
  }
  switch(scaling_type) {
    case SKILL_SCALE_NONE:
      return damage * overcharge;
    case SKILL_SCALE_MAIN_WEAPON:
      return (damage + owner->getGear()->getWeapon_1()->getDamageFromType(damage_type) + owner->getGear()->getWeapon_1()->getDamageFromType(DAMAGE_PHYSICAL) * damage_multipliers[damage_type]) * overcharge;
    case SKILL_SCALE_SUB_WEAPON:
      return (damage + owner->getGear()->getWeapon_2()->getDamageFromType(damage_type) + owner->getGear()->getWeapon_2()->getDamageFromType(DAMAGE_PHYSICAL) * damage_multipliers[damage_type]) * overcharge;
  }
}
