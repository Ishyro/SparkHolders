#include <sstream>

#include "ai/AI.h"
#include "data/Database.h"
#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/Settings.h"
#include "data/Speech.h"
#include "data/ways/Way.h"
#include "data/ways/Race.h"
#include "data/World.h"

#include "data/actions/Action.h"

#include "data/items/Gear.h"
#include "data/items/WeaponItem.h"
#include "data/items/ArmorItem.h"
#include "data/items/SerialItem.h"
#include "data/items/AmmunitionItem.h"
#include "data/items/Item.h"

#include "communication/SpeechManager.h"

#include "data/Character.h"

#include "util/String.h"

Character::~Character() {
      delete ai;
      ai = nullptr;
      for(Effect * effect : effects) {
        delete effect;
        effect = nullptr;
      }
    }

void Character::initializeCharacter(Gear * gear) {
  size = race->getSize(race_modifiers);
  height = race->getHeight(race_modifiers);
  maxHp = main_class->baseHp + race->getBaseHp(race_modifiers) + origin->baseHp + culture->baseHp + religion->baseHp + profession->baseHp;
  for(Way * way : titles) {
    maxHp += way->baseHp;
  }
  maxMana = main_class->baseMana + race->getBaseMana(race_modifiers) + origin->baseMana + culture->baseMana + religion->baseMana + profession->baseMana;
  for(Way * way : titles) {
    maxMana += way->baseMana;
  }
  maxShield = main_class->baseShield + race->getBaseShield(race_modifiers) + origin->baseShield + culture->baseShield + religion->baseShield + profession->baseShield;
  for(Way * way : titles) {
    maxShield += way->baseShield;
  }
  hp = maxHp;
  mana = maxMana;
  shield = maxShield;
  damage_multiplier = main_class->baseDamageMult + race->getBaseDamageMult(race_modifiers) + origin->baseDamageMult + culture->baseDamageMult + religion->baseDamageMult + profession->baseDamageMult;
  for(Way * way : titles) {
    damage_multiplier += way->baseDamageMult;
  }
  soulBurnTreshold = main_class->baseSoulBurn + race->getBaseSoulBurn(race_modifiers) + origin->baseSoulBurn + culture->baseSoulBurn + religion->baseSoulBurn + profession->baseSoulBurn;
  for(Way * way : titles) {
    soulBurnTreshold += way->baseSoulBurn;
  }
  flow = main_class->baseFlow + race->getBaseFlow(race_modifiers) + origin->baseFlow + culture->baseFlow + religion->baseFlow + profession->baseFlow;
  for(Way * way : titles) {
    flow += way->baseFlow;
  }
  visionRange = main_class->baseVisionRange + race->getBaseVisionRange(race_modifiers) + origin->baseVisionRange + culture->baseVisionRange + religion->baseVisionRange + profession->baseVisionRange;
  for(Way * way : titles) {
    visionRange += way->baseVisionRange;
  }
  visionPower = main_class->baseVisionPower + race->getBaseVisionPower(race_modifiers) + origin->baseVisionPower + culture->baseVisionPower + religion->baseVisionPower + profession->baseVisionPower;
  for(Way * way : titles) {
    visionPower += way->baseVisionPower;
  }
  detectionRange = main_class->baseDetectionRange + race->getBaseDetectionRange(race_modifiers) + origin->baseDetectionRange + culture->baseDetectionRange + religion->baseDetectionRange + profession->baseDetectionRange;
  for(Way * way : titles) {
    detectionRange += way->baseDetectionRange;
  }
  currentSoulBurn = 0;
  stamina = 75.;
  satiety = 75.;
  savedHpRegen = 0.;
  savedManaRegen = 0.;
  channeledMana = 0;
  this->gear = new Gear(gear);
  initSkillsAndEffects();
}
void Character::initSkillsAndEffects() {
  for(Skill * skill : main_class->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : main_class->getEffects()) {
    if(effect->level == 0) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : second_class->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : second_class->getEffects()) {
    if(effect->level == 0) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : spec_class->getSkills()) {
    if(skill->level == 0) {
      addSkill(skill);
    }
  }
  for(Effect * effect : spec_class->getEffects()) {
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
bool Character::isMarkedDead() { return dead; }
void Character::markDead(bool dead) { this->dead = dead; }
bool Character::isSoulBurning() { return currentSoulBurn >= soulBurnTreshold; }
MapUtil::Vector3 Character::getCoord() { return coord; }
float Character::getSize() { return size; }
float Character::getHeight() { return height; }
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
int Character::getChanneledMana() { return channeledMana; }

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
int Character::getShield() { return shield; }

int Character::getMaxShield() {
  int bonus = 0;
  for(Effect * e : effects) {
    if(e->type == EFFECT_SHIELD_MAX) {
      bonus += e->power;
    }
  }
  return std::max(maxShield + bonus, 0);
}

int Character::getSoulBurnThreshold() {
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

long Character::getRawPower() {
  return getMaxHp() + getMaxMana() + getMaxShield() + getSoulBurnThreshold() + 5 * (getFlow() + (getDamageMultiplier() - 1.F) * 100.F);
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
  return getMaxHp() + getMaxMana() + 5 * (getFlow() + getSoulBurnThreshold() + getShield() + getDamageMultiplier());
}

bool Character::needToSend() { return need_to_send; }
void Character::setNeedToSend(bool need_to_send) { this->need_to_send = need_to_send; }

AI * Character::getAI() { return ai; }
std::string Character::getTeam() { return team; }
Speech * Character::getDeathSpeech() { return death_speech; }
Speech * Character::getTalkingSpeech() { return talking_speech; }
int Character::getType() { return race->getType(race_modifiers); }

Region * Character::getRegion() { return region; }
Action * Character::getCurrentAction() { return current_action; }

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

float Character::getStrikeTime(int slot) {
  if(slot == ITEM_SLOT_WEAPON_1) {
    return (float) gear->getWeapon_1()->strike_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_2) {
    return (float) gear->getWeapon_2()->strike_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_3) {
    return (float) gear->getBackupWeapon_1()->strike_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_4) {
    return (float) gear->getBackupWeapon_2()->strike_time / getHandActionTimeModifier();
  }
  return 0.F;
}

float Character::getReloadTime(int slot) {
  if(slot == ITEM_SLOT_WEAPON_1) {
    return (float) gear->getWeapon_1()->reload_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_2) {
    return (float) gear->getWeapon_2()->reload_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_3) {
    return (float) gear->getBackupWeapon_1()->reload_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_4) {
    return (float) gear->getBackupWeapon_2()->reload_time / getHandActionTimeModifier();
  }
  return 0.F;
}

float Character::getSwapTime(int slot1, int slot2) {
  if(slot1 == ITEM_SLOT_WEAPON_1) {
    if(slot2 == ITEM_SLOT_WEAPON_3) {
      return ((float) gear->getWeapon_1()->swap_time + gear->getBackupWeapon_1()->swap_time) / getHandActionTimeModifier();
    }
    else if(slot2 == ITEM_SLOT_WEAPON_4) {
      return ((float) gear->getWeapon_1()->swap_time + gear->getBackupWeapon_2()->swap_time) / getHandActionTimeModifier();
    }
  }
  else if(slot1 == ITEM_SLOT_WEAPON_2) {
    if(slot2 == ITEM_SLOT_WEAPON_3) {
      return ((float) gear->getWeapon_2()->swap_time + gear->getBackupWeapon_1()->swap_time) / getHandActionTimeModifier();
    }
    else if(slot2 == ITEM_SLOT_WEAPON_4) {
      return ((float) gear->getWeapon_2()->swap_time + gear->getBackupWeapon_2()->swap_time) / getHandActionTimeModifier();
    }
  }
  return 0.F;
}

float Character::getUseTime(Item * item) {
  return (float) item->use_time / getHandActionTimeModifier();
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
    damages[i] = getDamageFromType(i, ITEM_SLOT_WEAPON_1);
  }
  result.insert(std::make_pair(nullptr, damages));
  return result;
}

std::list<Item *> Character::getSellableItems() { return sellable_items; }
std::list<Effect *> Character::getSellableEffects() { return sellable_effects; }
std::list<Skill *> Character::getSellableSkills() { return sellable_skills; }

Attributes * Character::getMainClass() { return main_class; }
Attributes * Character::getSecondClass() { return second_class; }
Attributes * Character::getSpecClass() { return spec_class; }
Race * Character::getRace() { return race; }
std::list<Race *> Character::getRaceModifiers() { return race_modifiers; }
Way * Character::getOrigin() { return origin; }
Way * Character::getCulture() { return culture; }
Way * Character::getReligion() { return religion; }
Way * Character::getProfession() { return profession; }

std::list<Way *> Character::getTitles() { return titles; }

void Character::setOrientation(float orientation) { this->orientation = orientation; }
void Character::setSize(float size) { this->size = size; }
void Character::move(MapUtil::Vector3 coord, float orientation, World * world) {
  MapUtil::Vector3 ori = this->coord;
  this->coord = coord;
  this->orientation = orientation;
  world->checkRegion(this, ori, coord);
}

void Character::hpHeal(int hp) { this->hp = std::min(this->hp + hp, getMaxHp()); }
void Character::incrMaxHp() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += main_class->hpIncr;
  incr += second_class->hpIncr;
  incr += spec_class->hpIncr;
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

void Character::manaHeal(int mana) { this->mana = std::min(this->mana + mana, getMaxMana()); }

void Character::incrMaxMana() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += main_class->manaIncr;
  incr += second_class->manaIncr;
  incr += spec_class->manaIncr;
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
void Character::shieldRestore(int shield) { this->shield = std::min(getMaxShield(), this->shield + shield); }
void Character::setShield(int shield) { this->shield = shield; }
void Character::incrMaxShield() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += main_class->shieldIncr;
  incr += second_class->shieldIncr;
  incr += spec_class->shieldIncr;
  incr += race->getShieldIncr(race_modifiers);
  incr += origin->shieldIncr;
  incr += culture->shieldIncr;
  incr += religion->shieldIncr;
  incr += profession->shieldIncr;
  for(Way * title : titles) {
    incr += title->shieldIncr;
  }
  maxShield += std::max(incr, 0);
}
void Character::addStamina(float stamina) { this->stamina = std::min(100.F, this->stamina + stamina); }
void Character::addSatiety(float satiety) { this->satiety = std::min(100.F, this->satiety + satiety); }
void Character::removeStamina(float stamina) { this->stamina = std::max(0.F, this->stamina - stamina); }
void Character::removeSatiety(float satiety) { this->satiety = std::max(0.F, this->satiety - satiety); }
void Character::setStamina(float stamina) { this->stamina = stamina; }
void Character::setSatiety(float satiety) { this->satiety = satiety; }

void Character::incrDamageMultiplier() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += main_class->damageMultIncr;
  incr += second_class->damageMultIncr;
  incr += spec_class->damageMultIncr;
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
  incr += main_class->soulBurnIncr;
  incr += second_class->soulBurnIncr;
  incr += spec_class->soulBurnIncr;
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
  incr += main_class->flowIncr;
  incr += second_class->flowIncr;
  incr += spec_class->flowIncr;
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

void Character::setRegion(Region * region) { this->region = region; }

void Character::setCurrentAction(Action * action) { this->current_action = action; }

void Character::applySoulBurn() {
  int soulBurnReduction = (int) std::max( (float) currentSoulBurn / 100.F, (float) soulBurnTreshold / 100.F);
  if(currentSoulBurn > soulBurnTreshold) {
    hp -= std::min(soulBurnReduction, currentSoulBurn - soulBurnTreshold);
  }
}
void Character::applyManaWaste() {
  channeledMana = (int) std::floor( (float) channeledMana * 0.99);
}

void Character::channel(int cost) {
  int toadd;
  // can't use last mana point to kill itself
  if(cost == -1) {
    toadd = std::min(mana - 1, getFlow());
  }
  else {
    toadd = std::min(std::min(getFlow(), cost - channeledMana), mana - 1);
  }
  currentSoulBurn += toadd;
  channeledMana += toadd;
  mana -= toadd;
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
  mana -= cost;
  currentSoulBurn += cost;
}
void Character::gainXP(long xp) { this->xp += xp; }
void Character::gainLevel() {
  while(xp >= level * level * 1000) { // INSERT FORMULA HERE
    level++;
    int old_max_mana = getMaxMana();
    int old_max_hp = getMaxHp();
    int old_max_shield = getMaxHp();
    incrMaxHp();
    hpHeal(getMaxHp() - old_max_hp);
    incrMaxMana();
    manaHeal(getMaxMana() - old_max_mana);
    incrMaxShield();
    shieldRestore(getMaxShield() - old_max_shield);
    incrDamageMultiplier();
    incrSoulBurnTreshold();
    incrFlow();
    newSkillsAndEffects();
    if(level == 10) {
     //selectSecondAttributes();
    }
  }
}

void Character::newSkillsAndEffects() {
  for(Skill * skill : main_class->getSkills()) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : main_class->getEffects()) {
    if(level == 5 * effect->level) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
  }
  for(Skill * skill : second_class->getSkills()) {
    if(level == 5 * skill->level) {
      addSkill(skill);
    }
  }
  for(Effect * effect : second_class->getEffects()) {
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

void Character::add(Item * item, int slot, int x, int y) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(slot == ITEM_SLOT_INSIDE_BAG) {
    gear->getBag()->add(item, x, y);
  }
  else if(slot == ITEM_SLOT_INSIDE_BELT) {
    gear->getBelt()->add(item, x, y);
  }
}

Item * Character::remove(int slot, int x, int y) {
  if(player_character) {
    setNeedToSend(true);
  }
  Item * item = nullptr;
  if(slot == ITEM_SLOT_INSIDE_BAG) {
    item = gear->getBag()->remove(x, y);
  }
  else if(slot == ITEM_SLOT_INSIDE_BELT) {
    item = gear->getBelt()->remove(x, y);
  }
  return item;
}

float Character::swap(int slot1, int slot2) {
  float time = getSwapTime(slot1, slot2);
  gear->swapWeapon(slot1, slot2);
  return time;
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

void Character::addItem(Item * i, int x, int y, int slot) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(slot == ITEM_SLOT_INSIDE_BAG) {
    gear->getBag()->add(i, x, y);
  }
  else if(slot == ITEM_SLOT_INSIDE_BELT) {
    gear->getBag()->add(i, x, y);
  }
}

void Character::removeItem(int x, int y, int slot) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(slot == ITEM_SLOT_INSIDE_BAG) {
    gear->getBag()->remove(x, y);
  }
  else if(slot == ITEM_SLOT_INSIDE_BELT) {
    gear->getBag()->remove(x, y);
  }
}

