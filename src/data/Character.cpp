#include <sstream>

#include "ai/AI.h"
#include "data/Database.h"
#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/Settings.h"
#include "data/Speech.h"
#include "data/Way.h"
#include "data/Race.h"
#include "data/World.h"

#include "data/items/Gear.h"
#include "data/items/WeaponItem.h"
#include "data/items/ArmorItem.h"
#include "data/items/SerialItem.h"
#include "data/items/AmmunitionItem.h"
#include "data/items/Item.h"

#include "communication/SpeechManager.h"

#include "data/Character.h"

#include "util/String.h"

void Character::initializeCharacter(Gear * gear) {
  size = race->getSize(race_modifiers);
  maxHp = attributes->baseHp + race->getBaseHp(race_modifiers) + origin->baseHp + culture->baseHp + religion->baseHp + profession->baseHp;
  for(Way * way : titles) {
    maxHp += way->baseHp;
  }
  maxMana = attributes->baseMana + race->getBaseMana(race_modifiers) + origin->baseMana + culture->baseMana + religion->baseMana + profession->baseMana;
  for(Way * way : titles) {
    maxHp += way->baseMana;
  }
  hp = maxHp;
  mana = maxMana;
  armor = race->getBaseArmor(race_modifiers);
  armor_multiplier = attributes->baseArmorMult + race->getBaseArmorMult(race_modifiers) + origin->baseArmorMult + culture->baseArmorMult + religion->baseArmorMult + profession->baseArmorMult;
  for(Way * way : titles) {
    maxHp += way->baseArmorMult;
  }
  damage_multiplier = attributes->baseDamageMult + race->getBaseDamageMult(race_modifiers) + origin->baseDamageMult + culture->baseDamageMult + religion->baseDamageMult + profession->baseDamageMult;
  for(Way * way : titles) {
    maxHp += way->baseDamageMult;
  }
  soulBurnTreshold = attributes->baseSoulBurn + race->getBaseSoulBurn(race_modifiers) + origin->baseSoulBurn + culture->baseSoulBurn + religion->baseSoulBurn + profession->baseSoulBurn;
  for(Way * way : titles) {
    maxHp += way->baseSoulBurn;
  }
  flow = attributes->baseFlow + race->getBaseFlow(race_modifiers) + origin->baseFlow + culture->baseFlow + religion->baseFlow + profession->baseFlow;
  for(Way * way : titles) {
    maxHp += way->baseFlow;
  }
  visionRange = attributes->baseVisionRange + race->getBaseVisionRange(race_modifiers) + origin->baseVisionRange + culture->baseVisionRange + religion->baseVisionRange + profession->baseVisionRange;
  for(Way * way : titles) {
    maxHp += way->baseVisionRange;
  }
  visionPower = attributes->baseVisionPower + race->getBaseVisionPower(race_modifiers) + origin->baseVisionPower + culture->baseVisionPower + religion->baseVisionPower + profession->baseVisionPower;
  for(Way * way : titles) {
    maxHp += way->baseVisionPower;
  }
  detectionRange = attributes->baseDetectionRange + race->getBaseDetectionRange(race_modifiers) + origin->baseDetectionRange + culture->baseDetectionRange + religion->baseDetectionRange + profession->baseDetectionRange;
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
    Item * toadd;
    if(item->type == ITEM_MISCELLANEOUS || item->type == ITEM_CONSUMABLE || item->type == ITEM_AMMUNITION) {
      toadd = Item::init(item, item->tier, ((SerialItem *) item)->getNumber());
    }
    else {
      toadd = Item::init(item, item->tier, 1);
    }
    items.push_back(toadd);
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
  for(Skill * skill : race->getSkills(race_modifiers)) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : race->getEffects(race_modifiers)) {
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
float Character::getZ() { return z; }
float Character::getSize() { return size; }
float Character::getOrientation() { return orientation; }
int Character::getHp() { return hp; }
int Character::getMaxHp() {
  int bonus = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_HP_MAX) {
      bonus += e->power;
    }
  }
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
  for(Effect * e : effects) {
    if(e->type == EFFECT_MANA_MAX) {
      bonus += e->power;
    }
  }
  return std::max(maxMana + bonus, 0);
}

float Character::getStamina() { return stamina; }
float Character::getSatiety() { return satiety; }

int Character::getArmor() {
  int result = (int) std::ceil( (float) (armor + gear->getArmor()) * getArmorMultiplier());
  for(Effect * e : effects) {
    if(e->type == EFFECT_ARMOR) {
      result += e->power;
    }
  }
  return std::max(result, 0);
}

