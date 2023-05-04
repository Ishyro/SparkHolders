#include <sstream>

#include "ai/AI.h"
#include "data/Database.h"
#include "data/Effect.h"
#include "data/Gear.h"
#include "data/Item.h"
#include "data/skills/Skill.h"
#include "data/Settings.h"
#include "data/Speech.h"
#include "data/Way.h"
#include "data/Weapon.h"
#include "data/World.h"

#include "communication/SpeechManager.h"

#include "data/Character.h"

#include "util/String.h"

void Character::initializeCharacter(Gear * gear) {
  size = race->size;
  maxHp = attributes->baseHp + race->baseHp + origin->baseHp + culture->baseHp + religion->baseHp + profession->baseHp;
  for(Way * way : titles) {
    maxHp += way->baseHp;
  }
  maxMana = attributes->baseMana + race->baseMana + origin->baseMana + culture->baseMana + religion->baseMana + profession->baseMana;
  for(Way * way : titles) {
    maxHp += way->baseMana;
  }
  hp = maxHp;
  mana = maxMana;
  armor = race->baseArmor;
  armor_multiplier = attributes->baseArmorMult + race->baseArmorMult + origin->baseArmorMult + culture->baseArmorMult + religion->baseArmorMult + profession->baseArmorMult;
  for(Way * way : titles) {
    maxHp += way->baseArmorMult;
  }
  damage_multiplier = attributes->baseDamageMult + race->baseDamageMult + origin->baseDamageMult + culture->baseDamageMult + religion->baseDamageMult + profession->baseDamageMult;
  for(Way * way : titles) {
    maxHp += way->baseDamageMult;
  }
  soulBurnTreshold = attributes->baseSoulBurn + race->baseSoulBurn + origin->baseSoulBurn + culture->baseSoulBurn + religion->baseSoulBurn + profession->baseSoulBurn;
  for(Way * way : titles) {
    maxHp += way->baseSoulBurn;
  }
  flow = attributes->baseFlow + race->baseFlow + origin->baseFlow + culture->baseFlow + religion->baseFlow + profession->baseFlow;
  for(Way * way : titles) {
    maxHp += way->baseFlow;
  }
  visionRange = attributes->baseVisionRange + race->baseVisionRange + origin->baseVisionRange + culture->baseVisionRange + religion->baseVisionRange + profession->baseVisionRange;
  for(Way * way : titles) {
    maxHp += way->baseVisionRange;
  }
  visionPower = attributes->baseVisionPower + race->baseVisionPower + origin->baseVisionPower + culture->baseVisionPower + religion->baseVisionPower + profession->baseVisionPower;
  for(Way * way : titles) {
    maxHp += way->baseVisionPower;
  }
  detectionRange = attributes->baseDetectionRange + race->baseDetectionRange + origin->baseDetectionRange + culture->baseDetectionRange + religion->baseDetectionRange + profession->baseDetectionRange;
  for(Way * way : titles) {
    maxHp += way->baseDetectionRange;
  }
  currentSoulBurn = 0;
  currentFlowOut = 0;
  currentFlowIn = 0;
  stamina = 75.;
  satiety = 75.;
  savedHpRegen = 0.;
  savedManaRegen = 0.;
  channeledMana = 0;
  this->gear = new Gear(gear);
  for(Item * item : gear->getItems()) {
    Item * toadd = new Item(item);
    items.push_back(toadd);
  }
  for(Weapon * weapon : gear->getWeapons()) {
    Weapon * toadd = new Weapon(weapon);
    weapons.push_back(toadd);
  }
  for(Ammunition * ammo : gear->getAmmunitions()) {
    Ammunition * toadd = new Ammunition();
    toadd->projectile = ammo->projectile;
    toadd->number = ammo->number;
    toadd->gold_value = ammo->gold_value;
    toadd->ammo_type = ammo->ammo_type;
    ammunition.push_back(toadd);
  }
  initSkillsAndEffects();
}

