#include "ai/AI.h"
#include "data/Database.h"
#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/Settings.h"
#include "data/Speech.h"
#include "data/Stance.h"
#include "data/ways/Way.h"
#include "data/ways/Race.h"
#include "data/Region.h"
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

/*
Character::~Character() {
  delete ai;
  delete hitbox;
  for(Effect * effect : getEffects()) {
    delete effect;
  }
  skills.clear();
  titles.clear();
}
*/

void Character::initializeCharacter(Gear * gear) {
  size = race->getSize(race_modifiers);
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
  hunger = 75.;
  thirst = 75.;
  stamina = 75.;
  sanity = 50.;
  channeledMana = 0.F;
  this->gear = new Gear(gear);
  for(int i = 0; i < DAMAGE_TYPE_STATUS_NUMBER; i++) {
    status[i] = 0.F;
  }
  initSkillsAndEffects();
  hitbox = new MathUtil::HitboxOBB(HITBOX_OBB, coord, getSize().x, getSize().y, getSize().z);
  hitbox->applyMove(coord, MathUtil::getDirectionFromOrientation(orientation));
  stance_attack = 0;
  // TODO real stance manager
  int32_t weapon_combinaison = gear->getWeaponsCombination();
  for(Stance * stance : getAvaillableStances()) {
    if(stance->isValid(weapon_combinaison)) {
      if(stance->magical) {
        active_magical_stances.insert(std::make_pair(weapon_combinaison, stance));
      }
      else {
        active_stances.insert(std::make_pair(weapon_combinaison, stance));
      }
    }
  }
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
  for(Effect * effect : getEffects()) {
    Effect * toadd = new Effect(effect, 1, 1);
    toadd->activate(this);
  }
}

bool Character::isAlive() { return hp > 0 && mana > 0; }
bool Character::isMarkedDead() { return dead; }
void Character::markDead(bool dead) { this->dead = dead; }
bool Character::isSoulBurning() { return currentSoulBurn >= soulBurnTreshold; }
MathUtil::Vector3 Character::getCoord() { return coord; }
MathUtil::Vector3 Character::getSpeed() { return speed; }
MathUtil::Coords Character::getWorldCoords() { return MathUtil::getCoords(coord); }
MathUtil::HitboxOBB * Character::getHitbox() { return hitbox; }

void Character::produceShield(int32_t shield_type, float shield_hp, MathUtil::Vector3 size) {
  outer_shield = new Shield(shield_type, shield_hp, size, this);
  region->addShield(outer_shield);
}

void Character::stopShield() {
  region->removeShield(outer_shield);
  delete outer_shield;
  outer_shield = nullptr;
}

MathUtil::Vector3 Character::getSize() { return size; }
MathUtil::Vector3 Character::getOrientation() { return orientation; }
float Character::getHp() { return hp; }
int32_t Character::getMaxHp() {
  int32_t bonus = 0;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_HP_MAX) {
      bonus += e->power;
    }
  }
  return std::max(maxHp + bonus, 0);
}

float Character::getMana() { return mana; }
float Character::getChanneledMana() { return channeledMana; }

int32_t Character::getMaxMana() {
  int32_t bonus = 0;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_MANA_MAX) {
      bonus += e->power;
    }
  }
  return std::max(maxMana + bonus, 0);
}

float Character::getShield() { return shield; }

int32_t Character::getMaxShield() {
  int32_t bonus = 0;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_SHIELD_MAX) {
      bonus += e->power;
    }
  }
  return std::max(maxShield + bonus, 0);
}

float Character::getHunger() { return hunger; }
float Character::getThirst() { return thirst; }
float Character::getStamina() { return stamina; }
float Character::getSanity() { 
  float result = sanity;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_STATUS_BROKEN) {
      result -= 25;
    }
  }
  return result;
}

int32_t Character::getSoulBurnThreshold() {
  int32_t bonus = 0;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_SOULBURNTRESHOLD) {
      bonus += e->power;
    }
  }
  return std::max(soulBurnTreshold + bonus, 0);
}

int32_t Character::getCurrentSoulBurn() { return currentSoulBurn; }

int32_t Character::getFlow() {
  int32_t bonus = 0;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_FLOW) {
      bonus += e->power;
    }
  }
  int32_t result = std::max(flow + bonus, 0);
  int32_t decrement = result / 10;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_STATUS_WEAKENED) {
      result -= decrement;
    }
  }
  return std::max(result, 0);
}

int64_t Character::getRawPower() {
  return getMaxHp() + getMaxMana() + getMaxShield() + getSoulBurnThreshold() + 5 * (getFlow() + (getDamageMultiplier() - 1.F) * 100.F);
}

int32_t Character::getVisionRange() {
  int32_t bonus = 0;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_VISION_RANGE) {
      bonus += e->power;
    }
  }
  return std::max(visionRange + bonus, 0);
}

int32_t Character::getVisionPower() {
  int32_t bonus = 0;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_VISION_POWER) {
      bonus += e->power;
    }
  }
  return std::max(visionPower + bonus, 0);
}

int32_t Character::getDetectionRange() {
  int32_t bonus = 0;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_DETECTION_RANGE) {
      bonus += e->power;
    }
  }
  return std::max(detectionRange + bonus, 0);
}

int64_t Character::getGold() { return gold; }
int64_t Character::getXP() { return xp + last_level_xp; }
int32_t Character::getLevel() { return level; }

float Character::getDamageMultiplier() {
  int32_t result = damage_multiplier;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_DAMAGE_MULTIPLIER) {
      result += e->power;
    }
  }
  result = std::max(1.F + ((float) result) / 100.F, 0.F);
  int32_t decrement = result / 10;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_STATUS_WEAKENED) {
      result -= decrement;
    }
  }
  return std::max(result, 0);
}

int32_t Character::getPowerScore() {
  return getMaxHp() + getMaxMana() + 5 * (getFlow() + getSoulBurnThreshold() + getShield() + getDamageMultiplier());
}

bool Character::needToSend() { return need_to_send; }
void Character::setNeedToSend(bool need_to_send) { this->need_to_send = need_to_send; }