void Character::useItem(int x, int y, int slot) {
  gear->useItem(x, y, slot, this);
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

int Character::getDamageFromType(int damage_type, int slot) {
  int damage = 0;
  if(slot == ITEM_SLOT_WEAPON_1) {
    if(gear->getWeapon_1() != nullptr) {
      damage = gear->getWeapon_1()->getDamageFromType(damage_type);
    }
  }
  else if(slot == ITEM_SLOT_WEAPON_1) {
    if(gear->getWeapon_2() != nullptr) {
      damage = gear->getWeapon_2()->getDamageFromType(damage_type);
    }
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

Projectile * Character::shoot(Target * target, Adventure * adventure, int slot) {
  if(gear->getWeapon_1()->use_projectile && gear->getWeapon_1()->range >= std::max(abs(coord.x - target->coord.x), abs(coord.y - target->coord.y))) {
    if(!gear->getWeapon_1()->use_ammo || gear->getWeapon_1()->getCurrentCapacity() > 0) {
      int realDamages[DAMAGE_TYPE_NUMBER];
      for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
        realDamages[damage_type] = getDamageFromType(damage_type, slot);
      }
      Projectile * base_projectile = (Projectile *) gear->getWeapon_1()->getAmmo()->getProjectile();
      if(gear->getWeapon_1()->use_ammo) {
        if(player_character) {
          setNeedToSend(true);
        }
        gear->getWeapon_1()->useAmmo();
      }
      //return new Projectile(base_projectile, realDamages, adventure->getWorld(), current_map->id, target, this, 1, 1, 1, true);
    }
  }
  return nullptr;
}

void Character::attack(Character * target, Adventure * adventure, int type) {
  mainAttack(target, adventure, type);
  if(gear->getWeapon_2() != nullptr) {
    subAttack(target, adventure, type);
  }
}

void Character::mainAttack(Character * target, Adventure * adventure, int type) {
  if(gear->getWeapon_1()->range >= MapUtil::distance(coord, target->getCoord())) {
    if(gear->getWeapon_1()->use_projectile) {
      Target * t_target = new Target();
      t_target->type = TARGET_CHARACTER;
      t_target->id = target->id;
      shoot(t_target, adventure, ITEM_SLOT_WEAPON_1);
    }
    else {
      int realDamages[DAMAGE_TYPE_NUMBER];
      for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
        realDamages[damage_type] = getDamageFromType(damage_type, ITEM_SLOT_WEAPON_1);
      }
      target->receiveAttack(realDamages, orientation, type);
    }
  }
}

void Character::subAttack(Character * target, Adventure * adventure, int type) {
  if(gear->getWeapon_2()->range >= MapUtil::distance(coord, target->getCoord())) {
    if(gear->getWeapon_2()->use_projectile) {
      Target * t_target = new Target();
      t_target->type = TARGET_CHARACTER;
      t_target->id = target->id;
      shoot(t_target, adventure, ITEM_SLOT_WEAPON_2);
    }
    else {
      int realDamages[DAMAGE_TYPE_NUMBER];
      for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
        realDamages[damage_type] = getDamageFromType(damage_type, ITEM_SLOT_WEAPON_2);
      }
      target->receiveAttack(realDamages, orientation, type);
    }
  }
}