void Character::initSkillsAndEffects() {
  for(Skill * skill : attributes->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : attributes->getEffects()) {
    if(effect->level == 0) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  if(attributes->getArchetype() != nullptr) {
    for(Skill * skill : attributes->getArchetype()->getSkills()) {
      if(skill->level == 0) {
        addSkill(skill);
      }
    }
    for(Effect * effect : attributes->getArchetype()->getEffects()) {
      if(effect->level == 0) {
        Effect * toadd = new Effect(effect, 1, 1);
        toadd->activate(this);
      }
    }
  }
  for(Skill * skill : second_attributes->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : second_attributes->getEffects()) {
    if(effect->level == 0) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : race->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : race->getEffects()) {
    if(effect->level == 0) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : origin->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : origin->getEffects()) {
    if(effect->level == 0) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : culture->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : culture->getEffects()) {
    if(effect->level == 0) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : religion->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : religion->getEffects()) {
    if(effect->level == 0) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : profession->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : profession->getEffects()) {
    if(effect->level == 0) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Way * way : titles) {
    for(Skill * skill : way->getSkills()) {
      if(skill->level == 0) {
        addSkill(skill);
      }
    }
    for(Effect * effect : way->getEffects()) {
      if(effect->level == 0) {
        Effect * toadd = new Effect(effect, 1, 1);
        toadd->activate(this);
      }
    }
  }
}

void Character::initEffects(std::list<Effect *> effects) {
  for(Effect * effect : effects) {
    Effect * toadd = new Effect(effect, 1, 1);
    toadd->activate(this);
  }
}

bool Character::isAlive() { return hp > 0 && mana > 0; }
bool Character::isSoulBurning() { return currentSoulBurn >= soulBurnTreshold; }
float Character::getX() { return x; }
float Character::getY() { return y; }
float Character::getSize() { return size; }
float Character::getOrientation() { return orientation; }
int Character::getHp() { return hp; }
int Character::getMaxHp() {
  int bonus = 0;
  for(Effect * e : effects)
    if(e->type == HP_MAX)
      bonus += e->power;
  return maxHp + bonus;
}

int Character::getMana() { return mana; }

int Character::getAvaillableMana(bool overflow) {
  // mana - 1 because killing itself is forbidden
  if(overflow) {
    return std::min(mana - 1, std::max(0, getFlow())) + channeledMana;
  }
  else {
    return std::min(mana - 1, std::max(0, getFlow() - currentFlowOut)) + channeledMana;
  }
}

int Character::getMaxMana() {
  int bonus = 0;
  for(Effect * e : effects)
    if(e->type == MANA_MAX)
      bonus += e->power;
  return std::max(maxMana + bonus, 0);
}

float Character::getStamina() { return stamina; }
float Character::getSatiety() { return satiety; }

int Character::getArmor() {
  int result = (int) std::ceil( (float) (armor + gear->getArmor()) * getArmorMultiplier());
  for(Effect * e : effects)
    if(e->type == ARMOR)
      result += e->power;
  return std::max(result, 0);
}

float Character::getArmorMultiplier() {
  int result = armor_multiplier;
  for(Effect * e : effects)
    if(e->type == ARMOR_MULTIPLIER)
      result += e->power;
  return std::max(0.F, 1.F + ((float) result) / 100.F);
}

int Character::getSoulBurnTreshold() {
  int bonus = 0;
  for(Effect * e : effects)
    if(e->type == SOULBURNTRESHOLD)
      bonus += e->power;
  return std::max(soulBurnTreshold + bonus, 0);
}

int Character::getCurrentSoulBurn() { return currentSoulBurn; }

int Character::getFlow() {
  int bonus = 0;
  for(Effect * e : effects)
    if(e->type == FLOW)
      bonus += e->power;
  return std::max(flow + bonus, 0);
}

int Character::getVisionRange() {
  int bonus = 0;
  for(Effect * e : effects)
    if(e->type == VISION_RANGE)
      bonus += e->power;
  return std::max(visionRange + bonus, 0);
}

int Character::getVisionPower() {
  int bonus = 0;
  for(Effect * e : effects)
    if(e->type == VISION_POWER)
      bonus += e->power;
  return std::max(visionPower + bonus, 0);
}

int Character::getDetectionRange() {
  int bonus = 0;
  for(Effect * e : effects)
    if(e->type == DETECTION_RANGE)
      bonus += e->power;
  return std::max(detectionRange + bonus, 0);
}

long Character::getGold() { return gold; }
long Character::getXP() { return xp; }
int Character::getLevel() { return level; }

float Character::getDamageMultiplier() {
  int result = damage_multiplier;
  for(Effect * e : effects) {
    if(e->type == DAMAGE_MULTIPLIER) {
      result += e->power;
    }
  }
  return std::max(0.F, 1.F + ((float) result) / 100.F);
}

int Character::getPowerScore() {
  return getMaxHp() + getMaxMana() + 5 * (getFlow() + getSoulBurnTreshold() + getArmor() + getDamageMultiplier());
}

bool Character::needToSend() { return need_to_send; }
void Character::setNeedToSend(bool need_to_send) { this->need_to_send = need_to_send; }
bool Character::getNeedToUpdateActions() { return need_to_update_actions; }
void Character::setNeedToUpdateActions(bool need_to_update_actions) { this->need_to_update_actions = need_to_update_actions; }

AI * Character::getAI() { return ai; }
std::string Character::getTeam() { return team; }
Speech * Character::getDeathSpeech() { return death_speech; }
Speech * Character::getTalkingSpeech() { return talking_speech; }

int Character::getCurrentMapId() { return current_map_id; }

Gear * Character::getGear() { return gear; }

float Character::getActionTimeModifier() {
  float result = race->action_time_modifier;
  for(Effect * e : effects) {
    if(e->type == ACTION_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result;
}

float Character::getHandActionTimeModifier() {
  float result = race->strike_time_modifier;
  for(Effect * e : effects) {
    if(e->type == HAND_ACTION_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result * getActionTimeModifier();
}

float Character::getSkillTimeModifier() {
  float result = race->skill_time_modifier;
  for(Effect * e : effects) {
    if(e->type == SKILL_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result * getActionTimeModifier();
}

float Character::getMovementTimeModifier() {
  float result = race->movement_time_modifier;
  for(Effect * e : effects) {
    if(e->type == MOVEMENT_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result * getActionTimeModifier();
}

float Character::getStrikeTime() {
  return (float) gear->getWeapon()->strike_time / getHandActionTimeModifier();
}

float Character::getReloadTime() {
  return (float) gear->getWeapon()->reload_time / getHandActionTimeModifier();
}

float Character::getSwapTime(std::string object) {

  return (float) gear->getWeapon()->swap_time / getHandActionTimeModifier();
}

int Character::getLight() {
  int light = 0;
  for(Effect * effect : effects) {
    if(effect->type == LIGHT) {
      light += effect->power;
    }
  }
  return light;
}
std::list<Item *> Character::getItems() { return items; }
std::list<Weapon *> Character::getWeapons() { return weapons; }
std::list<Ammunition *> Character::getAmmunitions() { return ammunition; }
std::list<Effect *> Character::getEffects() { return effects; }
std::list<Skill *> Character::getSkills() { return skills; }

std::map<Skill *, std::array<int, DAMAGE_TYPE_NUMBER>> Character::getDamageSkills() {
  std::map<Skill *, std::array<int, DAMAGE_TYPE_NUMBER>> result = std::map<Skill *, std::array<int, DAMAGE_TYPE_NUMBER>>();
  for(Skill * skill : skills) {
    if(skill->getManaCost(1, 1, 1) < mana) {
      std::array<int, DAMAGE_TYPE_NUMBER> damages;
      bool isDamageSkill = false;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages[i] = skill->getDamageFromType(i, 1);
        if(damages[i] > 0) {
          isDamageSkill = true;
        }
      }
      if(isDamageSkill) {
        result.insert(std::make_pair(skill, damages));
      }
    }
  }
  std::array<int, DAMAGE_TYPE_NUMBER> damages;
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] = getDamageFromType(i);
  }
  result.insert(std::make_pair(nullptr, damages));
  return result;
}

std::list<Item *> Character::getSellableItems() { return sellable_items; }
std::list<Weapon *> Character::getSellableWeapons() { return sellable_weapons; }
std::list<Ammunition *> Character::getSellableAmmunitions() { return sellable_ammunition; }
std::list<Effect *> Character::getSellableEffects() { return sellable_effects; }
std::list<Skill *> Character::getSellableSkills() { return sellable_skills; }

Attributes * Character::getAttributes() { return attributes; }
Attributes * Character::getSecondAttributes() { return second_attributes; }
Way * Character::getRace() { return race; }
Way * Character::getOrigin() { return origin; }
Way * Character::getCulture() { return culture; }
Way * Character::getReligion() { return religion; }
Way * Character::getProfession() { return profession; }

std::list<Way *> Character::getTitles() { return titles; }

void Character::setOrientation(float orientation) { this->orientation = orientation; }
void Character::setSize(float size) { this->size = size; }
void Character::move(float y, float x, float orientation, int map_id) {
  this->x = x;
  this->y = y;
  this->orientation = orientation;
  this->current_map_id = map_id;
}

void Character::hpHeal(int hp) { this->hp = std::min(this->hp + hp, getMaxHp()); }
void Character::incrMaxHp() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += attributes->hpIncr;
  incr += second_attributes->hpIncr;
  incr += race->hpIncr;
  incr += origin->hpIncr;
  incr += culture->hpIncr;
  incr += religion->hpIncr;
  incr += profession->hpIncr;
  for(Way * title : titles) {
    incr += title->hpIncr;
  }
  maxHp += std::max(incr, 0);
}
void Character::setHp(int hp) { this->hp = hp; }

void Character::manaHeal(int mana) {
  int maxManaHeal = std::max(0, std::min(mana, getFlow() - currentFlowIn));
  int realManaHeal = std::min(maxManaHeal, getMaxMana() - this->mana);
  this->mana += realManaHeal;
  currentFlowIn += realManaHeal;
}

void Character::incrMaxMana() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += attributes->manaIncr;
  incr += second_attributes->manaIncr;
  incr += race->manaIncr;
  incr += origin->manaIncr;
  incr += culture->manaIncr;
  incr += religion->manaIncr;
  incr += profession->manaIncr;
  for(Way * title : titles) {
    incr += title->manaIncr;
  }
  maxMana += std::max(incr, 0);
}

void Character::setMana(int mana) { this->mana = mana; }
void Character::addStamina(float stamina) { this->stamina = std::min(100.F, this->stamina + stamina); }
void Character::addSatiety(float satiety) { this->satiety = std::min(100.F, this->satiety + satiety); }
void Character::removeStamina(float stamina) { this->stamina = std::max(0.F, this->stamina - stamina); }
void Character::removeSatiety(float satiety) { this->satiety = std::max(0.F, this->satiety - satiety); }
void Character::setStamina(float stamina) { this->stamina = stamina; }
void Character::setSatiety(float satiety) { this->satiety = satiety; }

void Character::incrArmorMultiplier() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += attributes->armorMultIncr;
  incr += second_attributes->armorMultIncr;
  incr += race->armorMultIncr;
  incr += origin->armorMultIncr;
  incr += culture->armorMultIncr;
  incr += religion->armorMultIncr;
  incr += profession->armorMultIncr;
  for(Way * title : titles) {
    incr += title->armorMultIncr;
  }
  armor += std::max(incr, 0);
}
void Character::incrDamageMultiplier() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += attributes->damageMultIncr;
  incr += second_attributes->damageMultIncr;
  incr += race->damageMultIncr;
  incr += origin->damageMultIncr;
  incr += culture->damageMultIncr;
  incr += religion->damageMultIncr;
  incr += profession->damageMultIncr;
  for(Way * title : titles) {
    incr += title->damageMultIncr;
  }
  damage_multiplier += std::max(incr, 0);
}
void Character::incrSoulBurnTreshold() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += attributes->soulBurnIncr;
  incr += second_attributes->soulBurnIncr;
  incr += race->soulBurnIncr;
  incr += origin->soulBurnIncr;
  incr += culture->soulBurnIncr;
  incr += religion->soulBurnIncr;
  incr += profession->soulBurnIncr;
  for(Way * title : titles) {
    incr += title->soulBurnIncr;
  }
  soulBurnTreshold += std::max(incr, 0);
}
void Character::setCurrentSoulBurn(int soulBurn) { this->currentSoulBurn = soulBurn; }
void Character::incrFlow() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += attributes->flowIncr;
  incr += second_attributes->flowIncr;
  incr += race->flowIncr;
  incr += origin->flowIncr;
  incr += culture->flowIncr;
  incr += religion->flowIncr;
  incr += profession->flowIncr;
  for(Way * title : titles) {
    incr += title->flowIncr;
  }
  flow += std::max(incr, 0);
}