AI * Character::getAI() { return ai; }
std::string Character::getTeam() { return team; }
Speech * Character::getDeathSpeech() { return death_speech; }
Speech * Character::getTalkingSpeech() { return talking_speech; }
int32_t Character::getType() { return race->getType(race_modifiers); }

Region * Character::getRegion() { return region; }
Action * Character::getAction() { return action; }
Action * Character::getLegAction() { return leg_action; }

Gear * Character::getGear() { return gear; }

float Character::getActionTimeModifier() {
  float result = race->getActionTimeModifier(race_modifiers);
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_ACTION_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result;
}

float Character::getHandActionTimeModifier() {
  float result = race->getStrikeTimeModifier(race_modifiers);
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_HAND_ACTION_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result * getActionTimeModifier();
}

float Character::getSkillTimeModifier() {
  float result = race->getSkillTimeModifier(race_modifiers);
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_SKILL_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return result * getActionTimeModifier();
}

float Character::getMovementTimeModifier() {
  float result = race->getMovementTimeModifier(race_modifiers);
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_MOVEMENT_TIME_MODIFIER) {
      result += (float) e->power / 100.F;
    }
  }
  return running ? result * getActionTimeModifier() * 2.F : result * getActionTimeModifier();
}

float Character::getStrikeTime(int32_t slot) {
  if(slot == ITEM_SLOT_WEAPON_1) {
    return (float) gear->getWeapon_1()->strike_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_2) {
    return (float) gear->getWeapon_2()->strike_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_3) {
    return (float) gear->getBackupWeapon()->strike_time / getHandActionTimeModifier();
  }
  return 0.F;
}

float Character::getReloadTime(int32_t slot) {
  if(slot == ITEM_SLOT_WEAPON_1) {
    return (float) gear->getWeapon_1()->reload_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_2) {
    return (float) gear->getWeapon_2()->reload_time / getHandActionTimeModifier();
  }
  else if(slot == ITEM_SLOT_WEAPON_3) {
    return (float) gear->getBackupWeapon()->reload_time / getHandActionTimeModifier();
  }
  return 0.F;
}

float Character::getSwapTime(int32_t slot1, int32_t slot2) {
  if(slot1 == ITEM_SLOT_WEAPON_1) {
    if(slot2 == ITEM_SLOT_WEAPON_3) {
      return ((float) gear->getWeapon_1()->swap_time + gear->getBackupWeapon()->swap_time) / getHandActionTimeModifier();
    }
  }
  else if(slot1 == ITEM_SLOT_WEAPON_2) {
    if(slot2 == ITEM_SLOT_WEAPON_3) {
      return ((float) gear->getWeapon_2()->swap_time + gear->getBackupWeapon()->swap_time) / getHandActionTimeModifier();
    }
  }
  return 0.F;
}

float Character::getUseTime(Item * item) {
  return (float) item->use_time / getHandActionTimeModifier();
}

int32_t Character::getLight() {
  int32_t light = 0;
  for(Effect * effect : getEffects()) {
    if(effect->type == EFFECT_LIGHT) {
      light += effect->power;
    }
  }
  return light;
}

std::list<Item *> Character::getLoot() { return race->getLoot(race_modifiers); }
std::list<Effect *> Character::getEffects() {
  std::list<Effect *> result = effects;
  for(Effect * effect : gear->getEffects()) {
    result.push_back(effect);
  }
  return result;
}
std::list<Skill *> Character::getSkills() { return skills; }

bool Character::getToggled(Skill * s) {
  std::map<const Skill *, bool>::iterator it = toggled_skills.find(s);
  if(it != toggled_skills.end()) {
    return it->second;
  }
  return false;
}

std::map<Skill *, std::array<float, DAMAGE_TYPE_NUMBER>> Character::getDamageSkills() {
  std::map<Skill *, std::array<float, DAMAGE_TYPE_NUMBER>> result = std::map<Skill *, std::array<float, DAMAGE_TYPE_NUMBER>>();
  for(Skill * skill : skills) {
    if(skill->getManaCost() < mana) {
      std::array<float, DAMAGE_TYPE_NUMBER> damages;
      bool isDamageSkill = false;
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages[i] = skill->getDamageFromType(i, this, 1);
        if(damages[i] > 0) {
          isDamageSkill = true;
        }
      }
      if(isDamageSkill) {
        result.insert(std::make_pair(skill, damages));
      }
    }
  }
  std::array<float, DAMAGE_TYPE_NUMBER> damages;
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
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

std::list<Way *> Character::getWays() {
  std::list<Way *> ways = std::list<Way *>();
  ways.push_back(main_class);
  if(second_class != nullptr) {
    ways.push_back(second_class);
  }
  if(spec_class != nullptr) {
    ways.push_back(spec_class);
  }
  ways.push_back(race);
  ways.push_back(origin);
  ways.push_back(culture);
  ways.push_back(religion);
  ways.push_back(profession);
  for(Way * way : titles) {
    ways.push_back(way);
  }
  return ways;
}

Stance * Character::getCurrentStance() {
  if(using_magical_stance) {
    return active_magical_stances.at(gear->getWeaponsCombination());
  }
  else {
    return active_stances.at(gear->getWeaponsCombination());
  }
}

Stance * Character::getStance(int32_t weapon_type) {
  std::map<const int32_t, Stance *>::iterator it = active_stances.find(weapon_type);
  if(it != active_stances.end()) {
    return it->second;
  }
  else {
    return nullptr;
  }
}

Stance * Character::getMagicalStance(int32_t weapon_type) {
  std::map<const int32_t, Stance *>::iterator it = active_magical_stances.find(weapon_type);
  if(it != active_magical_stances.end()) {
    return it->second;
  }
  else {
    return nullptr;
  }
}

std::list<Stance *> Character::getAvaillableStances() {
  std::list<Stance *> stances = std::list<Stance *>();
  for(Way * way : getWays()) {
    for(Stance * stance : way->getStances()) {
      stances.push_back(stance);
    }
  }
  return stances;
}

