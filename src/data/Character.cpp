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

#include "utils/String.h"

void Character::applyAttributes(Attributes * attributes, bool init) {
  maxHp=attributes->baseHp;
  maxMana=attributes->baseMana;
  hp=maxHp;
  mana=maxMana;
  armor=attributes->baseArmor;
  damage_multiplier=attributes->baseDamage;
  soulBurnTreshold=attributes->baseSoulBurn;
  flow=attributes->baseFlow;
  visionRange=attributes->baseVisionRange;
  visionPower=attributes->baseVisionPower;
  detectionRange=attributes->baseDetectionRange;
  currentSoulBurn = 0;
  currentFlow = 0;
  stamina = 75.;
  satiety = 75.;
  savedHpRegen = 0.;
  savedManaRegen = 0.;
  if(init) {
    gear = new Gear(attributes->getStartingGear());
    for(Item * item : attributes->getItems()) {
      Item * toadd = new Item(item);
      items.push_back(toadd);
    }
    for(Weapon * weapon : attributes->getWeapons()) {
      Weapon * toadd = new Weapon(weapon);
      weapons.push_back(toadd);
    }
    for(Ammunition * ammo : attributes->getAmmunitions()) {
      Ammunition * toadd = new Ammunition();
      toadd->projectile = ammo->projectile;
      toadd->number = ammo->number;
      toadd->gold_value = ammo->gold_value;
      toadd->ammo_type = ammo->ammo_type;
      ammunition.push_back(toadd);
    }
    for(Effect * effect : attributes->getEffects()) {
      Effect * toadd = new Effect(effect, 1, 1);
      toadd->activate(this);
    }
    for(Skill * skill : attributes->getSkills()) {
      addSkill(skill);
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
int Character::getX() { return x; }
int Character::getY() { return y; }
int Character::getOrientation() { return orientation; }
int Character::getHp() { return hp; }
int Character::getMaxHp() {
  int bonus = 0;
  for(auto e : effects)
    if(e->type == HP_MAX)
      bonus += e->power;
  return maxHp + bonus;
}

int Character::getMana() { return mana; }

int Character::getMaxMana() {
  int bonus = 0;
  for(auto e : effects)
    if(e->type == MANA_MAX)
      bonus += e->power;
  return std::max(maxMana + bonus, 0);
}

float Character::getStamina() { return stamina; }
float Character::getSatiety() { return satiety; }

int Character::getArmor() {
  int bonus = 0;
  for(auto e : effects)
    if(e->type == ARMOR)
      bonus += e->power;
  return std::max(armor + bonus, 0);
}

int Character::getSoulBurnTreshold() {
  int bonus = 0;
  for(auto e : effects)
    if(e->type == SOULBURNTRESHOLD)
      bonus += e->power;
  return std::max(soulBurnTreshold + bonus, 0);
}

int Character::getCurrentSoulBurn() { return currentSoulBurn; }

int Character::getFlow() {
  int bonus = 0;
  for(auto e : effects)
    if(e->type == FLOW)
      bonus += e->power;
  return std::max(flow + bonus, 0);
}

int Character::getVisionRange() {
  int bonus = 0;
  for(auto e : effects)
    if(e->type == VISION_RANGE)
      bonus += e->power;
  return std::max(visionRange + bonus, 0);
}

int Character::getVisionPower() {
  int bonus = 0;
  for(auto e : effects)
    if(e->type == VISION_POWER)
      bonus += e->power;
  return std::max(visionPower + bonus, 0);
}

int Character::getDetectionRange() {
  int bonus = 0;
  for(auto e : effects)
    if(e->type == DETECTION_RANGE)
      bonus += e->power;
  return std::max(detectionRange + bonus, 0);
}

long Character::getGold() { return gold; }
long Character::getXP() { return xp; }
int Character::getLevel() { return level; }

float Character::getPriorityModifier() {
  return std::max(0.F, (std::min(75.F, stamina) - std::abs(50.F - satiety)) / gear->getWeight());
}

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

AI * Character::getAI() { return ai; }
std::string Character::getTeam() { return team; }
Speech * Character::getDeathSpeech() { return death_speech; }
Speech * Character::getTalkingSpeech() { return talking_speech; }

int Character::getCurrentMapId() { return current_map_id; }

Gear * Character::getGear() { return gear; }
std::list<Item *> Character::getItems() { return items; }
std::list<Item *> Character::getLoot() { return loot; }
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

std::list<Way *> Character::getTitles() { return titles; }

void Character::setOrientation(int orientation) { this->orientation = orientation; }
void Character::move(int orientation) {
  switch(orientation) {
    case NORTH:
      y++;
      break;
    case NORTH_EAST:
      y++;
      x++;
      break;
    case EAST:
      x++;
      break;
    case SOUTH_EAST:
      y--;
      x++;
      break;
    case SOUTH:
      y--;
      break;
    case SOUTH_WEST:
      y--;
      x--;
      break;
    case WEST:
      x--;
      break;
    case NORTH_WEST:
      y++;
      x--;
      break;
  }
  this->orientation = orientation;
}

void Character::move(int y, int x) {
  this->x = x;
  this->y = y;
}
void Character::move(int y, int x, int orientation) {
  this->x = x;
  this->y = y;
  this->orientation = orientation;
}

void Character::hpHeal(int hp) { this->hp = std::min(this->hp + hp, getMaxHp()); }
void Character::incrMaxHp() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
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
  int currentManaHeal = std::max(0, std::min(mana, getFlow() - currentFlow));
  int realManaHeal = std::min(currentManaHeal, getMaxMana() - this->mana);
  this->mana += realManaHeal;
  currentFlow += realManaHeal;
}

void Character::incrMaxMana() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
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

void Character::incrArmor() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += race->armorIncr;
  incr += origin->armorIncr;
  incr += culture->armorIncr;
  incr += religion->armorIncr;
  incr += profession->armorIncr;
  for(Way * title : titles) {
    incr += title->armorIncr;
  }
  armor += std::max(incr, 0);
}
void Character::incrDamageMultiplier() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
  incr += race->damageIncr;
  incr += origin->damageIncr;
  incr += culture->damageIncr;
  incr += religion->damageIncr;
  incr += profession->damageIncr;
  for(Way * title : titles) {
    incr += title->damageIncr;
  }
  damage_multiplier += std::max(incr, 0);
}
void Character::incrSoulBurnTreshold() {
  if(player_character) {
    setNeedToSend(true);
  }
  int incr = 0;
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
  currentSoulBurn = std::max(0, currentSoulBurn - soulBurnReduction) + std::max(0, 2 * (currentFlow - getFlow()));
  currentFlow = 0;
}