void Character::incrVisionRange() {
  if(player_character) {
    setNeedToSend(true);
  }
  visionRange++;
}

void Character::incrVisionPower() {
  if(player_character) {
    setNeedToSend(true);
  }
  visionPower++;
}

void Character::incrDetectionRange() {
  if(player_character) {
    setNeedToSend(true);
  }
  detectionRange++;
}

void Character::setCurrentMapId(int map_id) { this->current_map_id = map_id; }

void Character::applySoulBurn() {
  int soulBurnReduction = (int) std::max( (float) currentSoulBurn / 10., (float) soulBurnTreshold / 10.);
  if(currentSoulBurn > soulBurnTreshold) {
    hp -= std::min(soulBurnReduction, currentSoulBurn - soulBurnTreshold);
  }
  // currentFlowOut will damage the character.
  currentSoulBurn = std::max(0, currentSoulBurn - soulBurnReduction) + std::max(0, 2 * (currentFlowOut - getFlow()));
  currentFlowOut = 0;
  // currentFlowIn will not damage the character, because any extra mana in is lost.
  currentFlowIn = 0;
  channeledMana = (int) std::floor( (float) channeledMana * 0.8);
}

void Character::applyTiredness() {
  if(race->need_to_sleep) {
    float step = 100.F / (Settings::getDayDurationInRound() * Settings::getMaxNumberOfDaysAwake());
    float currentManaRegen = Settings::getStaminaRecoveryRatio() * step * getMaxMana() / 100.F;
    int manaValue = (int) std::floor(currentManaRegen + savedManaRegen);
    savedManaRegen += currentManaRegen - (float) manaValue;
    if(stamina > 0.) {
      removeStamina(step);
      manaHeal(manaValue);
    }
    else {
      manaValue = (int) std::floor(currentManaRegen * Settings::getStaminaOverextendRatio() + savedManaRegen);
      savedManaRegen += currentManaRegen - (float) manaValue;
      payMana(manaValue);
    }
  }
}

void Character::applyHunger() {
  if(race->need_to_eat) {
    float step = 100.F / (Settings::getDayDurationInRound() * Settings::getMaxNumberOfDaysFasting());
    float currentHpRegen = Settings::getSatietyRecoveryRatio() * step * getMaxHp() / 100.F;
    int hpValue = (int) std::floor(currentHpRegen + savedHpRegen);
    savedHpRegen += currentHpRegen - (float) hpValue;
    if(satiety > 0.) {
      removeSatiety(step);
      hpHeal(hpValue);
    }
    else {
      hpValue = (int) std::floor(currentHpRegen * Settings::getSatietyOverextendRatio() + savedHpRegen);
      savedHpRegen += currentHpRegen - (float) hpValue;
      hp -= hpValue;
    }
  }
}

void Character::applyEffects() {
  for(Effect * e : effects) {
    if(e->duration_type == TEMPORARY_DURATION) {
      if(player_character) {
        setNeedToSend(true);
      }
      if(e->tick(this)) {
        e->desactivate(this);
        delete e;
      }
    }
  }
}

void Character::rest() {
  if(race->need_to_sleep) {
    // +1 because the character will still apply his tiredness while sleeping
    addStamina( (float) (3 + 1) * 100.F / (Settings::getDayDurationInRound() * Settings::getMaxNumberOfDaysAwake()));
  }
}

void Character::gainGold(long gold) { this->gold += gold; }
void Character::loseGold(long gold) { this->gold = (long) std::max(0., (double) this->gold + gold); }
void Character::payMana(int cost) {
  int realCost = std::max(0, cost - channeledMana);
  channeledMana -= cost - realCost;
  mana -= realCost;
  currentSoulBurn += realCost;
  currentFlowOut += realCost;
}
void Character::gainXP(long xp) { this->xp += xp; }
void Character::gainLevel() {
  while(xp >= level * level * 1000) { // INSERT FORMULA HERE
    level++;
    int old_max_mana = getMaxMana();
    int old_max_hp = getMaxHp();
    incrMaxHp();
    hpHeal(getMaxHp() - old_max_hp);
    incrMaxMana();
    manaHeal(getMaxMana() - old_max_mana);
    incrArmorMultiplier();
    incrDamageMultiplier();
    incrSoulBurnTreshold();
    incrFlow();
    newSkillsAndEffects();
    if(level == 10) {
      selectSecondAttributes();
    }
  }
}