std::map<int32_t, Stance *> Character::getActiveStances() {
  return active_stances;
}

std::map<int32_t, Stance *> Character::getActiveMagicalStances() {
  return active_magical_stances;
}

Skill * Character::getAttack() {
  Skill * result = getCurrentStance()->getAttack(stance_attack);
  return result;
}

void Character::incrAttack() {
  stance_attack = (stance_attack + 1) % (getCurrentStance()->attack_skills_number);
}

Skill * Character::getDefense() {
  return getCurrentStance()->getBlock();
}

void Character::setOrientationX(float orientation_x) { orientation.x = orientation_x; }
void Character::setOrientationZ(float orientation_z) { orientation.z = orientation_z; }
void Character::move(MathUtil::Vector3 coord, MathUtil::Vector3 orientation, World * world) {
  MathUtil::Vector3 ori = this->coord;
  this->coord = coord;
  this->orientation.x = orientation.x;
  this->orientation.z = orientation.z;
  hitbox->applyMove(coord, MathUtil::getDirectionFromOrientation(orientation));
  world->checkRegion(this, ori, coord);
}

void Character::run() {
  running = !running;
}

void Character::jump() {
  if(region->getBlock(MathUtil::Vector3i(coord.x, coord.y, coord.z - 0.01F)) != nullptr) {
    // running doesn't make jumping higher
    // TODO: verify we cannot time a jump to avoid fall damage
    speed.z = (running ? 2.F * getMovementTimeModifier() : 4.F * getMovementTimeModifier());
  }
}

void Character::setSpeed(MathUtil::Vector3 speed) { this->speed = speed; }