void Character::applyTiredness() {
  if(need_to_sleep) {
    float step = 100.F / (Settings::getDayDurationInRound() * Settings::getMaxNumberOfDaysAwake());
    float currentManaRegen = Settings::getStaminaRecoveryRatio() * step * getMaxMana() / 100.F;
    int manaValue = (int) std::floor(currentManaRegen + savedManaRegen);
    savedManaRegen += currentManaRegen - (float) manaValue;
    if(stamina > 0.) {
      removeStamina(step);
      manaHeal(manaValue);
    } else {
      manaValue = (int) std::floor(currentManaRegen * Settings::getStaminaOverextendRatio() + savedManaRegen);
      savedManaRegen += currentManaRegen - (float) manaValue;
      payMana(manaValue);
    }
  }
}

void Character::applyHunger() {
  if(need_to_eat) {
    float step = 100.F / (Settings::getDayDurationInRound() * Settings::getMaxNumberOfDaysFasting());
    float currentHpRegen = Settings::getSatietyRecoveryRatio() * step * getMaxHp() / 100.F;
    int hpValue = (int) std::floor(currentHpRegen + savedHpRegen);
    savedHpRegen += currentHpRegen - (float) hpValue;
    if(satiety > 0.) {
      removeSatiety(step);
      hpHeal(hpValue);
    } else {
      hpValue = (int) std::floor(currentHpRegen * Settings::getSatietyOverextendRatio() + savedHpRegen);
      savedHpRegen += currentHpRegen - (float) hpValue;
      hp -= hpValue;
    }
  }
}