void Character::newSkillsAndEffects() {
  for(Skill * skill : attributes->getSkills()) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : attributes->getEffects()) {
    if(level == 5 * effect->level) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  if(attributes->getArchetype() != nullptr) {
    for(Skill * skill : attributes->getArchetype()->getSkills()) {
      if(level == 5 * skill->level) {
        addSkill(skill);
      }
    }
    for(Effect * effect : attributes->getArchetype()->getEffects()) {
      if(level == 5 * effect->level) {
        Effect * toadd = new Effect(effect, 1, 1);
        toadd->activate(this);
      }
    }
  }
  for(Skill * skill : second_attributes->getSkills()) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : second_attributes->getEffects()) {
    if(level == 5 * effect->level) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : race->getSkills()) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : race->getEffects()) {
    if(level == 5 * effect->level) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : origin->getSkills()) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : origin->getEffects()) {
    if(level == 5 * effect->level) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : culture->getSkills()) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : culture->getEffects()) {
    if(level == 5 * effect->level) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : religion->getSkills()) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : religion->getEffects()) {
    if(level == 5 * effect->level) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : profession->getSkills()) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : profession->getEffects()) {
    if(level == 5 * effect->level) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Way * way : titles) {
    for(Skill * skill : way->getSkills()) {
      if(level == 5 * skill->level) {
        addSkill(skill);
      }
    }
    for(Effect * effect : way->getEffects()) {
      if(level == 5 * effect->level) {
        Effect * toadd = new Effect(effect, 1, 1);
        toadd->activate(this);
      }
    }
  }
}

void Character::selectSecondAttributes() {

}

void Character::setAI(AI * ai) { this->ai = ai; }
void Character::setTeam(std::string team) { this->team = team; }

void Character::setDeathSpeech(std::string option, Database * database) {
  death_speech = death_speech->getNext(option, database);
  SpeechManager::add(death_speech);
}

void Character::setTalkingSpeech(std::string option, Database * database) {
  talking_speech = talking_speech->getNext(option, database);
  SpeechManager::add(talking_speech);
}

void Character::equip(Item * to_equip) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(to_equip != nullptr) {
    std::list<Item *> items = gear->equip(to_equip);
    for(Item * item : items) {
      for(Effect * e : item->effects) {
        removeEffect(e);
      }
      items.push_back(item);
    }
    for(Effect * e : to_equip->effects) {
      addEffect(e);
    }
  }
}

void Character::equip(Weapon * to_equip) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(to_equip != nullptr) {
    Weapon * old_weapon = gear->equip(to_equip);
    removeWeapon(to_equip);
    if(old_weapon != nullptr) {
      for(Effect * e : old_weapon->effects) {
        removeEffect(e);
      }
      addWeapon(old_weapon);
    }
    for(Effect * e : to_equip->effects) {
      addEffect(e);
    }
  }
}

void Character::unequip(int type) {
  if(player_character) {
    setNeedToSend(true);
  }
  Item * old_item = gear->unequip(type);
  if(old_item != nullptr) {
    for(Effect * e : old_item->effects) {
      removeEffect(e);
    }
    items.push_back(old_item);
  }
}

void Character::unequipWeapon() {
  if(player_character) {
    setNeedToSend(true);
  }
  Weapon * old_weapon = gear->unequipWeapon();
  if(old_weapon != nullptr) {
    for(Effect * e : old_weapon->effects) {
      removeEffect(e);
    }
    weapons.push_back(old_weapon);
  }
}

void Character::addEffect(Effect * e) {
  if(player_character) {
    setNeedToSend(true);
  }
  effects.push_back(e);
}

void Character::addSkill(Skill * s) {
  if(player_character) {
    setNeedToSend(true);
  }
  skills.push_back(s);
}

bool Character::hasSkill(Skill * s) {
  for(Skill * skill : skills) {
    if(skill->name == s->name) {
      return true;
    }
  }
  return false;
}

void Character::removeEffect(Effect * e) {
  if(player_character) {
    setNeedToSend(true);
  }
  effects.remove(e);
}

void Character::removeSkill(Skill * s) {
  if(player_character) {
    setNeedToSend(true);
  }
  skills.remove(s);
}

void Character::setWay(Way * way) {
  if(player_character) {
    setNeedToSend(true);
  }
  Way * to_remove = nullptr;
  switch(way->type) {
    case RACE:
      to_remove = race;
      race = way;
      break;
    case ORIGIN:
      to_remove = origin;
      origin = way;
      break;
    case CULTURE:
      to_remove = culture;
      culture = way;
      break;
    case RELIGION:
      to_remove = religion;
      religion = way;
      break;
    case PROFESSION:
      to_remove = profession;
      profession = way;
      break;
    case TITLE:
      for(Way * title : titles) {
        if(title == way) {
          return;
        }
      }
      titles.push_back(way);
    default:;
  }
  for(Effect * e : way->getEffects()) {
    Effect * effect = new Effect(e, 1, 1);
    effect->activate(this);
  }
  for(Skill * s : way->getSkills()) {
    addSkill(s);
  }
  if(to_remove != nullptr) {
    for(Effect * e1 : to_remove->getEffects()) {
      for(Effect * e2 : effects) {
        if(e1->name == e2->name) {
          e2->desactivate(this);
          break;
        }
      }
    }
    for(Skill * s1 : to_remove->getSkills()) {
      for(Skill * s2 : skills) {
        if(s1 == s2) {
          removeSkill(s2);
          break;
        }
      }
    }
  }
}

void Character::addItem(Item * i) {
  if(player_character) {
    setNeedToSend(true);
  }
  items.push_back(i);
}

void Character::addWeapon(Weapon * w) {
  if(player_character) {
    setNeedToSend(true);
  }
  weapons.push_back(w);
}

void Character::addAmmunition(Ammunition * a) {
  if(player_character) {
    setNeedToSend(true);
  }
  ammunition.push_back(a);
}

void Character::removeItem(Item * i) {
  if(player_character) {
    setNeedToSend(true);
  }
  items.remove(i);
}

void Character::removeWeapon(Weapon * w) {
  if(player_character) {
    setNeedToSend(true);
  }
  weapons.remove(w);
}

void Character::removeAmmunition(Ammunition * a) {
  if(player_character) {
    setNeedToSend(true);
  }
  ammunition.remove(a);
}

void Character::useItem(long item_id) {
  Item * to_remove = nullptr;
  for(Item * i : items) {
    if(i->id == item_id && i->consumable) {
      if(player_character) {
        setNeedToSend(true);
      }
      if(!i->isFood() || race->can_eat_food) {
        for(Effect * e : i->effects) {
          e->activate(this);
        }
        to_remove = i;
        break;
      }
    }
  }
  if(to_remove != nullptr) {
    removeItem(to_remove);
    delete to_remove;
  }
}

bool Character::isFlying() {
  for(Effect * e : effects) {
    if(e->type == FLY) {
      return true;
    }
  }
  return false;
}

bool Character::isChanneling() {
  for(Effect * e : effects) {
    if(e->type == CHANNELING) {
      return true;
    }
  }
  return false;
}

bool Character::isStunned() {
  for(Effect * e : effects) {
    if(e->type == STUNNED) {
      return true;
    }
  }
  return false;
}