void Character::hpHeal(float hp) { this->hp = std::min(this->hp + hp, (float) getMaxHp()); }
void Character::incrMaxHp() {
  if(player_character) {
    setNeedToSend(true);
  }
  int32_t incr = 0;
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
void Character::setHp(float hp) { this->hp = hp; }

void Character::manaHeal(float mana) { this->mana = std::min(this->mana + mana, (float) getMaxMana()); }

void Character::incrMaxMana() {
  if(player_character) {
    setNeedToSend(true);
  }
  int32_t incr = 0;
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

void Character::setMana(float mana) { this->mana = mana; }
float Character::shieldRestore(float shield) {
  this->shield = std::min(this->shield + shield, (float) getMaxShield());
  if(this->shield < 0.F) {
    float result = this->shield;
    this->shield = 0.F;
    return result;
  }
  return 0.F;
}
void Character::setShield(float shield) { this->shield = shield; }
void Character::incrMaxShield() {
  if(player_character) {
    setNeedToSend(true);
  }
  int32_t incr = 0;
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
void Character::addHunger(float hunger) { this->hunger = std::max(-100.F, std::min(100.F, this->hunger + hunger)); }
void Character::addThirst(float thirst) { this->thirst = std::max(-100.F, std::min(100.F, this->thirst + thirst)); }
void Character::addStamina(float stamina) { this->stamina = std::max(-100.F, std::min(100.F, this->stamina + stamina)); }
void Character::addSanity(float sanity) { this->sanity = std::max(-100.F, std::min(100.F, this->sanity + sanity)); }

void Character::setHunger(float hunger) { this->hunger = hunger; }
void Character::setThirst(float thirst) { this->thirst = thirst; }
void Character::setStamina(float stamina) { this->stamina = stamina; }
void Character::setSanity(float sanity) { this->sanity = sanity; }

void Character::incrDamageMultiplier() {
  if(player_character) {
    setNeedToSend(true);
  }
  int32_t incr = 0;
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
  int32_t incr = 0;
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
void Character::setCurrentSoulBurn(int32_t soulBurn) { this->currentSoulBurn = soulBurn; }
void Character::incrFlow() {
  if(player_character) {
    setNeedToSend(true);
  }
  int32_t incr = 0;
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

void Character::setAction(Action * action) { this->action = action; }
void Character::setLegAction(Action * action) { leg_action = action; }

void Character::applySoulBurn() {
  float soulBurnReduction = std::max( (float) currentSoulBurn / 100.F, (float) soulBurnTreshold / 100.F);
  if(currentSoulBurn > soulBurnTreshold) {
    hp -= std::min(soulBurnReduction, currentSoulBurn - soulBurnTreshold);
  }
}

void Character::applyManaWaste() {
  channeledMana *= 0.99;
}

void Character::channel(int32_t cost) {
  float toadd;
  // can't use last mana point to kill itself
  if(cost == -1) {
    toadd = std::min((int32_t) std::floor(mana) - 1, getFlow());
  }
  else {
    toadd = std::min(std::min(getFlow(), cost - (int32_t) std::floor(channeledMana)), (int32_t) std::floor(mana) - 1);
  }
  currentSoulBurn += toadd;
  channeledMana += toadd;
  mana -= toadd;
}

void Character::hungerStep(Environment e) {
  // 2 Days without eating
  if(!(isSleeping() || isIdling())) {
    addHunger(-100.F / (Settings::getMinuteDuration() * Settings::getDayDurationInRound() * 2.F));
  }
  else {
    addHunger(-100.F / (Settings::getMinuteDuration() * Settings::getDayDurationInRound() * 10.F));
  }
}

void Character::thirstStep(Environment e) {
  if(!(isSleeping() || isIdling())) {
    addThirst(-100.F / (Settings::getMinuteDuration() * Settings::getDayDurationInRound()));
  }
  else {
    addThirst(-100.F / (Settings::getMinuteDuration() * Settings::getDayDurationInRound() * 5.F));
  }
}

void Character::staminaStep(Environment e) {
  // 2 Days without eating
  if(!(isSleeping() || isIdling())) {
    addStamina(-100.F / (Settings::getMinuteDuration() * Settings::getDayDurationInRound() * 2.F));
  }
}

void Character::sanityStep(Environment e) {
  sanity = 50.F;
}

void Character::applyBodyNeeds() {
  if(race->getNeedToEat(race_modifiers)) {
    float body = std::min(hunger, thirst);
    if(body > 0.5) {
      hpHeal(getMaxHp() / (Settings::getMinuteDuration() * Settings::getDayDurationInRound()));
    }
    else if(body > 0.F) {
      hpHeal(getMaxHp() * body * 2.F / (Settings::getMinuteDuration() * Settings::getDayDurationInRound() * 100.F));
    }
    else {
      hpHeal(getMaxHp() * body / (Settings::getMinuteDuration() * 100.F));
    }
  }
}

void Character::applySoulNeeds() {
  if(race->getNeedToSleep(race_modifiers)) {
    if(stamina > 0.5) {
      shieldRestore(getMaxShield() / Settings::getMinuteDuration());
    }
    else if(stamina > 0.F) {
      shieldRestore(getMaxShield() * stamina * 2.F / (Settings::getMinuteDuration() * 100.F));
    }
    else {
      float rest = shieldRestore(getMaxShield() * stamina / (Settings::getMinuteDuration() * 100.F));
      payMana(rest / 2.F);
      hpHeal(-rest / 2.F);
    }
  }
}

void Character::applySpiritNeeds() {
  if(race->getNeedToThink(race_modifiers)) {
    manaHeal(getMaxMana() * sanity / (Settings::getMinuteDuration() * 100.F));
  }
}

void Character::applyEffects() {
  for(Effect * e : getEffects()) {
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
    stamina += 3.F * 100.F / (Settings::getMinuteDuration() * Settings::getDayDurationInRound() * 2.F);
  }
}

void Character::gainGold(int64_t gold) { this->gold += gold; }
void Character::loseGold(int64_t gold) { this->gold = (int64_t) std::max(0., (double) this->gold + gold); }
void Character::payMana(float cost) {
  mana -= cost;
  currentSoulBurn += cost;
}
void Character::gainXP(int64_t xp) { this->xp += xp; }
// u(0) = 0
// u(n+1) = n^3 * 60 + u(n)
void Character::gainLevel() {
  int64_t current_level_xp;
  while( (current_level_xp = level * level * level * 60) <= xp ) {
    last_level_xp += current_level_xp;
    xp -= current_level_xp;
    level++;
    int32_t old_max_mana = getMaxMana();
    int32_t old_max_hp = getMaxHp();
    int32_t old_max_shield = getMaxHp();
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

void Character::add(Item * item, int32_t slot, int32_t x, int32_t y) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(slot == ITEM_SLOT_INSIDE_BAG) {
    gear->getBag()->add(item, x, y);
  }
  else if(slot == ITEM_SLOT_INSIDE_BASE_INVENTORY) {
    gear->getBaseInventory()->add(item, x, y);
  }
}

Item * Character::remove(int32_t slot, int32_t x, int32_t y) {
  if(player_character) {
    setNeedToSend(true);
  }
  Item * item = nullptr;
  if(slot == ITEM_SLOT_INSIDE_BAG) {
    item = gear->getBag()->remove(x, y);
  }
  else if(slot == ITEM_SLOT_INSIDE_BASE_INVENTORY) {
    item = gear->getBaseInventory()->remove(x, y);
  }
  return item;
}

float Character::swap(int32_t slot1, int32_t slot2) {
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

void Character::removeSimilarEffect(Effect * e) {
  if(player_character) {
    setNeedToSend(true);
  }
  Effect * to_remove = nullptr;
  for(Effect * effect : effects) {
    if(e->isSimilar(effect)) {
      to_remove = effect;
      break;
    }
  }
  if(to_remove != nullptr) {
    effects.remove(to_remove);
  }
}

void Character::removeSkill(Skill * s) {
  if(player_character) {
    setNeedToSend(true);
  }
  skills.remove(s);
}

bool Character::setToggled(Skill * s) {
  bool result = true;
  std::map<const Skill *, bool>::iterator it = toggled_skills.find(s);
  if(it != toggled_skills.end()) {
    result = !it->second;
    toggled_skills.erase(it);
  }
  toggled_skills.insert(std::make_pair(s, result));
  return result;
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

void Character::addItem(Item * i, int32_t x, int32_t y, int32_t slot) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(slot == ITEM_SLOT_INSIDE_BAG) {
    gear->getBag()->add(i, x, y);
  }
  else if(slot == ITEM_SLOT_INSIDE_BASE_INVENTORY) {
    gear->getBag()->add(i, x, y);
  }
}

void Character::removeItem(int32_t x, int32_t y, int32_t slot) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(slot == ITEM_SLOT_INSIDE_BAG) {
    gear->getBag()->remove(x, y);
  }
  else if(slot == ITEM_SLOT_INSIDE_BASE_INVENTORY) {
    gear->getBag()->remove(x, y);
  }
}

void Character::useItem(int32_t x, int32_t y, int32_t slot) {
  gear->useItem(x, y, slot, this);
}

bool Character::isFlying() {
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_FLY) {
      return true;
    }
  }
  return false;
}

bool Character::isChanneling() {
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_CHANNELING) {
      return true;
    }
  }
  return false;
}

bool Character::isStunned() {
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_STUNNED) {
      return true;
    }
    if(e->type == EFFECT_STATUS_SHOCKED) {
      return true;
    }
  }
  return false;
}

bool Character::isCloaked() {
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_CLOAKED) {
      return true;
    }
  }
  return false;
}

bool Character::isInvisible() {
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_INVISIBLE) {
      return true;
    }
  }
  return false;
}

bool Character::isEtheral() {
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_ETHERAL) {
      return true;
    }
  }
  return false;
}

bool Character::isInvulnerable() {
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_INVULNERABLE) {
      return true;
    }
  }
  return false;
}


bool Character::isSleeping() {
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_SLEEPING) {
      return true;
    }
  }
  return false;
}

bool Character::isIdling() {
  return (action == nullptr || action->type == ACTION_IDLE) && leg_action == nullptr;
}

int32_t Character::cloakPower() {
  int32_t max = 0;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_CLOAKED) {
      if(e->power > max) {
        max = e->power;
      }
    }
  }
  return max;
}

