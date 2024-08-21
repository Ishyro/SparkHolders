#include "data/skills/SimpleSkill.h"

#include "data/items/Gear.h"

void SimpleSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state) {
  for(Effect * effect : effects) {
    Effect * to_add = new Effect(effect, overcharge_power, overcharge_duration);
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
          to_add->activate(adventure->getCharacter(target->id));
        }
        else {
          adventure->getCharacter(target->id)->removeSimilarEffect(to_add);
        }
        break;
      //TODO
      default:
        break;
    }
  }
  Effect * to_add;
  std::array<float, DAMAGE_TYPE_NUMBER> damage_reductions;
  switch(scalling_type) {
    case SKILL_SCALE_NONE:
      break;
    case SKILL_SCALE_MAIN_WEAPON:
      for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER - 1; damage_type++) {
        damage_reductions[damage_type] = owner->getGear()->getWeapon_1()->getDamageReductionFromType(damage_type);
      }
      to_add = new Effect(name, ++effect::id_cpt, 1, "", EFFECT_BLOCKING, DURATION_INFINITE, 0.F, -1, damage_reductions);
      if(toggle_state) {
        to_add->activate(adventure->getCharacter(target->id));
      }
      else {
        adventure->getCharacter(target->id)->removeSimilarEffect(to_add);
      }
      break;
    case SKILL_SCALE_SUB_WEAPON:
      for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER - 1; damage_type++) {
        damage_reductions[damage_type] = owner->getGear()->getWeapon_2()->getDamageReductionFromType(damage_type);
      }
      to_add = new Effect(name, ++effect::id_cpt, 1, "", EFFECT_BLOCKING, DURATION_INFINITE, 0.F, -1, damage_reductions);
      if(toggle_state) {
        to_add->activate(adventure->getCharacter(target->id));
      }
      else {
        adventure->getCharacter(target->id)->removeSimilarEffect(to_add);
      }
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
  switch(scalling_type) {
    case SKILL_SCALE_NONE:
      return damage * overcharge;
    case SKILL_SCALE_MAIN_WEAPON:
      return (damage + owner->getGear()->getWeapon_1()->getDamageFromType(damage_type) + owner->getGear()->getWeapon_1()->getDamageFromType(DAMAGE_PHYSICAL) * damage_multipliers[damage_type]) * overcharge;
    case SKILL_SCALE_SUB_WEAPON:
      return (damage + owner->getGear()->getWeapon_2()->getDamageFromType(damage_type) + owner->getGear()->getWeapon_2()->getDamageFromType(DAMAGE_PHYSICAL) * damage_multipliers[damage_type]) * overcharge;
  }
}