bool Character::isCloaked() {
  for(Effect * e : effects) {
    if(e->type == CLOAKED) {
      return true;
    }
  }
  return false;
}

bool Character::isInvisible() {
  for(Effect * e : effects) {
    if(e->type == INVISIBLE) {
      return true;
    }
  }
  return false;
}

bool Character::isEtheral() {
  for(Effect * e : effects) {
    if(e->type == ETHERAL) {
      return true;
    }
  }
  return false;
}

bool Character::isInvulnerable() {
  for(Effect * e : effects) {
    if(e->type == INVULNERABLE) {
      return true;
    }
  }
  return false;
}

bool Character::isSleeping() {
  for(Effect * e : effects) {
    if(e->type == SLEEPING) {
      return true;
    }
  }
  return false;
}

int Character::cloakPower() {
  int max = 0;
  for(Effect * e : effects) {
    if(e->type == CLOAKED) {
      if(e->power > max) {
        max = e->power;
      }
    }
  }
  return max;
}

bool Character::isInWeakState() {
  for(Effect * e : effects) {
    if(e->type == STUNNED || e->type == SLEEPING) {
      return true;
    }
  }
  return false;
}

void Character::useSkill(Skill * skill, Target * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range) {
  skill->activate(this, target, adventure, overcharge_power, overcharge_duration, overcharge_range);
}

int Character::getDamageFromType(int damage_type) {
  int damage = 0;
  if(gear->getWeapon() != nullptr) {
    damage = gear->getWeapon()->getDamageFromType(damage_type);
  }
  for(Effect * e : effects) {
    if(e->type == DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type);
    }
  }
  return (int) std::ceil((float) damage * getDamageMultiplier());
}

float Character::getDamageReductionFromType(int damage_type) {
  float reduction = gear->getDamageReductionFromType(damage_type);
  for(Effect * e : effects) {
    if(e->type == DAMAGE_REDUCTION) {
      reduction += e->getDamageReductionFromType(damage_type);
    }
  }
  return reduction;
}

Projectile * Character::shoot(Target * target, Adventure * adventure) {
  if(!gear->getWeapon()->melee) { //&& gear->getWeapon()->range >= MapUtil::distance(this->x, this->y, this->dx, this->dy, x, y, dx, dy)) {
    if(!gear->getWeapon()->use_ammo || gear->getWeapon()->getCurrentCapacity() > 0) {
      int realDamages[DAMAGE_TYPE_NUMBER];
      for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
        realDamages[damage_type] = getDamageFromType(damage_type);
      }
      Projectile * base_projectile = gear->getWeapon()->getAmmo()->projectile;
      if(gear->getWeapon()->use_ammo) {
        if(player_character) {
          setNeedToSend(true);
        }
        gear->getWeapon()->useAmmo();
      }
      return new Projectile(base_projectile, realDamages, adventure->getWorld(), current_map_id, target, this, 1, 1, 1, true);
    }
  }
  return nullptr;
}

void Character::attack(Character * target, int type) {
  if(gear->getWeapon()->melee && gear->getWeapon()->range >= std::max(abs(x - target->getX()), abs(y - target->getY()))) {
    int realDamages[DAMAGE_TYPE_NUMBER];
    for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
      realDamages[damage_type] = getDamageFromType(damage_type);
    }
    target->receiveAttack(realDamages, orientation, type);
  }
}

void Character::reload(Ammunition * ammo) {
  if(player_character) {
    setNeedToSend(true);
  }
  Ammunition * oldAmmo = gear->getWeapon()->reload(ammo);
  if(ammo->number == 0) {
    ammunition.remove(ammo);
    delete ammo;
    ammo = nullptr;
  }
  if(oldAmmo != nullptr) {
    ammunition.push_back(oldAmmo);
  }
}


Ammunition * Character::canReload() {
  Ammunition * ammo = nullptr;
  int number = 0;
  for(Ammunition * current : ammunition) {
    if(gear->getWeapon()->ammo_type == current->ammo_type && current->number > number) {
      ammo = current;
      number = current->number;
    }
  }
  return ammo;
}

Weapon * Character::swapMelee() {
  Weapon * weapon = nullptr;
  int rawDamage = 0;
  for(Weapon * current : weapons) {
    int currentRawDamage = current->getRawDamage();
    if(currentRawDamage > rawDamage) {
      weapon = current;
      rawDamage = currentRawDamage;
    }
  }
  return weapon;
}

void Character::receiveAttack(int damages[DAMAGE_TYPE_NUMBER], float orientation, int type) {
  if(!isInvulnerable() && !isEtheral()) {
    if(orientation != 360.F) {
      float diff = 360.F + orientation - this->orientation;
      diff = diff >= 360.F ? diff - 360.F : diff;
      if(diff <= 30.F || diff >= 330.F) {
        return receiveCriticalAttack(damages, type);
      }
    }
    if(isInWeakState()) {
      return receiveCriticalAttack(damages, type);
    }
    int damage = 0;
    for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
      if(damage_type == NEUTRAL_DAMAGE) {
        damage += damages[damage_type];
      }
      if(damage_type == MIND_DAMAGE) {
        hp -= std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
      }
      if(damage_type == TRUE_DAMAGE) {
        hp -= damages[damage_type];
      }
      if(damage_type == SOUL_DAMAGE) {
        payMana(damages[damage_type]);
      }
      else {
        damage += std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
      }
    }
    if(type == HEAVY_STRIKE) {
      hp -= (std::max(0, damage - getArmor())) * 1.5;
    }
    else {
      hp -= std::max(0, damage - getArmor());
    }
  }
}

void Character::receiveCriticalAttack(int damages[DAMAGE_TYPE_NUMBER], int type) {
  if(!isInvulnerable() && !isEtheral()) {
    int damage = 0;
    for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
      if(damage_type == NEUTRAL_DAMAGE) {
        damage += damages[damage_type] * 2;
      }
      if(damage_type == MIND_DAMAGE) {
        float damage_reduction = getDamageReductionFromType(damage_type);
        if(damage_reduction > 0.F) {
          hp -= std::max(0, (int) floor( (float) (damages[damage_type] * 2) * (1.F - .5 * damage_reduction)));
        }
        else {
          hp -= std::max(0, (int) floor( (float) (damages[damage_type] * 2) * (1.F - damage_reduction)));
        }
      }
      if(damage_type == TRUE_DAMAGE) {
        hp -= damages[damage_type] * 2;
      }
      if(damage_type == SOUL_DAMAGE) {
        hp -= damages[damage_type] * 2;
        payMana(damages[damage_type] * 2);
      }
      else {
        float damage_reduction = getDamageReductionFromType(damage_type);
        if(damage_reduction > 0.F) {
          damage += std::max(0, (int) floor( (float) (damages[damage_type] * 2) * (1.F - .5 * damage_reduction)));
        }
        else {
          damage += std::max(0, (int) floor( (float) (damages[damage_type] * 2) * (1.F - damage_reduction)));
        }
      }
    }
    if(type == HEAVY_STRIKE) {
      hp -= (std::max(0, damage - getArmor())) * 1.5;
    }
    else {
      hp -= std::max(0, damage - getArmor());
    }
  }
}