bool Character::isInWeakState() {
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_STUNNED || e->type == EFFECT_SLEEPING || e->type == EFFECT_STATUS_SHOCKED) {
      return true;
    }
  }
  return false;
}

void Character::useSkill(Skill * skill, MathUtil::Target * target, Adventure * adventure, float overcharge) {
  skill->activate(this, target, adventure, overcharge);
}

bool Character::selectStance(Stance * stance) {
  for(Stance * to_check : getAvaillableStances()) {
    if(stance == to_check) {
      int32_t weapon_type = gear->getWeaponsCombination();
      if(stance->isValid(weapon_type)) {
        if(stance->magical) {
          std::map<const int32_t, Stance *>::iterator it = active_magical_stances.find(weapon_type);
          if(it != active_magical_stances.end()) {
            active_magical_stances.erase(it);
          }
          active_magical_stances.insert(std::make_pair(weapon_type, stance));
        }
        else {
          std::map<const int32_t, Stance *>::iterator it = active_stances.find(weapon_type);
          if(it != active_stances.end()) {
            active_stances.erase(it);
          }
          active_stances.insert(std::make_pair(weapon_type, stance));
        }
        return true;
      }
      else {
        return false;
      }
    }
  }
  return false;
}

int32_t Character::getDamageFromType(int32_t damage_type, int32_t slot) {
  int32_t damage = 0;
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
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type);
    }
  }
  return (int32_t) std::ceil((float) damage * getDamageMultiplier());
}

float Character::getRawDamageReductionFromType(int32_t damage_type) {
  float reduction = gear->getDamageReductionFromType(damage_type);
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_DAMAGE_REDUCTION) {
      reduction += e->getDamageReductionFromType(damage_type);
    }
  }
  int32_t decrement = std::max(0.F, reduction / 10);
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_STATUS_CORRODED) {
      reduction -= decrement;
    }
  }
  return reduction;
}

float Character::getDamageReductionFromType(int32_t damage_type) {
  float damage_reduction = getRawDamageReductionFromType(damage_type);
  if(damage_reduction > 0.F) {
    return 1.F - 100.F / (100.F + damage_reduction);
  }
  else {
    return damage_reduction / 100.F;
  }
}

float Character::getStatusPower() {
  float status_power = 1.F;
  for(Effect * effect : getEffects()) {
    if(effect->type == EFFECT_STATUS_POWER) {
      status_power += effect->power;
    }
  }
  return status_power * gear->getWeapon_1()->status_power;
}

float Character::getStatusReductionFromType(int32_t damage_type) {
  float status_reduction = 0.F;
  for(Effect * e : getEffects()) {
    if(e->type == EFFECT_STATUS_REDUCTION) {
      // no need to use another variable
      status_reduction += e->getDamageReductionFromType(damage_type);
    }
    // 100% status reduction
    if(e->type == EFFECT_STATUS_IMMUNITY_BLEEDING + damage_type) {
      return 1.F;
    }
  }
  if(status_reduction > 0.F) {
    return 1.F - 100.F / (100.F + status_reduction);
  }
  else {
    return status_reduction / 100.F;
  }
}