void Character::reload(ItemSlot * ammo, int slot_weapon) {
  if(player_character) {
    setNeedToSend(true);
  }
  gear->reload(ammo, slot_weapon);
}


ItemSlot * Character::canReload(int slot) {
  ItemSlot * ammo = nullptr;
  int number = 0;
  if(slot == ITEM_SLOT_WEAPON_1) {
    for(ItemSlot * current : gear->getBelt()->getItems()) {
      if(current->item->type == ITEM_AMMUNITION && gear->getWeapon_1()->ammo_type == current->item->type2 && ( (AmmunitionItem *) current->item)->getNumber() > number) {
        ammo = current;
        number = ((AmmunitionItem *) current->item)->getNumber();
      }
    }
  }
  else if(slot == ITEM_SLOT_WEAPON_2) {
    for(ItemSlot * current : gear->getBelt()->getItems()) {
      if(current->item->type == ITEM_AMMUNITION && gear->getWeapon_2()->ammo_type == current->item->type2 && ( (AmmunitionItem *) current->item)->getNumber() > number) {
        ammo = current;
        number = ((AmmunitionItem *) current->item)->getNumber();
      }
    }
  }
  return ammo;
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
    damage = std::max(0, damage);
    int leftover = damage - shield;
    shield -= damage;
    hp -= std::max(0, leftover);
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
    damage = std::max(0, damage);
    int leftover = damage - shield;
    shield -= damage;
    hp -= std::max(0, leftover);
  }
}