int Character::tryAttack(std::array<int, DAMAGE_TYPE_NUMBER> damages, int type) {
  if(isInvulnerable() || isEtheral()) {
    return 0;
  }
  int damage = 0;
  int trueDamage = 0;
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    if(damage_type == NEUTRAL_DAMAGE) {
      damage += damages[damage_type];
    }
    if(damage_type == MIND_DAMAGE) {
      trueDamage += std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
    }
    if(damage_type == TRUE_DAMAGE) {
      trueDamage += damages[damage_type];
    }
    if(damage_type == SOUL_DAMAGE) {
      trueDamage += damages[damage_type];
    }
    else {
      damage += std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
    }
  }
  if(type == HEAVY_STRIKE) {
    return (trueDamage + std::max(0, damage - getArmor())) * 1.5;
  }
  else {
    return trueDamage + std::max(0, damage - getArmor());
  }
}

void Character::trade(Character * buyer, int object_type, std::string object_name, float price_modifier) {
  int price = 0;
  switch (object_type) {
    case ITEM:
      for(Item * item : sellable_items) {
        if(item->name == object_name) {
          price = (int) std::ceil((float) item->gold_value * price_modifier);
          if(buyer->getGold() >= price) {
            buyer->addItem(item);
            buyer->loseGold(price);
            sellable_items.remove(item);
            gainGold(price);
          }
        }
      }
      break;
    case WEAPON:
      for(Weapon * weapon : sellable_weapons) {
        if(weapon->name == object_name) {
          price = (int) std::ceil((float) weapon->gold_value * price_modifier);
          if(buyer->getGold() >= price) {
            buyer->addWeapon(weapon);
            buyer->loseGold(price);
            sellable_weapons.remove(weapon);
            gainGold(price);
          }
        }
      }
      break;
    case AMMUNITION:
      for(Ammunition * ammo : sellable_ammunition) {
        if(ammo->projectile->name == object_name) {
          price = (int) std::ceil((float) ammo->gold_value * price_modifier);
          if(buyer->getGold() >= price) {
            buyer->addAmmunition(ammo);
            buyer->loseGold(price);
            sellable_ammunition.remove(ammo);
            gainGold(price);
          }
        }
      }
      break;
    case SKILL:
      for(Skill * skill : sellable_skills) {
        if(skill->name == object_name && buyer->level >= 5 * skill->level && (skill->attributes == "" || buyer->attributes->name == skill->attributes)) {
          price = (int) std::ceil((float) (skill->level * skill->level) * 1000.F * price_modifier);
          if(buyer->getGold() >= price) {
            buyer->addSkill(skill);
            buyer->loseGold(price);
            gainGold(price);
          }
        }
      }
      break;
    case EFFECT:
      for(Effect * effect : sellable_effects) {
        if(effect->name == object_name && buyer->level >= 5 * effect->level && (effect->attributes == "" || buyer->attributes->name == effect->attributes)) {
          price = (int) std::ceil((float) (effect->level * effect->level) * 1000.F * price_modifier);
          if(buyer->getGold() >= price) {
            Effect * toadd = new Effect(effect, 1, 1);
            toadd->activate(buyer);
            buyer->loseGold(price);
            gainGold(price);
          }
        }
      }
      break;
    default:
      break;
  }
}

std::string Character::to_string(int offsetY, int offsetX) {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, hp);
  String::insert_int(ss, getMaxHp());
  String::insert_int(ss, mana);
  String::insert_int(ss, getMaxMana());
  String::insert_float(ss, getStamina());
  String::insert_float(ss, getSatiety());
  String::insert_int(ss, getCurrentSoulBurn());
  String::insert_int(ss, getSoulBurnTreshold());
  String::insert_int(ss, getFlow());
  String::insert_bool(ss, player_character);
  String::insert_int(ss, type);
  String::insert_float(ss, x - (float) offsetX);
  String::insert_float(ss, y - (float) offsetY);
  String::insert_float(ss, size);
  String::insert_float(ss, orientation);
  String::insert(ss, team);
  String::insert_int(ss, getArmor());
  String::insert_int(ss, xp);
  String::insert_int(ss, level);
  if(talking_speech != nullptr) {
    String::insert(ss, ((Speech *) talking_speech)->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_float(ss, getDamageReductionFromType(i));
  }
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_int(ss, getDamageFromType(i));
  }
  std::stringstream * ss_sellable_items = new std::stringstream();
  for(Item * item : sellable_items) {
    String::insert(ss_sellable_items, item->to_string());
  }
  String::insert(ss, ss_sellable_items->str());
  delete ss_sellable_items;
  std::stringstream * ss_sellable_weapons = new std::stringstream();
  for(Weapon * weapon : sellable_weapons) {
    String::insert(ss_sellable_weapons, weapon->to_string());
  }
  String::insert(ss, ss_sellable_weapons->str());
  delete ss_sellable_weapons;
  std::stringstream * ss_sellable_ammunition = new std::stringstream();
  for(Ammunition * ammo : sellable_ammunition) {
    String::insert(ss_sellable_ammunition, Projectile::ammo_to_string(ammo));
  }
  String::insert(ss, ss_sellable_ammunition->str());
  delete ss_sellable_ammunition;
  std::stringstream * ss_sellable_effects = new std::stringstream();
  for(Effect * effect : sellable_effects) {
    String::insert(ss_sellable_effects, effect->to_string());
  }
  String::insert(ss, ss_sellable_effects->str());
  delete ss_sellable_effects;
  std::stringstream * ss_sellable_skills = new std::stringstream();
  for(Skill * skill : sellable_skills) {
    String::insert(ss_sellable_skills, skill->to_string());
  }
  String::insert(ss, ss_sellable_skills->str());
  delete ss_sellable_skills;
  std::string result = ss->str();
  delete ss;
  return result;
}

