#include "data/Effect.h"
#include "data/Settings.h"

#include "util/String.h"

void Effect::activate(Character * target) {
  if(duration != 0) {
    target->addEffect(this);
  }
  else {
    switch (type) {
      // no duration means INSTANT_DURATION for these effects
      case EFFECT_HP:
        target->hpHeal(power);
        break;
      case EFFECT_MANA:
        target->manaHeal(power);
        break;
      case EFFECT_SHIELD:
        target->shieldRestore(power);
        break;
      case EFFECT_HP_MAX:
        target->incrMaxHp();
        break;
      case EFFECT_MANA_MAX:
        target->incrMaxMana();
        break;
      case EFFECT_SHIELD_MAX:
        target->incrMaxShield();
        break;
      case EFFECT_HUNGER:
        target->addHunger((float) power);
        break;
      case EFFECT_THIRST:
        target->addThirst((float) power);
        break;
      case EFFECT_STAMINA:
        target->addStamina((float) power);
        break;
      case EFFECT_SANITY:
        target->addSanity((float) power);
        break;
      case EFFECT_SOULBURNTRESHOLD:
        target->incrSoulBurnTreshold();
        break;
      case EFFECT_FLOW:
        target->incrFlow();
        break;
      case EFFECT_VISION_RANGE:
        target->incrVisionRange();
        break;
      case EFFECT_VISION_POWER:
        target->incrVisionPower();
        break;
      case EFFECT_DETECTION_RANGE:
        target->incrDetectionRange();
        break;
      case EFFECT_DAMAGE:
        target->receiveDamage(damages, owner, power);
        break;
      case EFFECT_EXPERIENCE:
        target->gainXP(power);
        break;
      // no duration means INFINITE for next effects (can be cancelled)
      default:
        target->addEffect(this);
    }
  }
}

bool Effect::tick(Character * target) {
  switch (type) {
    case EFFECT_HP:
      target->hpHeal(power);
      break;
    case EFFECT_MANA:
      target->manaHeal(power);
    case EFFECT_SHIELD:
      target->shieldRestore(power);
      break;
    case EFFECT_HUNGER:
      target->addHunger((float) power);
      break;
    case EFFECT_THIRST:
      target->addThirst((float) power);
      break;
    case EFFECT_STAMINA:
      target->addStamina((float) power);
      break;
    case EFFECT_SANITY:
      target->addSanity((float) power);
      break;
    case EFFECT_DAMAGE:
      target->receiveDamage(damages, owner, power);
      break;
    case EFFECT_EXPERIENCE:
      target->gainXP(power);
      break;
    case EFFECT_STATUS_BLEEDING: {
      float potency = 4.F * duration;
      target->receiveDamage(damages, owner, power);
      target->addHunger(-100.F / potency);
      target->addThirst(-100.F / potency);
      target->addStamina(-100.F / potency);
      break;
    }
    case EFFECT_STATUS_WEAKENED:
      target->receiveDamage(damages, owner, power);
      break;
    case EFFECT_STATUS_CONFUSED:
      target->receiveDamage(damages, owner, power);
      break;
    case EFFECT_STATUS_BURNING:
      target->receiveDamage(damages, owner, power);
      break;
    case EFFECT_STATUS_FROZEN:
      target->receiveDamage(damages, owner, power);
      break;
    case EFFECT_STATUS_SHOCKED:
      target->receiveDamage(damages, owner, power);
      break;
    case EFFECT_STATUS_POISONED:
      target->receiveDamage(damages, owner, power);
      break;
    case EFFECT_STATUS_CORRODED:
      target->receiveDamage(damages, owner, power);
      break;
    case EFFECT_STATUS_BROKEN:
      target->receiveDamage(damages, owner, power);
      break;
    case EFFECT_STATUS_DISINTEGRATED:
      target->receiveDamage(damages, owner, power);
      break;
    default:
      ;
  }
  return duration_type == DURATION_TEMPORARY && --tick_left == 0;
}

void Effect::desactivate(Character * target) {
  if(duration_type == DURATION_INFINITE || duration_type == DURATION_TEMPORARY) {
    target->removeEffect(this);
  }
}

int32_t Effect::getTickLeft() { return tick_left; }

int32_t Effect::getRawDamage() {
  int32_t power = 0;
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += damages[i];
  }
  return power;
}

int32_t Effect::getDamageFromType(int32_t damage_type) { return damages[damage_type]; }
float Effect::getDamageReductionFromType(int32_t damage_type) { return damage_reductions[damage_type]; }

std::string Effect::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, level);
  String::insert(ss, attributes);
  String::insert_int(ss, type);
  String::insert_int(ss, duration_type);
  String::insert_float(ss, power);
  String::insert_int(ss, duration);
  String::insert_int(ss, tick_left);
  if(type == EFFECT_DAMAGE || type == EFFECT_DAMAGE_BUFF) {
    for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
      String::insert_int(ss, damages[i]);
    }
  }
  if(type == EFFECT_DAMAGE_REDUCTION) {
    for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
      String::insert_float(ss, damage_reductions[i]);
    }
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

Effect * Effect::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int64_t id = String::extract_long(ss);
  int32_t level = String::extract_int(ss);
  std::string attributes = String::extract(ss);
  int32_t type = String::extract_int(ss);
  int32_t duration_type = String::extract_int(ss);
  float power = String::extract_float(ss);
  int32_t duration = String::extract_int(ss);
  int32_t tick_left = String::extract_int(ss);
  std::array<int32_t, DAMAGE_TYPE_NUMBER> damages;
  if(type == EFFECT_DAMAGE || type == EFFECT_DAMAGE_BUFF) {
    for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
      damages[i] = String::extract_int(ss);
    }
  }
  std::array<float, DAMAGE_TYPE_NUMBER> damage_reductions;
  if(type == EFFECT_DAMAGE_REDUCTION) {
    for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
      damage_reductions[i] = String::extract_float(ss);
    }
  }
  delete ss;
  return new Effect(name, id, level, attributes, type, duration_type, power, duration, tick_left, damages, damage_reductions);
 }