float Character::getArmorMultiplier() {
  int result = armor_multiplier;
  for(Effect * e : effects) {
    if(e->type == EFFECT_ARMOR_MULTIPLIER) {
      result += e->power;
    }
  }
  return std::max(0.F, 1.F + ((float) result) / 100.F);
}

int Character::getSoulBurnTreshold() {
  int bonus = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_SOULBURNTRESHOLD) {
      bonus += e->power;
    }
  }
  return std::max(soulBurnTreshold + bonus, 0);
}

int Character::getCurrentSoulBurn() { return currentSoulBurn; }

int Character::getFlow() {
  int bonus = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_FLOW) {
      bonus += e->power;
    }
  }
  return std::max(flow + bonus, 0);
}

int Character::getVisionRange() {
  int bonus = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_VISION_RANGE) {
      bonus += e->power;
    }
  }
  return std::max(visionRange + bonus, 0);
}

int Character::getVisionPower() {
  int bonus = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_VISION_POWER) {
      bonus += e->power;
    }
  }
  return std::max(visionPower + bonus, 0);
}

int Character::getDetectionRange() {
  int bonus = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_DETECTION_RANGE) {
      bonus += e->power;
    }
  }
  return std::max(detectionRange + bonus, 0);
}

long Character::getGold() { return gold; }
long Character::getXP() { return xp; }
int Character::getLevel() { return level; }