CharacterDisplay * Character::from_string(std::string to_read) {
  CharacterDisplay * display = new CharacterDisplay();
  std::stringstream * ss = new std::stringstream(to_read);
  display->name = String::extract(ss);
  display->id = String::extract_long(ss);
  display->hp = String::extract_int(ss);
  display->maxHp = String::extract_int(ss);
  display->mana = String::extract_int(ss);
  display->maxMana = String::extract_int(ss);
  display->stamina = String::extract_float(ss);
  display->satiety = String::extract_float(ss);
  display->soulBurn = String::extract_int(ss);
  display->soulBurnTreshold = String::extract_int(ss);
  display->flow = String::extract_int(ss);
  display->player_character = String::extract_bool(ss);
  display->type = String::extract_int(ss);
  display->x = String::extract_float(ss);
  display->y = String::extract_float(ss);
  display->size = String::extract_float(ss);
  display->orientation = String::extract_float(ss);
  display->team = String::extract(ss);
  display->armor = String::extract_int(ss);
  display->xp = String::extract_int(ss);
  display->level = String::extract_int(ss);
  std::string talking_speech_str = String::extract(ss);
  display->talking_speech = nullptr;
  if(talking_speech_str != "none") {
    display->talking_speech = Speech::from_string(talking_speech_str);
  }
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damage_reductions[i] = String::extract_float(ss);
  }
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damages[i] = String::extract_int(ss);
  }
  std::stringstream * ss_sellable_items = new std::stringstream(String::extract(ss));
  while(ss_sellable_items->rdbuf()->in_avail() != 0) {
    display->sellable_items.push_back(Item::from_string(String::extract(ss_sellable_items)));
  }
  delete ss_sellable_items;
  std::stringstream * ss_sellable_weapons = new std::stringstream(String::extract(ss));
  while(ss_sellable_weapons->rdbuf()->in_avail() != 0) {
    display->sellable_weapons.push_back(Weapon::from_string(String::extract(ss_sellable_weapons)));
  }
  delete ss_sellable_weapons;
  std::stringstream * ss_sellable_ammunition = new std::stringstream(String::extract(ss));
  while(ss_sellable_ammunition->rdbuf()->in_avail() != 0) {
    display->sellable_ammunition.push_back(Projectile::ammo_from_string(String::extract(ss_sellable_ammunition)));
  }
  delete ss_sellable_ammunition;
  std::stringstream * ss_sellable_effects = new std::stringstream(String::extract(ss));
  while(ss_sellable_effects->rdbuf()->in_avail() != 0) {
    display->sellable_effects.push_back(Effect::from_string(String::extract(ss_sellable_effects)));
  }
  delete ss_sellable_effects;
  std::stringstream * ss_sellable_skills = new std::stringstream(String::extract(ss));
  while(ss_sellable_skills->rdbuf()->in_avail() != 0) {
    display->sellable_skills.push_back(Skill::from_string(String::extract(ss_sellable_skills)));
  }
  delete ss_sellable_skills;
  delete ss;
  return display;
}