Projectile * Character::shoot(MathUtil::Target * target, Adventure * adventure, int32_t slot) {
  if(gear->getWeapon_1()->use_projectile && gear->getWeapon_1()->range.y >= std::max(abs(coord.x - target->coord.x), abs(coord.y - target->coord.y))) {
    if(!gear->getWeapon_1()->use_ammo || gear->getWeapon_1()->getCurrentCapacity() > 0) {
      std::array<float, DAMAGE_TYPE_NUMBER> realDamages;
      for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
        realDamages[damage_type] = getDamageFromType(damage_type, slot);
      }
      //Projectile * base_projectile = (Projectile *) gear->getWeapon_1()->getAmmo()->getProjectile();
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

/*
void Character::mainAttack(Character * target, Adventure * adventure, int32_t type) {
  if(gear->getWeapon_1()->range >= MathUtil::distance(coord, target->getCoord())) {
    if(gear->getWeapon_1()->use_projectile) {
      MathUtil::Target * t_target = new MathUtil::Target();
      t_target->type = TARGET_CHARACTER;
      t_target->id = target->id;
      shoot(t_target, adventure, ITEM_SLOT_WEAPON_1);
    }
    else {
      std::array<float, DAMAGE_TYPE_NUMBER> realDamages;
      for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
        realDamages[damage_type] = getDamageFromType(damage_type, ITEM_SLOT_WEAPON_1);
      }
      target->receiveDamages(realDamages, this, this->getStatusPower());
    }
  }
}
*/

void Character::reload(ItemSlot * ammo, int32_t slot_weapon) {
  if(player_character) {
    setNeedToSend(true);
  }
  gear->reload(ammo, slot_weapon);
}


ItemSlot * Character::canReload(int32_t slot) {
  ItemSlot * ammo = nullptr;
  int32_t number = 0;
  if(slot == ITEM_SLOT_WEAPON_1) {
    for(ItemSlot * current : gear->getBaseInventory()->getItems()) {
      if(current->item->type == ITEM_AMMUNITION && gear->getWeapon_1()->ammo_type == current->item->subtype && ( (AmmunitionItem *) current->item)->getNumber() > number) {
        ammo = current;
        number = ((AmmunitionItem *) current->item)->getNumber();
      }
    }
  }
  else if(slot == ITEM_SLOT_WEAPON_2) {
    for(ItemSlot * current : gear->getBaseInventory()->getItems()) {
      if(current->item->type == ITEM_AMMUNITION && gear->getWeapon_2()->ammo_type == current->item->subtype && ( (AmmunitionItem *) current->item)->getNumber() > number) {
        ammo = current;
        number = ((AmmunitionItem *) current->item)->getNumber();
      }
    }
  }
  return ammo;
}

void Character::receiveDamages(std::array<float, DAMAGE_TYPE_NUMBER> damages, Character * attacker, float status_power) {
  if(!isInvulnerable() && !isEtheral()) {
    int32_t damage = 0;
    int32_t current_damage = 0;
    // skip DAMAGE_PHYSICAL
    for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER - 1; damage_type++) {
      if(damages[damage_type] > 0) {
        if(damage_type == DAMAGE_ACID) {
          current_damage = damages[damage_type];
          damage += current_damage;
          status[damage_type] += current_damage * status_power * 100.F * (1.F - getStatusReductionFromType(damage_type)) / (Settings::getStatusThreshold(damage_type) * (1 + (float) (level - 1) * 0.25));
        }
        if(damage_type == DAMAGE_MIND) {
          current_damage = std::max(0, (int32_t) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
          hp -= current_damage;
          status[damage_type] += current_damage * status_power * 100.F * (1.F - getStatusReductionFromType(damage_type)) / (Settings::getStatusThreshold(damage_type) * (1 + (float) (level - 1) * 0.25));
        }
        if(damage_type == DAMAGE_SOLAR) {
          current_damage -= damages[damage_type];
          hp -= current_damage;
          status[damage_type] += current_damage * status_power * 100.F * (1.F - getStatusReductionFromType(damage_type)) / (Settings::getStatusThreshold(damage_type) * (1 + (float) (level - 1) * 0.25));
        }
        if(damage_type == DAMAGE_AETHER) {
          payMana(damages[damage_type]);
          float status_inc = damages[damage_type] * status_power * 0.25;
          for(int32_t i; i < DAMAGE_TYPE_STATUS_NUMBER; i++) {
            status[i] += status_inc * 100.F * (1.F - getStatusReductionFromType(i)) / (Settings::getStatusThreshold(i) * (1 + (float) (level - 1) * 0.25));
          } 
        }
        else {
          current_damage = std::max(0, (int32_t) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
          damage += current_damage;
          if(damage_type < DAMAGE_TYPE_STATUS_NUMBER) {
            status[damage_type] += current_damage * status_power * 100.F * (1.F - getStatusReductionFromType(damage_type)) / (Settings::getStatusThreshold(damage_type) * (1 + (float) (level - 1) * 0.25));
          }
        }
      }
    }
    for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_STATUS_NUMBER; damage_type++) {
      while(status[damage_type] > 100.F) {
        status[damage_type] -= 100.F;
        std::array<float, DAMAGE_TYPE_NUMBER> effect_damages;
        Effect * effect;
        float potency = 0;
        int32_t duration = 60;
        switch(damage_type) {
          case DAMAGE_SLASH:
            potency = 4.F * duration;
            effect_damages[DAMAGE_TRUE] = getMaxHp() / potency;
            effect = new Effect("TXT_BLEEDING", ++effect::id_counter, 1, "", EFFECT_STATUS_BLEEDING, DURATION_TEMPORARY, getStatusPower(), duration, effect_damages);
            break;
          case DAMAGE_PUNCTURE:
            potency = 10.F * duration;
            effect_damages[DAMAGE_TRUE] = getMaxHp() / potency;
            effect = new Effect("TXT_WEAKENED", ++effect::id_counter, 1, "", EFFECT_STATUS_WEAKENED, DURATION_TEMPORARY, getStatusPower(), duration, effect_damages);
            break;
          case DAMAGE_BLUNT:
            potency = 10.F * duration;
            effect_damages[DAMAGE_TRUE] = getMaxHp() / potency;
            effect = new Effect("TXT_CONFUSED", ++effect::id_counter, 1, "", EFFECT_STATUS_CONFUSED, DURATION_TEMPORARY, getStatusPower(), duration, effect_damages);
            break;
          case DAMAGE_FIRE:
            duration = 12;
            potency = 10.F * duration;
            effect_damages[DAMAGE_NEUTRAL] = getMaxHp() / potency;
            effect = new Effect("TXT_BURNING", ++effect::id_counter, 1, "", EFFECT_STATUS_BURNING, DURATION_TEMPORARY, getStatusPower(), duration, effect_damages);
            break;
          case DAMAGE_LIGHTNING:
            duration = 12;
            potency = 10.F * duration;
            effect_damages[DAMAGE_NEUTRAL] = getMaxHp() / potency;
            effect = new Effect("TXT_SHOCKED", ++effect::id_counter, 1, "", EFFECT_STATUS_SHOCKED, DURATION_TEMPORARY, getStatusPower(), duration, effect_damages);
            break;
          case DAMAGE_FROST:
            potency = 10.F * duration;
            effect_damages[DAMAGE_NEUTRAL] = getMaxHp() / potency;
            effect = new Effect("TXT_FROZEN", ++effect::id_counter, 1, "", EFFECT_STATUS_FROZEN, DURATION_TEMPORARY, getStatusPower(), duration, effect_damages);
            break;
          case DAMAGE_POISON:
            potency = 10.F * duration;
            effect_damages[DAMAGE_POISON] = getMaxHp() / potency;
            effect = new Effect("TXT_POISONED", ++effect::id_counter, 1, "", EFFECT_STATUS_POISONED, DURATION_TEMPORARY, getStatusPower(), duration, effect_damages);
            break;
          case DAMAGE_ACID:
            potency = 10.F * duration;
            effect_damages[DAMAGE_NEUTRAL] = getMaxHp() / potency;
            effect = new Effect("TXT_CORRODED", ++effect::id_counter, 1, "", EFFECT_STATUS_CORRODED, DURATION_TEMPORARY, getStatusPower(), duration, effect_damages);
            break;
          case DAMAGE_MIND:
            potency = 10.F * duration;
            effect_damages[DAMAGE_TRUE] = getMaxHp() / potency;
            effect = new Effect("TXT_BROKEN", ++effect::id_counter, 1, "", EFFECT_STATUS_BROKEN, DURATION_TEMPORARY, getStatusPower(), duration, effect_damages);
            break;
          case DAMAGE_SOLAR:
            effect = new Effect("TXT_DISINTEGRATED", ++effect::id_counter, 1, "", EFFECT_STATUS_DISINTEGRATED, DURATION_INSTANT, 0.F, 0, effect_damages);
            break;
        }
        effect->activate(this);
      }
    }
    damage = std::max(0, damage);
    int32_t leftover = damage - shield;
    shield -= damage;
    hp -= std::max(0, leftover);
  }
}

int32_t Character::tryAttack(std::array<float, DAMAGE_TYPE_NUMBER> damages) {
  if(isInvulnerable() || isEtheral()) {
    return 0;
  }
  int32_t damage = 0;
  for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    if(damage_type == DAMAGE_ACID) {
      damage += damages[damage_type];
    }
    if(damage_type == DAMAGE_MIND) {
      damage += std::max(0, (int32_t) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
    }
    if(damage_type == DAMAGE_SOLAR) {
      damage += damages[damage_type];
    }
    if(damage_type == DAMAGE_AETHER) {
      damage += damages[damage_type];
    }
    else {
      damage += std::max(0, (int32_t) floor( (float) damages[damage_type] * (1.F - getDamageReductionFromType(damage_type))));
    }
  }
  return damage;
}

void Character::trade(Character * buyer, int32_t object_type, std::string object_name, float price_modifier) {
  int32_t price = 0;
  switch (object_type) {
    case OBJECT_ITEM:
      for(Item * item : sellable_items) {
        if(item->name == object_name) {
          price = (int32_t) std::ceil((float) item->gold_value * price_modifier);
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
          price = (int32_t) std::ceil((float) (skill->level * skill->level) * 1000.F * price_modifier);
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
          price = (int32_t) std::ceil((float) (effect->level * effect->level) * 1000.F * price_modifier);
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
  String::insert_float(ss, hp);
  String::insert_int(ss, getMaxHp());
  String::insert_float(ss, mana);
  String::insert_int(ss, getMaxMana());
  String::insert_float(ss, shield);
  String::insert_int(ss, getMaxShield());
  String::insert_float(ss, getHunger());
  String::insert_float(ss, getThirst());
  String::insert_float(ss, getStamina());
  String::insert_float(ss, getSanity());
  String::insert_int(ss, getCurrentSoulBurn());
  String::insert_int(ss, getSoulBurnThreshold());
  String::insert_int(ss, getFlow());
  String::insert_bool(ss, player_character);
  String::insert_int(ss, race->getType(race_modifiers));
  String::insert_float(ss, coord.x);
  String::insert_float(ss, coord.y);
  String::insert_float(ss, coord.z);
  String::insert_float(ss, speed.x);
  String::insert_float(ss, speed.y);
  String::insert_float(ss, speed.z);
  String::insert_float(ss, getSize().x);
  String::insert_float(ss, getSize().y);
  String::insert_float(ss, getSize().z);
  String::insert_float(ss, orientation.x);
  String::insert_float(ss, orientation.z);
  String::insert(ss, team);
  String::insert_int(ss, xp);
  String::insert_int(ss, level);
  if(talking_speech != nullptr) {
    String::insert(ss, ((Speech *) talking_speech)->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_float(ss, getDamageReductionFromType(i));
  }
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_float(ss, getDamageFromType(i, ITEM_SLOT_WEAPON_1));
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
  display->hp = String::extract_float(ss);
  display->maxHp = String::extract_int(ss);
  display->mana = String::extract_float(ss);
  display->maxMana = String::extract_int(ss);
  display->shield = String::extract_float(ss);
  display->maxShield = String::extract_int(ss);
  display->hunger = String::extract_float(ss);
  display->thirst = String::extract_float(ss);
  display->stamina = String::extract_float(ss);
  display->sanity = String::extract_float(ss);
  display->soulBurn = String::extract_int(ss);
  display->soulBurnTreshold = String::extract_int(ss);
  display->flow = String::extract_int(ss);
  display->player_character = String::extract_bool(ss);
  display->type = String::extract_int(ss);
  display->x = String::extract_float(ss);
  display->y = String::extract_float(ss);
  display->z = String::extract_float(ss);
  display->vx = String::extract_float(ss);
  display->vy = String::extract_float(ss);
  display->vz = String::extract_float(ss);
  display->sizeX = String::extract_float(ss);
  display->sizeY = String::extract_float(ss);
  display->sizeZ = String::extract_float(ss);
  display->orientation_x = String::extract_float(ss);
  display->orientation_z = String::extract_float(ss);
  display->team = String::extract(ss);
  display->xp = String::extract_int(ss);
  display->level = String::extract_int(ss);
  std::string talking_speech_str = String::extract(ss);
  display->talking_speech = nullptr;
  if(talking_speech_str != "none") {
    display->talking_speech = Speech::from_string(talking_speech_str);
  }
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damage_reductions[i] = String::extract_float(ss);
  }
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damages[i] = String::extract_float(ss);
  }
  delete ss;
  return display;
}

std::string Character::full_to_string(Adventure * adventure) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, maxHp);
  String::insert_int(ss, maxMana);
  String::insert_int(ss, maxShield);
  String::insert_float(ss, hp);
  String::insert_float(ss, mana);
  String::insert_float(ss, shield);
  String::insert_int(ss, damage_multiplier);
  String::insert_int(ss, soulBurnTreshold);
  String::insert_int(ss, flow);
  String::insert_int(ss, visionRange);
  String::insert_int(ss, visionPower);
  String::insert_int(ss, detectionRange);
  String::insert_int(ss, currentSoulBurn);
  String::insert_float(ss, hunger);
  String::insert_float(ss, thirst);
  String::insert_float(ss, stamina);
  String::insert_float(ss, sanity);
  String::insert_float(ss, channeledMana);
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
  String::insert_float(ss, speed.x);
  String::insert_float(ss, speed.y);
  String::insert_float(ss, speed.z);
  String::insert_float(ss, size.x);
  String::insert_float(ss, size.y);
  String::insert_float(ss, size.z);
  String::insert_float(ss, orientation.x);
  String::insert_float(ss, orientation.z);
  String::insert_bool(ss, merchant);
  String::insert_long(ss, gold);
  String::insert_long(ss, xp);
  String::insert_long(ss, last_level_xp);
  String::insert_int(ss, level);
  String::insert(ss, team);
  String::insert(ss, gear->to_string());
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : getEffects()) {
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
  String::insert_bool(ss, using_magical_stance);
  String::insert_int(ss, stance_attack); 
  std::stringstream * ss_active_stances = new std::stringstream();
  for(std::pair<const int32_t, Stance *> pair : active_stances) {
    String::insert_int(ss_active_stances, pair.first);
    String::insert(ss_active_stances, pair.second->name);
  }
  String::insert(ss, ss_active_stances->str());
  delete ss_active_stances;
  std::stringstream * ss_active_magical_stances = new std::stringstream();
  for(std::pair<const int32_t, Stance *> pair : active_magical_stances) {
    String::insert_int(ss_active_magical_stances, pair.first);
    String::insert(ss_active_magical_stances, pair.second->name);
  }
  String::insert(ss, ss_active_magical_stances->str());
  delete ss_active_magical_stances;  
  std::stringstream * ss_toggled_skills = new std::stringstream();
  for(std::pair<const Skill *, bool> pair : toggled_skills) {
    String::insert(ss_toggled_skills, pair.first->name);
    String::insert_bool(ss_toggled_skills, pair.second);
  }
  String::insert(ss, ss_toggled_skills->str());
  delete ss_toggled_skills;
  for(int32_t i = 0; i < DAMAGE_TYPE_STATUS_NUMBER; i++) {
    String::insert_float(ss, status[i]);
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

Character * Character::full_from_string(std::string to_read, Adventure * adventure) {
  std::stringstream * ss = new std::stringstream(to_read);
  int32_t maxHp = String::extract_int(ss);
  int32_t maxMana = String::extract_int(ss);
  int32_t maxShield = String::extract_int(ss);
  float hp = String::extract_float(ss);
  float mana = String::extract_float(ss);
  float shield = String::extract_float(ss);
  int32_t damage_multiplier = String::extract_int(ss);
  int32_t soulBurnTreshold = String::extract_int(ss);
  int32_t flow = String::extract_int(ss);
  int32_t visionRange = String::extract_int(ss);
  int32_t visionPower = String::extract_int(ss);
  int32_t detectionRange = String::extract_int(ss);
  int32_t currentSoulBurn = String::extract_int(ss);
  float hunger = String::extract_float(ss);
  float thirst = String::extract_float(ss);
  float stamina = String::extract_float(ss);
  float sanity = String::extract_float(ss);
  float channeledMana = String::extract_float(ss);
  std::string name = String::extract(ss);
  int64_t id = String::extract_long(ss);
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
  MathUtil::Vector3 coord = MathUtil::Vector3();
  coord.x = String::extract_float(ss);
  coord.y = String::extract_float(ss);
  coord.z = String::extract_float(ss);
  MathUtil::Vector3 speed = MathUtil::Vector3();
  speed.x = String::extract_float(ss);
  speed.y = String::extract_float(ss);
  speed.z = String::extract_float(ss);
  float sizeX = String::extract_float(ss);
  float sizeY = String::extract_float(ss);
  float sizeZ = String::extract_float(ss);
  float orientation_x = String::extract_float(ss);
  float orientation_z = String::extract_float(ss);
  bool merchant = String::extract_bool(ss);
  int64_t gold = String::extract_long(ss);
  int64_t xp = String::extract_long(ss);
  int64_t last_level_xp = String::extract_long(ss);
  int32_t level = String::extract_int(ss);
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
  bool using_magical_stance = String::extract_bool(ss);
  int32_t stance_attack = String::extract_int(ss);
  std::stringstream * ss_active_stances = new std::stringstream(String::extract(ss));
  std::map<int32_t, Stance *> * active_stances = new std::map<int32_t, Stance *>();
  while(ss_active_stances->rdbuf()->in_avail() != 0) {
    int32_t weapon_type = String::extract_int(ss_active_stances);
    active_stances->insert(std::make_pair(weapon_type, (Stance *) adventure->getDatabase()->getStance(String::extract(ss_active_stances))));
  }
  delete ss_active_stances;
  std::stringstream * ss_active_magical_stances = new std::stringstream(String::extract(ss));
  std::map<int32_t, Stance *> * active_magical_stances = new std::map<int32_t, Stance *>();
  while(ss_active_magical_stances->rdbuf()->in_avail() != 0) {
    int32_t weapon_type = String::extract_int(ss_active_magical_stances);
    active_magical_stances->insert(std::make_pair(weapon_type, (Stance *) adventure->getDatabase()->getStance(String::extract(ss_active_magical_stances))));
  }
  delete ss_active_magical_stances;
  std::stringstream * ss_toggled_skills = new std::stringstream(String::extract(ss));
  std::map<const Skill *, bool> * toggled_skills = new std::map<const Skill *, bool>();
  while(ss_toggled_skills->rdbuf()->in_avail() != 0) {
    const Skill * skill = adventure->getDatabase()->getSkill(String::extract(ss_toggled_skills));
    bool state = String::extract_bool(ss_toggled_skills);
    toggled_skills->insert(std::make_pair(skill, state));
  }
  delete ss_toggled_skills;
  std::array<float, DAMAGE_TYPE_STATUS_NUMBER> status;
  for(int32_t i = 0; i < DAMAGE_TYPE_STATUS_NUMBER; i++) {
    status[i] = String::extract_float(ss);
  }
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
    hunger,
    thirst,
    stamina,
    sanity,
    channeledMana,
    name,
    id,
    player_character,
    death_speech,
    talking_speech,
    coord,
    speed,
    sizeX,
    sizeY,
    sizeZ,
    orientation_x,
    orientation_z,
    nullptr,
    merchant,
    gold,
    xp,
    last_level_xp,
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
    *titles,
    using_magical_stance,
    stance_attack,
    *active_stances,
    *active_magical_stances,
    *toggled_skills,
    status
  );
  delete effects;
  delete skills;
  delete sellable_items;
  delete sellable_effects;
  delete sellable_skills;
  delete race_modifiers;
  delete titles;
  delete active_stances;
  delete active_magical_stances;
  delete toggled_skills;
  return result;
}