int Character::tryAttack(std::array<int, DAMAGE_TYPE_NUMBER> damages, int type) {
  if(isInvulnerable() || isEtheral()) {
    return 0;
  }
  int damage = 0;
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    if(damage_type == DAMAGE_ACID) {
      damage += damages[damage_type];
    }
    if(damage_type == DAMAGE_MIND) {
      damage += std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
    }
    if(damage_type == DAMAGE_TRUE) {
      damage += damages[damage_type];
    }
    if(damage_type == DAMAGE_SOUL) {
      damage += damages[damage_type];
    }
    else {
      damage += std::max(0, (int) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
    }
  }
  return damage;
}

void Character::trade(Character * buyer, int object_type, std::string object_name, float price_modifier) {
  int price = 0;
  switch (object_type) {
    case OBJECT_ITEM:
      for(Item * item : sellable_items) {
        if(item->name == object_name) {
          price = (int) std::ceil((float) item->gold_value * price_modifier);
          if(buyer->getGold() >= price) {
            //buyer->addItem(item);
            buyer->loseGold(price);
            sellable_items.remove(item);
            gainGold(price);
          }
        }
      }
      break;
    case OBJECT_SKILL:
      for(Skill * skill : sellable_skills) {
        if(skill->name == object_name && buyer->level >= 5 * skill->level && (skill->attributes == "" || buyer->main_class->name == skill->attributes)) {
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
        if(effect->name == object_name && buyer->level >= 5 * effect->level && (effect->attributes == "" || buyer->main_class->name == effect->attributes)) {
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

std::set<std::string> Character::getTags() {
  std::set<std::string> result = std::set<std::string>();
  for(std::string tag : main_class->getTags()) {
    result.insert(tag);
  }
  for(std::string tag : second_class->getTags()) {
    result.insert(tag);
  }
  for(std::string tag : spec_class->getTags()) {
    result.insert(tag);
  }
  for(std::string tag : race->getTags()) {
    result.insert(tag);
  }
  for(Race * modifier : race_modifiers) {
    for(std::string tag : modifier->getTags()) {
      result.insert(tag);
    }
  }
  for(std::string tag : origin->getTags()) {
    result.insert(tag);
  }
  for(std::string tag : culture->getTags()) {
    result.insert(tag);
  }
  for(std::string tag : religion->getTags()) {
    result.insert(tag);
  }
  for(std::string tag : profession->getTags()) {
    result.insert(tag);
  }
  for(Way * title : titles) {
    for(std::string tag : title->getTags()) {
      result.insert(tag);
    }
  }
  return result;
}

std::string Character::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, hp);
  String::insert_int(ss, getMaxHp());
  String::insert_int(ss, mana);
  String::insert_int(ss, getMaxMana());
  String::insert_int(ss, shield);
  String::insert_int(ss, getMaxShield());
  String::insert_float(ss, getStamina());
  String::insert_float(ss, getSatiety());
  String::insert_int(ss, getCurrentSoulBurn());
  String::insert_int(ss, getSoulBurnThreshold());
  String::insert_int(ss, getFlow());
  String::insert_bool(ss, player_character);
  String::insert_int(ss, race->getType(race_modifiers));
  String::insert_float(ss, coord.x);
  String::insert_float(ss, coord.y);
  String::insert_float(ss, coord.z);
  String::insert_float(ss, getSize());
  String::insert_float(ss, getHeight());
  String::insert_float(ss, orientation);
  String::insert(ss, team);
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
    String::insert_int(ss, getDamageFromType(i, ITEM_SLOT_WEAPON_1));
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
  display->shield = String::extract_int(ss);
  display->maxShield = String::extract_int(ss);
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
  display->height = String::extract_float(ss);
  display->orientation = String::extract_float(ss);
  display->team = String::extract(ss);
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
  String::insert_int(ss, maxShield);
  String::insert_int(ss, hp);
  String::insert_int(ss, mana);
  String::insert_int(ss, shield);
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
  String::insert_float(ss, coord.x);
  String::insert_float(ss, coord.y);
  String::insert_float(ss, coord.z);
  String::insert_float(ss, size);
  String::insert_float(ss, height);
  String::insert_float(ss, orientation);
  String::insert_bool(ss, merchant);
  String::insert_long(ss, gold);
  String::insert_long(ss, xp);
  String::insert_int(ss, level);
  String::insert(ss, team);
  String::insert(ss, gear->to_string());
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

  String::insert(ss, main_class->name);
  if(second_class != nullptr) {
    String::insert(ss, ((Attributes *) second_class)->name);
  }
  else {
    String::insert(ss, "none");
  }
  if(spec_class != nullptr) {
    String::insert(ss, ((Attributes *) spec_class)->name);
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
  int maxShield = String::extract_int(ss);
  int hp = String::extract_int(ss);
  int mana = String::extract_int(ss);
  int shield = String::extract_int(ss);
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
  MapUtil::Vector3 coord = MapUtil::Vector3();
  coord.x = String::extract_float(ss);
  coord.y = String::extract_float(ss);
  coord.z = String::extract_float(ss);
  float size = String::extract_float(ss);
  float height = String::extract_float(ss);
  float orientation = String::extract_float(ss);
  bool merchant = String::extract_bool(ss);
  long gold = String::extract_long(ss);
  long xp = String::extract_long(ss);
  int level = String::extract_int(ss);
  std::string team = String::extract(ss);
  Gear * gear = Gear::from_string(String::extract(ss), adventure);
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
  Attributes * main_class = (Attributes *) adventure->getDatabase()->getAttributes(String::extract(ss));
  std::string second_attributes_str = String::extract(ss);
  Attributes * second_class = nullptr;
  if(second_attributes_str != "none") {
    second_class = (Attributes *) adventure->getDatabase()->getAttributes(second_attributes_str);
  }
  std::string spec_attributes_str = String::extract(ss);
  Attributes * spec_class = nullptr;
  if(spec_attributes_str != "none") {
    spec_class = (Attributes *) adventure->getDatabase()->getAttributes(spec_attributes_str);
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
    maxShield,
    hp,
    mana,
    shield,
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
    coord,
    size,
    height,
    orientation,
    nullptr,
    merchant,
    gold,
    xp,
    level,
    team,
    gear,
    *effects,
    *skills,
    *sellable_items,
    *sellable_effects,
    *sellable_skills,
    main_class,
    second_class,
    spec_class,
    race,
    *race_modifiers,
    origin,
    culture,
    religion,
    profession,
    *titles
  );
  delete effects;
  delete skills;
  delete sellable_items;
  delete sellable_effects;
  delete sellable_skills;
  delete race_modifiers;
  delete titles;
  return result;
}
