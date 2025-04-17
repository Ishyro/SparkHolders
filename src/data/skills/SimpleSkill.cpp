#include "data/skills/SimpleSkill.h"

#include "data/items/Gear.h"
#include "data/Region.h"

void SimpleSkill::activate(
  Character * owner,
  MathUtil::Target * target,
  Adventure * adventure,
  int32_t overcharge_power_type,
  int32_t overcharge_duration_type,
  int32_t overcharge_range_type,
  float overcharge_power,
  float overcharge_duration,
  float overcharge_range,
  int32_t range,
  bool toggle_state
) {
  if(target_type == TARGET_ORIENTATION) {
    Attack * attack = new Attack();
    attack->owner = owner;
    attack->status_power = status_power;
    MathUtil::Vector3 h_size = size * overcharge_range;
    switch(scaling_type) {
      case SKILL_SCALE_NONE:
        break;
      case SKILL_SCALE_MAIN_WEAPON:
        h_size = h_size + owner->getGear()->getWeapon_1()->range * overcharge_range;
        attack->status_power *= owner->getStatusPower();
        break;
      case SKILL_SCALE_SUB_WEAPON:
        h_size = h_size + owner->getGear()->getWeapon_2()->range * overcharge_range;
        attack->status_power *= owner->getStatusPower();
        break;
    }
    attack->hitbox = new MathUtil::HitboxOBB(HITBOX_OBB, owner->getCoord(), h_size.x, h_size.y, h_size.z);
    for(HitboxOwner * hit : owner->getRegion()->sortHitboxes(attack)) {
      if(hit->type == HITBOX_OWNER_FURNITURE) {
        break;
      }
      else if(hit->type == HITBOX_OWNER_CHARACTER) {
        hit->owner->receiveDamage(attack->damages, owner, attack->status_power);
        for(Effect * effect : effects) {
          Effect * to_add = new Effect(effect, overcharge_power, overcharge_duration, scaling_type, damage_multipliers, owner);
          if(toggle_state) {
            to_add->activate(hit->owner);
          }
          else {
            hit->owner->removeSimilarEffect(to_add);
          }
        }
      }
    }
  }
  else {
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
            MathUtil::Target * iter = target->next;
            for(iter = target->next; iter != nullptr; iter = iter->next) {
              to_add->activate(iter->character);
            }
          }
          else {
            MathUtil::Target * iter = target->next;
            for(iter = target->next; iter != nullptr; iter = iter->next) {
              iter->character->removeSimilarEffect(to_add);
            }
          }
          break;
        //TODO
        default:
          break;
      }
    }
  }
  delete target;
}

bool SimpleSkill::canCast(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {
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