std::string Character::full_to_string(Adventure * adventure) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, maxHp);
  String::insert_int(ss, maxMana);
  String::insert_int(ss, hp);
  String::insert_int(ss, mana);
  String::insert_int(ss, armor);
  String::insert_int(ss, armor_multiplier);
  String::insert_int(ss, damage_multiplier);
  String::insert_int(ss, soulBurnTreshold);
  String::insert_int(ss, flow);
  String::insert_int(ss, visionRange);
  String::insert_int(ss, visionPower);
  String::insert_int(ss, detectionRange);
  String::insert_int(ss, currentSoulBurn);
  String::insert_float(ss, stamina);
  String::insert_float(ss, satiety);
  String::insert_float(ss, savedHpRegen);
  String::insert_float(ss, savedManaRegen);
  String::insert_int(ss, channeledMana);
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_bool(ss, player_character);
  if(death_speech != nullptr) {
    String::insert(ss, ((Speech *) death_speech)->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(talking_speech != nullptr) {
    String::insert(ss, ((Speech *) talking_speech)->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  String::insert_int(ss, type);
  String::insert_float(ss, x);
  String::insert_float(ss, y);
  String::insert_float(ss, size);
  String::insert_float(ss, orientation);
  String::insert_int(ss, current_map_id);
  String::insert_bool(ss, has_soulspark);
  String::insert_bool(ss, merchant);
  String::insert_long(ss, gold);
  String::insert_long(ss, xp);
  String::insert_int(ss, level);
  String::insert(ss, team);
  String::insert(ss, gear->to_string());
  std::stringstream * ss_items = new std::stringstream();
  for(Item * item : items) {
    String::insert(ss_items, item->to_string());
  }
  String::insert(ss, ss_items->str());
  delete ss_items;
  std::stringstream * ss_weapons = new std::stringstream();
  for(Weapon * weapon : weapons) {
    String::insert(ss_weapons, weapon->to_string());
  }
  String::insert(ss, ss_weapons->str());
  delete ss_weapons;
  std::stringstream * ss_ammunition = new std::stringstream();
  for(Ammunition * ammo : ammunition) {
    String::insert(ss_ammunition, Projectile::ammo_to_string(ammo));
  }
  String::insert(ss, ss_ammunition->str());
  delete ss_ammunition;
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  std::stringstream * ss_skills = new std::stringstream();
  for(Skill * skill : skills) {
    String::insert(ss_skills, skill->to_string());
  }
  String::insert(ss, ss_skills->str());
  delete ss_skills;
  std::stringstream * ss_sellable_items = new std::stringstream();
  for(Item * item : sellable_items) {
    String::insert(ss_sellable_items, item->to_string());
  }
  String::insert(ss, ss_sellable_items->str());
  delete ss_sellable_items;
  std::stringstream * ss_sellable_weapons = new std::stringstream();
  for(Weapon * weapon : sellable_weapons) {
    String::insert(ss_sellable_weapons, weapon->to_string());
  }
  String::insert(ss, ss_sellable_weapons->str());
  delete ss_sellable_weapons;
  std::stringstream * ss_sellable_ammunition = new std::stringstream();
  for(Ammunition * ammo : sellable_ammunition) {
    String::insert(ss_sellable_ammunition, Projectile::ammo_to_string(ammo));
  }
  String::insert(ss, ss_sellable_ammunition->str());
  delete ss_sellable_ammunition;
  std::stringstream * ss_sellable_effects = new std::stringstream();
  for(Effect * effect : sellable_effects) {
    String::insert(ss_sellable_effects, effect->to_string());
  }
  String::insert(ss, ss_sellable_effects->str());
  delete ss_sellable_effects;
  std::stringstream * ss_sellable_skills = new std::stringstream();
  for(Skill * skill : sellable_skills) {
    String::insert(ss_sellable_skills, skill->to_string());
  }
  String::insert(ss, ss_sellable_skills->str());
  delete ss_sellable_skills;

  String::insert(ss, attributes->to_string());
  if(second_attributes != nullptr) {
    String::insert(ss, ((Attributes *) second_attributes)->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  String::insert(ss, race->to_string());
  String::insert(ss, origin->to_string());
  String::insert(ss, culture->to_string());
  String::insert(ss, religion->to_string());
  String::insert(ss, profession->to_string());
  std::stringstream * ss_titles = new std::stringstream();
  for(Way * title : titles) {
    String::insert(ss_titles, title->to_string());
  }
  String::insert(ss, ss_titles->str());
  delete ss_titles;
  std::string result = ss->str();
  delete ss;
  return result;
}

Character * Character::full_from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  int maxHp = String::extract_int(ss);
  int maxMana = String::extract_int(ss);
  int hp = String::extract_int(ss);
  int mana = String::extract_int(ss);
  int armor = String::extract_int(ss);
  int armor_multiplier = String::extract_int(ss);
  int damage_multiplier = String::extract_int(ss);
  int soulBurnTreshold = String::extract_int(ss);
  int flow = String::extract_int(ss);
  int visionRange = String::extract_int(ss);
  int visionPower = String::extract_int(ss);
  int detectionRange = String::extract_int(ss);
  int currentSoulBurn = String::extract_int(ss);
  float stamina = String::extract_float(ss);
  float satiety = String::extract_float(ss);
  float savedHpRegen = String::extract_float(ss);
  float savedManaRegen = String::extract_float(ss);
  int channeledMana = String::extract_int(ss);
  std::string name = String::extract(ss);
  long id = String::extract_long(ss);
  bool player_character = String::extract_bool(ss);
  std::string death_speech_str = String::extract(ss);
  Speech * death_speech = nullptr;
  if(death_speech_str != "none") {
    death_speech = Speech::from_string(death_speech_str);
  }
  std::string talking_speech_str = String::extract(ss);
  Speech * talking_speech = nullptr;
  if(talking_speech_str != "none") {
    talking_speech = Speech::from_string(talking_speech_str);
  }
  int type = String::extract_int(ss);
  float x = String::extract_float(ss);
  float y = String::extract_float(ss);
  float size = String::extract_float(ss);
  float orientation = String::extract_float(ss);
  int current_map_id = String::extract_int(ss);
  bool has_soulspark = String::extract_bool(ss);
  bool merchant = String::extract_bool(ss);
  int gold = String::extract_long(ss);
  int xp = String::extract_long(ss);
  int level = String::extract_int(ss);
  std::string team = String::extract(ss);
  Gear * gear = Gear::from_string(String::extract(ss));
  std::stringstream * ss_items = new std::stringstream(String::extract(ss));
  std::list<Item *> * items = new std::list<Item *>();
  while(ss_items->rdbuf()->in_avail() != 0) {
    items->push_back(Item::from_string(String::extract(ss_items)));
  }
  delete ss_items;
  std::stringstream * ss_weapons = new std::stringstream(String::extract(ss));
  std::list<Weapon *> * weapons = new std::list<Weapon *>();
  while(ss_weapons->rdbuf()->in_avail() != 0) {
    weapons->push_back(Weapon::from_string(String::extract(ss_weapons)));
  }
  delete ss_weapons;
  std::stringstream * ss_ammunition = new std::stringstream(String::extract(ss));
  std::list<Ammunition *> * ammunition = new std::list<Ammunition *>();
  while(ss_ammunition->rdbuf()->in_avail() != 0) {
    ammunition->push_back(Projectile::ammo_from_string(String::extract(ss_ammunition)));
  }
  delete ss_ammunition;
  std::stringstream * ss_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * effects = new std::list<Effect *>();
  while(ss_effects->rdbuf()->in_avail() != 0) {
    effects->push_back(Effect::from_string(String::extract(ss_effects)));
  }
  delete ss_effects;
  std::stringstream * ss_skills = new std::stringstream(String::extract(ss));
  std::list<Skill *> * skills = new std::list<Skill *>();
  while(ss_skills->rdbuf()->in_avail() != 0) {
    skills->push_back(Skill::from_string(String::extract(ss_skills)));
  }
  delete ss_skills;
  std::stringstream * ss_sellable_items = new std::stringstream(String::extract(ss));
  std::list<Item *> * sellable_items = new std::list<Item *>();
  while(ss_sellable_items->rdbuf()->in_avail() != 0) {
    sellable_items->push_back(Item::from_string(String::extract(ss_sellable_items)));
  }
  delete ss_sellable_items;
  std::stringstream * ss_sellable_weapons = new std::stringstream(String::extract(ss));
  std::list<Weapon *> * sellable_weapons = new std::list<Weapon *>();
  while(ss_sellable_weapons->rdbuf()->in_avail() != 0) {
    sellable_weapons->push_back(Weapon::from_string(String::extract(ss_sellable_weapons)));
  }
  delete ss_sellable_weapons;
  std::stringstream * ss_sellable_ammunition = new std::stringstream(String::extract(ss));
  std::list<Ammunition *> * sellable_ammunition = new std::list<Ammunition *>();
  while(ss_sellable_ammunition->rdbuf()->in_avail() != 0) {
    sellable_ammunition->push_back(Projectile::ammo_from_string(String::extract(ss_sellable_ammunition)));
  }
  delete ss_sellable_ammunition;
  std::stringstream * ss_sellable_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * sellable_effects = new std::list<Effect *>();
  while(ss_sellable_effects->rdbuf()->in_avail() != 0) {
    sellable_effects->push_back(Effect::from_string(String::extract(ss_sellable_effects)));
  }
  delete ss_sellable_effects;
  std::stringstream * ss_sellable_skills = new std::stringstream(String::extract(ss));
  std::list<Skill *> * sellable_skills = new std::list<Skill *>();
  while(ss_sellable_skills->rdbuf()->in_avail() != 0) {
    sellable_skills->push_back(Skill::from_string(String::extract(ss_sellable_skills)));
  }
  delete ss_sellable_skills;
  Attributes * attributes = Attributes::from_string(String::extract(ss));
  std::string second_attributes_str = String::extract(ss);
  Attributes * second_attributes = nullptr;
  if(second_attributes_str != "none") {
    second_attributes = Attributes::from_string(second_attributes_str);
  }
  Way * race = Way::from_string(String::extract(ss));
  Way * origin = Way::from_string(String::extract(ss));
  Way * culture = Way::from_string(String::extract(ss));
  Way * religion = Way::from_string(String::extract(ss));
  Way * profession = Way::from_string(String::extract(ss));
  std::stringstream * ss_titles = new std::stringstream(String::extract(ss));
  std::list<Way *> * titles = new std::list<Way *>();
  while(ss_titles->rdbuf()->in_avail() != 0) {
    titles->push_back(Way::from_string(String::extract(ss_titles)));
  }
  delete ss_titles;
  delete ss;
  Character * result = new Character(
    maxHp,
    maxMana,
    hp,
    mana,
    armor,
    armor_multiplier,
    damage_multiplier,
    soulBurnTreshold,
    flow,
    visionRange,
    visionPower,
    detectionRange,
    currentSoulBurn,
    stamina,
    satiety,
    savedHpRegen,
    savedManaRegen,
    channeledMana,
    name,
    id,
    player_character,
    death_speech,
    talking_speech,
    type,
    x,
    y,
    size,
    orientation,
    current_map_id,
    has_soulspark,
    merchant,
    gold,
    xp,
    level,
    team,
    gear,
    *items,
    *weapons,
    *ammunition,
    *effects,
    *skills,
    *sellable_items,
    *sellable_weapons,
    *sellable_ammunition,
    *sellable_effects,
    *sellable_skills,
    attributes,
    second_attributes,
    race,
    origin,
    culture,
    religion,
    profession,
    *titles
  );
  delete items;
  delete weapons;
  delete ammunition;
  delete effects;
  delete skills;
  delete sellable_items;
  delete sellable_weapons;
  delete sellable_ammunition;
  delete sellable_effects;
  delete sellable_skills;
  delete titles;
  return result;
}

void Character::deepDelete() {
  for(Item * item : items) {
    delete item;
  }
  for(Item * item : race->getLoot()) {
    delete item;
  }
  for(Weapon * weapon : weapons) {
    delete weapon;
  }
  for(Ammunition * ammo : ammunition) {
    delete ammo;
  }
  for(Skill * skill : skills) {
    delete skill;
  }
  for(Way * title : titles) {
    delete title;
  }
  delete gear;
  delete race;
  delete origin;
  delete culture;
  delete religion;
  delete profession;
}