void Character::applyEffects() {
  for(Effect * e : effects) {
    if(e->duration_type == TEMPORARY) {
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
  if(need_to_sleep) {
    // +1 because the character will still apply his tiredness while sleeping
    addStamina( (float) (3 + 1) * 100.F / (Settings::getDayDurationInRound() * Settings::getMaxNumberOfDaysAwake()));
  }
}

void Character::gainGold(long gold) { this->gold += gold; }
void Character::loseGold(long gold) { this->gold = (long) std::max(0., (double) this->gold + gold); }
void Character::payMana(int cost) {
  mana -= cost;
  currentSoulBurn += cost;
  currentFlow += std::abs(cost);
}
void Character::gainXP(long xp) { this->xp += xp; }
void Character::gainLevel() {
  while(xp >= level * level * 1000) { // INSERT FORMULA HERE
    level++;
    incrMaxHp();
    incrMaxMana();
    incrArmor();
    incrDamageMultiplier();
    incrSoulBurnTreshold();
    incrFlow();
    hpHeal(getMaxHp() / 2);
    manaHeal(getMaxMana() / 2);
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

void Character::equip(Item * to_equip) {
  if(player_character) {
    setNeedToSend(true);
  }
  if(to_equip != nullptr) {
    std::list<Item *> items = gear->equip(to_equip);
    for(auto item : items) {
      for(auto e : item->effects) {
        removeEffect(e);
      }
      items.push_back(item);
    }
    for(auto e : to_equip->effects) {
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
      for(auto e : old_weapon->effects) {
        removeEffect(e);
      }
      addWeapon(old_weapon);
    }
    for(auto e : to_equip->effects) {
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
    for(auto e : old_item->effects) {
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

void Character::useItem(std::string item) {
  Item * to_remove = nullptr;
  for(Item * i : items) {
    if(i->name == item && i->consumable) {
      if(player_character) {
        setNeedToSend(true);
      }
      if(!i->isFood() || can_eat_food) {
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

bool Character::isChanneling() {
  for(auto e : effects) {
    if(e->type == CHANNELING) {
      return true;
    }
  }
  return false;
}

bool Character::isStunned() {
  for(auto e : effects) {
    if(e->type == STUNNED) {
      return true;
    }
  }
  return false;
}

bool Character::isCloaked() {
  for(auto e : effects) {
    if(e->type == CLOAKED) {
      return true;
    }
  }
  return false;
}

bool Character::isInvisible() {
  for(auto e : effects) {
    if(e->type == INVISIBLE) {
      return true;
    }
  }
  return false;
}

bool Character::isEtheral() {
  for(auto e : effects) {
    if(e->type == ETHERAL) {
      return true;
    }
  }
  return false;
}

bool Character::isInvulnerable() {
  for(auto e : effects) {
    if(e->type == INVULNERABLE) {
      return true;
    }
  }
  return false;
}

bool Character::isSleeping() {
  for(auto e : effects) {
    if(e->type == SLEEPING) {
      return true;
    }
  }
  return false;
}

int Character::cloakPower() {
  int max = 0;
  for(auto e : effects) {
    if(e->type == CLOAKED) {
      return e->power;
    }
  }
  return max;
}

bool Character::isInWeakState() {
  for(auto e : effects) {
    if(e->type == STUNNED || e->type == SLEEPING) {
      return true;
    }
  }
  return false;
}

// Warning : Dangerous
void Character::useSkill(Skill * skill, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range, int x, int y) {
  skill->activate(this, target, adventure, overcharge_power, overcharge_duration, overcharge_range, current_map_id, x, y);
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

Projectile * Character::shoot(const Character * target, int y, int x) {
  if(!gear->getWeapon()->melee && gear->getWeapon()->range >= MapUtil::distance(this->x, this->y, x, y)) {
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
      MapUtil::Pair pair = MapUtil::getNextPairFromOrientation(orientation, this->x, this->y);
      return new Projectile(base_projectile, realDamages, current_map_id, pair.x, pair.y, x, y, (Character *) target, this, orientation, 1, 1, 1);
    }
  }
  return nullptr;
}

void Character::attack(Character * target) {
  if(gear->getWeapon()->melee && gear->getWeapon()->range >= std::max(abs(x - target->getX()), abs(y - target->getY()))) {
    int realDamages[DAMAGE_TYPE_NUMBER];
    for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
      realDamages[damage_type] = getDamageFromType(damage_type);
    }
    target->receiveAttack(realDamages, orientation);
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

void Character::receiveAttack(int damages[DAMAGE_TYPE_NUMBER], int orientation) {
  if(!isInvulnerable() && !isEtheral()) {
    if(orientation != NO_ORIENTATION) {
      int diff = abs(8 + orientation - this->orientation) % 8;
      if(diff <= 1 || diff >= 7) {
        return receiveCriticalAttack(damages);
      }
    }
    if(isInWeakState()) {
      return receiveCriticalAttack(damages);
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
    hp -= std::max(0, damage - getArmor());
  }
}

void Character::receiveCriticalAttack(int damages[DAMAGE_TYPE_NUMBER]) {
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
        } else {
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
        } else {
          damage += std::max(0, (int) floor( (float) (damages[damage_type] * 2) * (1.F - damage_reduction)));
        }
      }
    }
    hp -= std::max(0, damage - getArmor());
  }
}

int Character::tryAttack(std::array<int, DAMAGE_TYPE_NUMBER> damages) {
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
  return trueDamage + std::max(0, damage - getArmor());
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
        if(skill->name == object_name && buyer->level >= 5 * skill->level && (skill->attributes == "" || buyer->attributes == skill->attributes)) {
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
        if(effect->name == object_name && buyer->level >= 5 * effect->level && (effect->attributes == "" || buyer->attributes == effect->attributes)) {
          price = (int) std::ceil((float) (effect->level * effect->level) * 1000.F * price_modifier);
          if(buyer->getGold() >= price) {
            effect->activate(buyer);
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
  String::insert_int(ss, x - offsetX);
  String::insert_int(ss, y - offsetY);
  String::insert_int(ss, orientation);
  String::insert(ss, team);
  String::insert_int(ss, getArmor());
  String::insert_int(ss, xp);
  String::insert_int(ss, level);
  if(talking_speech != nullptr) {
    String::insert(ss, ((Speech *) talking_speech)->to_string());
  } else {
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
  display->x = String::extract_int(ss);
  display->y = String::extract_int(ss);
  display->orientation = String::extract_int(ss);
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
  String::insert(ss, name);
  String::insert_bool(ss, player_character);
  if(death_speech != nullptr) {
    String::insert(ss, ((Speech *) death_speech)->to_string());
  } else {
    String::insert(ss, "none");
  }
  if(talking_speech != nullptr) {
    String::insert(ss, ((Speech *) talking_speech)->to_string());
  } else {
    String::insert(ss, "none");
  }
  std::stringstream * ss_loot = new std::stringstream();
  for(Item * item : loot) {
    String::insert(ss_loot, item->to_string());
  }
  String::insert(ss, ss_loot->str());
  delete ss_loot;
  String::insert_int(ss, type);
  String::insert_int(ss, x);
  String::insert_int(ss, y);
  String::insert_int(ss, orientation);
  String::insert_int(ss, current_map_id);
  String::insert_bool(ss, has_soulspark);
  String::insert_bool(ss, need_to_eat);
  String::insert_bool(ss, can_eat_food);
  String::insert_bool(ss, need_to_sleep);
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

  String::insert(ss, attributes);
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
  std::string name = String::extract(ss);
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
  std::stringstream * ss_loot = new std::stringstream(String::extract(ss));
  std::list<Item *> * loot = new std::list<Item *>();
  while(ss_loot->rdbuf()->in_avail() != 0) {
    loot->push_back(Item::from_string(String::extract(ss_loot)));
  }
  delete ss_loot;
  int type = String::extract_int(ss);
  int x = String::extract_int(ss);
  int y = String::extract_int(ss);
  int orientation = String::extract_int(ss);
  int current_map_id = String::extract_int(ss);
  bool has_soulspark = String::extract_bool(ss);
  bool need_to_eat = String::extract_bool(ss);
  bool can_eat_food = String::extract_bool(ss);
  bool need_to_sleep = String::extract_bool(ss);
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
  std::string attributes = String::extract(ss);
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
    name,
    player_character,
    death_speech,
    talking_speech,
    *loot,
    type,
    x,
    y,
    orientation,
    current_map_id,
    has_soulspark,
    need_to_eat,
    can_eat_food,
    need_to_sleep,
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
    race,
    origin,
    culture,
    religion,
    profession,
    *titles
  );
  delete loot;
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
  for(Item * item : loot) {
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
