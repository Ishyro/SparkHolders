#include "data/skills/ShieldSkill.h"

#include "data/items/Gear.h"

void ShieldSkill::activate(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state) {
  if(toggle_state) {
    int32_t shield_type = SHIELD_PHYSICAL;
    float hp = getPower() * overcharge_power;
    MathUtil::Vector3 shield_size = size * overcharge_range;
    switch(scaling_type) {
      case SKILL_SCALE_NONE:
        break;
      case SKILL_SCALE_MAIN_WEAPON:
        hp += owner->getGear()->getWeapon_1()->getDurability() * overcharge_power;
        shield_size = shield_size + owner->getGear()->getWeapon_1()->range * overcharge_range;
        break;
      case SKILL_SCALE_SUB_WEAPON:
        hp += owner->getGear()->getWeapon_2()->getDurability() * overcharge_power;
        shield_size = shield_size + owner->getGear()->getWeapon_2()->range * overcharge_range;
    }
    Shield * shield = owner->produceShield(shield_type, hp, shield_size);
    adventure->getWorld()->addShield(owner, shield);
  }
  else {
    Shield * shield = owner->stopShield();
    adventure->getWorld()->eraseShield(owner, shield);
  }
  delete target;
}

bool ShieldSkill::canCast(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {
  return true;
}

float ShieldSkill::getPower() {
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

float ShieldSkill::getDamageFromType(int32_t damage_type, Character * owner, float overcharge) {
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
    default:
      return damage * overcharge;
  }
}
