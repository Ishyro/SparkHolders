#include "data/Race.h"

std::list<Effect *> effects;
std::list<Skill *> skills;

int Race::getBaseHp() {
  if(modifier != nullptr) {
    return baseHp + modifier->getBaseHp();
  }
  else {
    return baseHp;
  }
}

int Race::getBaseMana() {
  if(modifier != nullptr) {
    return baseMana + modifier->getBaseMana();
  }
  else {
    return baseMana;
  }
}

int Race::getBaseArmor() {
  if(modifier != nullptr) {
    return baseArmor + modifier->getBaseArmor();
  }
  else {
    return baseArmor;
  }
}

int Race::getBaseArmorMult() {
  if(modifier != nullptr) {
    return baseArmorMult + modifier->getBaseArmorMult();
  }
  else {
    return baseArmorMult;
  }
}

int Race::getBaseDamageMult() {
  if(modifier != nullptr) {
    return baseDamageMult + modifier->getBaseDamageMult();
  }
  else {
    return baseDamageMult;
  }
}

int Race::getBaseSoulBurn() {
  if(modifier != nullptr) {
    return baseSoulBurn + modifier->getBaseSoulBurn();
  }
  else {
    return baseSoulBurn;
  }
}

int Race::getBaseFlow() {
  if(modifier != nullptr) {
    return baseFlow + modifier->getBaseFlow();
  }
  else {
    return baseFlow;
  }
}

int Race::getBaseVisionRange() {
  if(modifier != nullptr) {
    return baseVisionRange + modifier->getBaseVisionRange();
  }
  else {
    return baseVisionRange;
  }
}

int Race::getBaseVisionPower() {
  if(modifier != nullptr) {
    return baseVisionPower + modifier->getBaseVisionPower();
  }
  else {
    return baseVisionPower;
  }
}

int Race::getBaseDetectionRange() {
  if(modifier != nullptr) {
    return baseDetectionRange + modifier->getBaseDetectionRange();
  }
  else {
    return baseDetectionRange;
  }
}

int Race::getHpIncr() {
  if(modifier != nullptr) {
    return hpIncr + modifier->getHpIncr();
  }
  else {
    return hpIncr;
  }
}

int Race::getManaIncr() {
  if(modifier != nullptr) {
    return manaIncr + modifier->getManaIncr();
  }
  else {
    return manaIncr;
  }
}

int Race::getArmorMultIncr() {
  if(modifier != nullptr) {
    return armorMultIncr + modifier->getArmorMultIncr();
  }
  else {
    return armorMultIncr;
  }
}

int Race::getDamageMultIncr() {
  if(modifier != nullptr) {
    return damageMultIncr + modifier->getDamageMultIncr();
  }
  else {
    return damageMultIncr;
  }
}

int Race::getSoulBurnIncr() {
  if(modifier != nullptr) {
    return soulBurnIncr + modifier->getSoulBurnIncr();
  }
  else {
    return soulBurnIncr;
  }
}

int Race::getFlowIncr() {
  if(modifier != nullptr) {
    return flowIncr + modifier->getFlowIncr();
  }
  else {
    return flowIncr;
  }
}

float Race::getSize() {
  if(modifier != nullptr) {
    return std::max(size, modifier->getSize());
  }
  else {
    return size;
  }
}

bool Race::getNeedToEat() {
  if(modifier != nullptr) {
    return need_to_eat && modifier->getNeedToEat();
  }
  else {
    return need_to_eat;
  }
}

bool Race::getCanEatFood() {
  if(modifier != nullptr) {
    return can_eat_food && modifier->getCanEatFood();
  }
  else {
    return can_eat_food;
  }
}

bool Race::getNeedToSleep() {
  if(modifier != nullptr) {
    return need_to_sleep && modifier->getNeedToSleep();
  }
  else {
    return need_to_sleep;
  }
}

float Race::getActionTimeModifier() {
  if(modifier != nullptr) {
    return action_time_modifier * modifier->getActionTimeModifier();
  }
  else {
    return action_time_modifier;
  }
}

float Race::getStrikeTimeModifier() {
  if(modifier != nullptr) {
    return strike_time_modifier * modifier->getStrikeTimeModifier();
  }
  else {
    return strike_time_modifier;
  }
}

float Race::getSkillTimeModifier() {
  if(modifier != nullptr) {
    return skill_time_modifier * modifier->getSkillTimeModifier();
  }
  else {
    return skill_time_modifier;
  }
}

float Race::getMovementTimeModifier() {
  if(modifier != nullptr) {
    return movement_time_modifier * modifier->getMovementTimeModifier();
  }
  else {
    return movement_time_modifier;
  }
}

std::list<Effect *> Race::getEffects() {
  if(modifier != nullptr) {
    std::list<Effect *> result = modifier->getEffects();
    for(Effect * effect : effects) {
      result.push_front(effect);
    }
    return result;
  }
  else {
    return effects;
  }
}

std::list<Skill *> Race::getSkills() {
  if(modifier != nullptr) {
    std::list<Skill *> result = modifier->getSkills();
    for(Skill * skill : skills) {
      result.push_front(skill);
    }
    return result;
  }
  else {
    return skills;
  }
}

std::list<Item *> Race::getLoot() {
  if(modifier != nullptr) {
    std::list<Item *> result = modifier->getLoot();
    for(Item * item : loot) {
      result.push_front(item);
    }
    return result;
  }
  else {
    return loot;
  }
}

std::list<Race *> Race::getModifiers() {
  std::list<Race *> result;
  if(modifier != nullptr) {
    result = modifier->getModifiers();
    result.push_front(modifier);
    return result;
  }
  else {
    return std::list<Race *>();
  }
}

void Race::addModifier(Race * modifier) {
  if(modifier == nullptr) {
    this->modifier = modifier;
  }
  else {
    this->modifier->addModifier(modifier);
  }
}