float Character::getDamageMultiplier() {
  int result = damage_multiplier;
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_MULTIPLIER) {
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
int Character::getType() { return race->getType(race_modifiers); }

Map * Character::getCurrentMap() { return current_map; }

Gear * Character::getGear() { return gear; }

float Character::getActionTimeModifier() {
  float result = race->getActionTimeModifier(race_modifiers);
  for(Effect * e : effects) {
    if(e->type == EFFECT_ACTION_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result;
}

float Character::getHandActionTimeModifier() {
  float result = race->getStrikeTimeModifier(race_modifiers);
  for(Effect * e : effects) {
    if(e->type == EFFECT_HAND_ACTION_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result * getActionTimeModifier();
}

float Character::getSkillTimeModifier() {
  float result = race->getSkillTimeModifier(race_modifiers);
  for(Effect * e : effects) {
    if(e->type == EFFECT_SKILL_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result * getActionTimeModifier();
}

float Character::getMovementTimeModifier() {
  float result = race->getMovementTimeModifier(race_modifiers);
  for(Effect * e : effects) {
    if(e->type == EFFECT_MOVEMENT_TIME_MODIFIER) {
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

float Character::getSwapTime(long item_id) {
  return (float) gear->getWeapon()->swap_time / getHandActionTimeModifier();
}

float Character::getUseTime(long item_id) {
  for(Item * item : items) {
    if(item->id == item_id) {
      return (float) item->use_time / getHandActionTimeModifier();
    }
  }
}

int Character::getLight() {
  int light = 0;
  for(Effect * effect : effects) {
    if(effect->type == EFFECT_LIGHT) {
      light += effect->power;
    }
  }
  return light;
}
std::list<Item *> Character::getItems() { return items; }
std::list<Item *> Character::getLoot() { return race->getLoot(race_modifiers); }
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
std::list<Effect *> Character::getSellableEffects() { return sellable_effects; }
std::list<Skill *> Character::getSellableSkills() { return sellable_skills; }

Attributes * Character::getAttributes() { return attributes; }
Attributes * Character::getSecondAttributes() { return second_attributes; }
Race * Character::getRace() { return race; }
Way * Character::getOrigin() { return origin; }
Way * Character::getCulture() { return culture; }
Way * Character::getReligion() { return religion; }
Way * Character::getProfession() { return profession; }

std::list<Way *> Character::getTitles() { return titles; }

void Character::setOrientation(float orientation) { this->orientation = orientation; }
void Character::setSize(float size) { this->size = size; }
void Character::move(float x, float y, float z, float orientation, World * world) {
  Map * new_map = world->getMap(x, y, z);
  if(current_map != nullptr && current_map->id != new_map->id) {
    world->getMap(this->x, this->y, this->z)->removeCharacter(this);
    new_map->addCharacter(this);
  }
  this->x = x;
  this->y = y;
  this->z = z;
  this->orientation = orientation;
}

void Character::hpHeal(int hp) { this->hp = std::min(this->hp + hp, getMaxHp()); }
void Character::incrMaxHp() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += attributes->hpIncr;
  incr += second_attributes->hpIncr;
  incr += race->getHpIncr(race_modifiers);
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
  incr += race->getManaIncr(race_modifiers);
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
  incr += race->getArmorMultIncr(race_modifiers);
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
  incr += race->getDamageMultIncr(race_modifiers);
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
  incr += race->getSoulBurnIncr(race_modifiers);
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
  incr += race->getFlowIncr(race_modifiers);
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

void Character::setCurrentMap(Map * map) {
  if(this->current_map != nullptr) {
    delete this->current_map;
  }
  this->current_map = map;
}

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
  if(race->getNeedToSleep(race_modifiers)) {
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
  if(race->getNeedToEat(race_modifiers)) {
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
    if(e->duration_type == DURATION_TEMPORARY) {
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
  if(race->getNeedToSleep(race_modifiers)) {
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
  for(Skill * skill : race->getSkills(race_modifiers)) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : race->getEffects(race_modifiers)) {
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

void Character::equip(GearItem * to_equip) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(to_equip != nullptr) {
    this->items.remove(to_equip);
    std::list<GearItem *> items = gear->equip(to_equip);
    for(GearItem * item : items) {
      for(Effect * e : item->effects) {
        removeEffect(e);
      }
      this->items.push_back(item);
    }
    for(Effect * e : to_equip->effects) {
      addEffect(e);
    }
  }
}

void Character::unequip(int type, int type2) {
  if(player_character) {
    setNeedToSend(true);
  }
  Item * old_item = gear->unequip(type, type2);
  if(old_item != nullptr) {
    for(Effect * e : old_item->effects) {
      removeEffect(e);
    }
    items.push_back(old_item);
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
    case WAY_RACE:
      to_remove = race;
      race = (Race *) way;
      break;
    case WAY_ORIGIN:
      to_remove = origin;
      origin = way;
      break;
    case WAY_CULTURE:
      to_remove = culture;
      culture = way;
      break;
    case WAY_RELIGION:
      to_remove = religion;
      religion = way;
      break;
    case WAY_PROFESSION:
      to_remove = profession;
      profession = way;
      break;
    case WAY_RACE_MODIFIER:
      for(Way * modifier : race_modifiers) {
        if(modifier == way) {
          return;
        }
      }
      race_modifiers.push_back( (Race *) way);
    case WAY_TITLE:
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
  if(i->type == ITEM_MISCELLANEOUS || i->type == ITEM_CONSUMABLE || i->type == ITEM_AMMUNITION) {
    for(Item * item : items) {
      if(i->name == item->name && i->tier == item->tier) {
        ( (SerialItem *) item)->add(( (SerialItem *) i)->getNumber());
        delete i;
        break;
      }
    }
    items.push_back(i);
  }
  else {
    items.push_back(i);
  }
}

void Character::removeItem(Item * i) {
  if(player_character) {
    setNeedToSend(true);
  }
  items.remove(i);
}

void Character::useItem(long item_id) {
  Item * to_remove = nullptr;
  for(Item * i : items) {
    if(i->id == item_id && i->usable) {
      if(player_character) {
        setNeedToSend(true);
      }
      if(i->type == ITEM_CONSUMABLE && !((SerialItem *) i)->isFood() || race->getCanEatFood(race_modifiers)) {
        for(Effect * e : i->effects) {
          e->activate(this);
        }
        if(((SerialItem *) i)->reduce(1) == 0) {
          to_remove = i;
        }
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
    if(e->type == EFFECT_FLY) {
      return true;
    }
  }
  return false;
}

bool Character::isChanneling() {
  for(Effect * e : effects) {
    if(e->type == EFFECT_CHANNELING) {
      return true;
    }
  }
  return false;
}

bool Character::isStunned() {
  for(Effect * e : effects) {
    if(e->type == EFFECT_STUNNED) {
      return true;
    }
  }
  return false;
}

bool Character::isCloaked() {
  for(Effect * e : effects) {
    if(e->type == EFFECT_CLOAKED) {
      return true;
    }
  }
  return false;
}

bool Character::isInvisible() {
  for(Effect * e : effects) {
    if(e->type == EFFECT_INVISIBLE) {
      return true;
    }
  }
  return false;
}

bool Character::isEtheral() {
  for(Effect * e : effects) {
    if(e->type == EFFECT_ETHERAL) {
      return true;
    }
  }
  return false;
}

bool Character::isInvulnerable() {
  for(Effect * e : effects) {
    if(e->type == EFFECT_INVULNERABLE) {
      return true;
    }
  }
  return false;
}

bool Character::isSleeping() {
  for(Effect * e : effects) {
    if(e->type == EFFECT_SLEEPING) {
      return true;
    }
  }
  return false;
}

int Character::cloakPower() {
  int max = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_CLOAKED) {
      if(e->power > max) {
        max = e->power;
      }
    }
  }
  return max;
}

bool Character::isInWeakState() {
  for(Effect * e : effects) {
    if(e->type == EFFECT_STUNNED || e->type == EFFECT_SLEEPING) {
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
    if(e->type == EFFECT_DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type);
    }
  }
  return (int) std::ceil((float) damage * getDamageMultiplier());
}

float Character::getDamageReductionFromType(int damage_type) {
  float reduction = gear->getDamageReductionFromType(damage_type);
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_REDUCTION) {
      reduction += e->getDamageReductionFromType(damage_type);
    }
  }
  return reduction;
}

Projectile * Character::shoot(Target * target, Adventure * adventure) {
  if(gear->getWeapon()->use_projectile && gear->getWeapon()->range >= std::max(abs(x - target->x), abs(y - target->y))) {
    if(!gear->getWeapon()->use_ammo || gear->getWeapon()->getCurrentCapacity() > 0) {
      int realDamages[DAMAGE_TYPE_NUMBER];
      for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
        realDamages[damage_type] = getDamageFromType(damage_type);
      }
      Projectile * base_projectile = (Projectile *) gear->getWeapon()->getAmmo()->getProjectile();
      if(gear->getWeapon()->use_ammo) {
        if(player_character) {
          setNeedToSend(true);
        }
        gear->getWeapon()->useAmmo();
      }
      return new Projectile(base_projectile, realDamages, adventure->getWorld(), current_map->id, target, this, 1, 1, 1, true);
    }
  }
  return nullptr;
}

void Character::attack(Character * target, int type) {
  if(!gear->getWeapon()->use_projectile && gear->getWeapon()->range >= std::max(abs(x - target->getX()), abs(y - target->getY()))) {
    int realDamages[DAMAGE_TYPE_NUMBER];
    for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
      realDamages[damage_type] = getDamageFromType(damage_type);
    }
    target->receiveAttack(realDamages, orientation, type);
  }
}

void Character::reload(AmmunitionItem * ammo) {
  if(player_character) {
    setNeedToSend(true);
  }
  AmmunitionItem * oldAmmo = gear->getWeapon()->reload(ammo);
  if(ammo->getNumber() == 0) {
    items.remove(ammo);
    delete ammo;
    ammo = nullptr;
  }
  if(oldAmmo != nullptr) {
    items.push_back(oldAmmo);
  }
}


AmmunitionItem * Character::canReload() {
  AmmunitionItem * ammo = nullptr;
  int number = 0;
  for(Item * current : items) {
    if(current->type == ITEM_AMMUNITION && gear->getWeapon()->ammo_type == current->type2 && ( (AmmunitionItem *) current)->getNumber() > number) {
      ammo = (AmmunitionItem *) current;
      number = ((AmmunitionItem *) current)->getNumber();
    }
  }
  return ammo;
}

WeaponItem * Character::swapMelee() {
  WeaponItem * weapon = nullptr;
  int rawDamage = 0;
  for(Item * current : items) {
    if(current->type == ITEM_WEAPON) {
      int currentRawDamage = ( (WeaponItem *) current)->getRawDamage();
      if(currentRawDamage > rawDamage) {
        weapon = (WeaponItem *) current;
        rawDamage = currentRawDamage;
      }
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
      if(damage_type == DAMAGE_ACID) {
        damage += damages[damage_type];
      }
      if(damage_type == DAMAGE_MIND) {
        hp -= std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
      }
      if(damage_type == DAMAGE_TRUE) {
        hp -= damages[damage_type];
      }
      if(damage_type == DAMAGE_SOUL) {
        payMana(damages[damage_type]);
      }
      else {
        damage += std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
      }
    }
    if(type == ACTION_HEAVY_STRIKE) {
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
      if(damage_type == DAMAGE_ACID) {
        damage += damages[damage_type] * 2;
      }
      if(damage_type == DAMAGE_MIND) {
        float damage_reduction = getDamageReductionFromType(damage_type);
        if(damage_reduction > 0.F) {
          hp -= std::max(0, (int) floor( (float) (damages[damage_type] * 2) * (1.F - .5 * damage_reduction)));
        }
        else {
          hp -= std::max(0, (int) floor( (float) (damages[damage_type] * 2) * (1.F - damage_reduction)));
        }
      }
      if(damage_type == DAMAGE_TRUE) {
        hp -= damages[damage_type] * 2;
      }
      if(damage_type == DAMAGE_SOUL) {
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
    if(type == ACTION_HEAVY_STRIKE) {
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
    if(damage_type == DAMAGE_ACID) {
      damage += damages[damage_type];
    }
    if(damage_type == DAMAGE_MIND) {
      trueDamage += std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
    }
    if(damage_type == DAMAGE_TRUE) {
      trueDamage += damages[damage_type];
    }
    if(damage_type == DAMAGE_SOUL) {
      trueDamage += damages[damage_type];
    }
    else {
      damage += std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
    }
  }
  if(type == ACTION_HEAVY_STRIKE) {
    return (trueDamage + std::max(0, damage - getArmor())) * 1.5;
  }
  else {
    return trueDamage + std::max(0, damage - getArmor());
  }
}

void Character::trade(Character * buyer, int object_type, std::string object_name, float price_modifier) {
  int price = 0;
  switch (object_type) {
    case OBJECT_ITEM:
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
    case OBJECT_SKILL:
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
    case OBJECT_EFFECT:
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

std::string Character::to_string() {
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
  String::insert_int(ss, race->getType(race_modifiers));
  String::insert_float(ss, x);
  String::insert_float(ss, y);
  String::insert_float(ss, z);
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
  display->z = String::extract_float(ss);
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
  String::insert_float(ss, x);
  String::insert_float(ss, y);
  String::insert_float(ss, z);
  String::insert_float(ss, size);
  String::insert_float(ss, orientation);
  if(current_map != nullptr) {
    String::insert_long(ss, current_map->id);
  }
  else {
    String::insert_long(ss, -1);
  }
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
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  std::stringstream * ss_skills = new std::stringstream();
  for(Skill * skill : skills) {
    String::insert(ss_skills, skill->name);
  }
  String::insert(ss, ss_skills->str());
  delete ss_skills;
  std::stringstream * ss_sellable_items = new std::stringstream();
  for(Item * item : sellable_items) {
    String::insert(ss_sellable_items, item->to_string());
  }
  String::insert(ss, ss_sellable_items->str());
  delete ss_sellable_items;
  std::stringstream * ss_sellable_effects = new std::stringstream();
  for(Effect * effect : sellable_effects) {
    String::insert(ss_sellable_effects, effect->name);
  }
  String::insert(ss, ss_sellable_effects->str());
  delete ss_sellable_effects;
  std::stringstream * ss_sellable_skills = new std::stringstream();
  for(Skill * skill : sellable_skills) {
    String::insert(ss_sellable_skills, skill->name);
  }
  String::insert(ss, ss_sellable_skills->str());
  delete ss_sellable_skills;

  String::insert(ss, attributes->name);
  if(second_attributes != nullptr) {
    String::insert(ss, ((Attributes *) second_attributes)->name);
  }
  else {
    String::insert(ss, "none");
  }
  String::insert(ss, race->name);
  std::stringstream * ss_race_modifiers = new std::stringstream();
  for(Race * modifier : race_modifiers) {
    String::insert(ss_race_modifiers, modifier->name);
  }
  String::insert(ss, ss_race_modifiers->str());
  delete ss_race_modifiers;
  String::insert(ss, origin->name);
  String::insert(ss, culture->name);
  String::insert(ss, religion->name);
  String::insert(ss, profession->name);
  std::stringstream * ss_titles = new std::stringstream();
  for(Way * title : titles) {
    String::insert(ss_titles, title->name);
  }
  String::insert(ss, ss_titles->str());
  delete ss_titles;
  std::string result = ss->str();
  delete ss;
  return result;
}

Character * Character::full_from_string(std::string to_read, Adventure * adventure) {
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
  float x = String::extract_float(ss);
  float y = String::extract_float(ss);
  float z = String::extract_float(ss);
  float size = String::extract_float(ss);
  float orientation = String::extract_float(ss);
  Map * current_map = nullptr;
  long map_id = String::extract_long(ss);
  if(map_id != -1) {
    current_map = adventure->getWorld()->getMap(String::extract_int(ss));
  }
  bool merchant = String::extract_bool(ss);
  int gold = String::extract_long(ss);
  int xp = String::extract_long(ss);
  int level = String::extract_int(ss);
  std::string team = String::extract(ss);
  Gear * gear = Gear::from_string(String::extract(ss), adventure);
  std::stringstream * ss_items = new std::stringstream(String::extract(ss));
  std::list<Item *> * items = new std::list<Item *>();
  while(ss_items->rdbuf()->in_avail() != 0) {
    items->push_back(Item::from_string(String::extract(ss_items), adventure));
  }
  delete ss_items;
  std::stringstream * ss_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * effects = new std::list<Effect *>();
  while(ss_effects->rdbuf()->in_avail() != 0) {
    effects->push_back(Effect::from_string(String::extract(ss_effects)));
  }
  delete ss_effects;
  std::stringstream * ss_skills = new std::stringstream(String::extract(ss));
  std::list<Skill *> * skills = new std::list<Skill *>();
  while(ss_skills->rdbuf()->in_avail() != 0) {
    skills->push_back( (Skill *) adventure->getDatabase()->getSkill((String::extract(ss_skills))));
  }
  delete ss_skills;
  std::stringstream * ss_sellable_items = new std::stringstream(String::extract(ss));
  std::list<Item *> * sellable_items = new std::list<Item *>();
  while(ss_sellable_items->rdbuf()->in_avail() != 0) {
    sellable_items->push_back(Item::from_string(String::extract(ss_sellable_items), adventure));
  }
  delete ss_sellable_items;
  std::stringstream * ss_sellable_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * sellable_effects = new std::list<Effect *>();
  while(ss_sellable_effects->rdbuf()->in_avail() != 0) {
    sellable_effects->push_back( (Effect *) adventure->getDatabase()->getEffect(String::extract(ss_sellable_effects)));
  }
  delete ss_sellable_effects;
  std::stringstream * ss_sellable_skills = new std::stringstream(String::extract(ss));
  std::list<Skill *> * sellable_skills = new std::list<Skill *>();
  while(ss_sellable_skills->rdbuf()->in_avail() != 0) {
    sellable_skills->push_back( (Skill *) adventure->getDatabase()->getSkill(String::extract(ss_sellable_skills)));
  }
  delete ss_sellable_skills;
  Attributes * attributes = (Attributes *) adventure->getDatabase()->getAttributes(String::extract(ss));
  std::string second_attributes_str = String::extract(ss);
  Attributes * second_attributes = nullptr;
  if(second_attributes_str != "none") {
    second_attributes = (Attributes *) adventure->getDatabase()->getAttributes(second_attributes_str);
  }
  Race * race = (Race *) adventure->getDatabase()->getWay(String::extract(ss));
  std::stringstream * ss_race_modifiers = new std::stringstream(String::extract(ss));
  std::list<Race *> * race_modifiers = new std::list<Race *>();
  while(ss_race_modifiers->rdbuf()->in_avail() != 0) {
    race_modifiers->push_back((Race *) adventure->getDatabase()->getWay(String::extract(ss_race_modifiers)));
  }
  delete ss_race_modifiers;
  Way * origin = (Way *) adventure->getDatabase()->getWay(String::extract(ss));
  Way * culture = (Way *) adventure->getDatabase()->getWay(String::extract(ss));
  Way * religion = (Way *) adventure->getDatabase()->getWay(String::extract(ss));
  Way * profession = (Way *) adventure->getDatabase()->getWay(String::extract(ss));
  std::stringstream * ss_titles = new std::stringstream(String::extract(ss));
  std::list<Way *> * titles = new std::list<Way *>();
  while(ss_titles->rdbuf()->in_avail() != 0) {
    titles->push_back((Way *) adventure->getDatabase()->getWay(String::extract(ss_titles)));
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
    x,
    y,
    z,
    size,
    orientation,
    current_map,
    merchant,
    gold,
    xp,
    level,
    team,
    gear,
    *items,
    *effects,
    *skills,
    *sellable_items,
    *sellable_effects,
    *sellable_skills,
    attributes,
    second_attributes,
    race,
    *race_modifiers,
    origin,
    culture,
    religion,
    profession,
    *titles
  );
  delete items;
  delete effects;
  delete skills;
  delete sellable_items;
  delete sellable_effects;
  delete sellable_skills;
  delete race_modifiers;
  delete titles;
  return result;
}

void Character::deepDelete() {
  for(Item * item : items) {
    delete item;
  }
  for(Item * item : race->getLoot(race_modifiers)) {
    delete item;
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
