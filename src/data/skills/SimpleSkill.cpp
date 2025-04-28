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
  if(target_type == TARGET_FRONT) {
    Attack * attack = new Attack();
    attack->owner = owner;
    attack->hit_order = hit_order;
    attack->status_power = status_power;
    attack->pierce_power = pierce_power;
    attack->damages = std::array<float, DAMAGE_TYPE_NUMBER>();
    for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
      attack->damages[i] = getDamageFromType(i, owner, overcharge_power);
    }
    MathUtil::Vector3 h_size = size * overcharge_range;
    switch(scaling_type) {
      case SKILL_SCALE_NONE:
        break;
      case SKILL_SCALE_MAIN_WEAPON:
        h_size = h_size + owner->getGear()->getWeapon_1()->range * overcharge_range;
        attack->status_power *= owner->getStatusPower();
        attack->pierce_power += owner->getGear()->getWeapon_1()->pierce_power;
        break;
      case SKILL_SCALE_SUB_WEAPON:
        h_size = h_size + owner->getGear()->getWeapon_2()->range * overcharge_range;
        attack->status_power *= owner->getStatusPower();
        attack->pierce_power += owner->getGear()->getWeapon_2()->pierce_power;
        break;
    }
    //target->coord.x += 90.F;
    attack->hitbox = new MathUtil::HitboxOBB(HITBOX_OBB, owner->getCoord(), h_size.x, h_size.y, h_size.z);
    ((MathUtil::HitboxOBB *) attack->hitbox)->applyMove(owner->getCoord(), MathUtil::getDirectionFromOrientation(target->coord));
    std::array<float, DAMAGE_TYPE_NUMBER> current_damages = attack->damages;
    for(HitboxOwner * hit : owner->getRegion()->sortHitboxes(attack)) {
      if(hit->type == HITBOX_OWNER_CHARACTER) {
        hit->owner->receiveDamages(attack->damages, owner, attack->status_power);
        for(Effect * effect : effects) {
          Effect * to_add = new Effect(effect, overcharge_power, overcharge_duration, scaling_type, damage_multipliers, owner);
          if(toggle_state) {
            to_add->activate(hit->owner);
            hit->owner->receiveDamages(current_damages, owner, attack->status_power);
            // reduce damages according to pierce_power
            bool no_damage = true;
            for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
              current_damages[i] = attack->damages[i] * attack->pierce_power;
              if(current_damages[i] > 0) {
                no_damage = false;
              }
            }
            if(no_damage) {
              break;
            }
          }
          else {
            hit->owner->removeSimilarEffect(to_add);
          }
        }
      }
      else if(hit->type == HITBOX_OWNER_FURNITURE) {
        break;
      }
      else if(hit->type == HITBOX_OWNER_SHIELD) {
        // TODO damage to shield
        break;
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
