#include <sstream>

#include "ai/AI.h"
#include "data/Attributes.h"
#include "data/Database.h"
#include "data/Effect.h"
#include "data/Gear.h"
#include "data/Item.h"
#include "data/Projectile.h"
#include "data/skills/Skill.h"
#include "data/Way.h"
#include "data/Weapon.h"
#include "data/World.h"

#include "data/Character.h"

void Character::applyAttributes(const Attributes * attributes) {
  this->attributes = attributes->name;
  maxHp=attributes->baseHp;
  maxMana=attributes->baseMana;
  hp=maxHp;
  mana=maxMana;
  armor=attributes->baseArmor;
  soulBurnTreshold=attributes->baseSoulBurn;
  flow=attributes->baseFlow;
  visionRange=attributes->baseVisionRange;
  visionPower=attributes->baseVisionPower;
  detectionRange=attributes->baseDetectionRange;
  currentSoulBurn = 0;
  gear = new Gear(attributes->startingGear);
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
long Character::getLevel() { return level; }

AI * Character::getAI() { return ai; }
std::string Character::getTeam() { return team; }

long Character::getCurrentMapId() { return current_map_id; }

std::list<Item *> Character::getItems() { return items; }
std::list<Weapon *> Character::getWeapons() { return weapons; }
std::list<Ammunition *> Character::getAmmunitions() { return ammunitions; }
std::list<Effect *> Character::getEffects() { return effects; }
std::list<Skill *> Character::getSkills() { return skills; }

void Character::setOrientation(int orientation) { this->orientation == orientation; }
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
}

void Character::move(int x, int y) {
  this->x = x;
  this->y = y;
}
void Character::move(int x, int y, int orientation) {
  this->x = x;
  this->y = y;
  this->orientation = orientation;
}

void Character::hpHeal(int hp) { this->hp = std::min(this->hp + hp, getMaxHp()); }
void Character::incrMaxHp() {
  int incr = 0;
  incr += race->hpIncr;
  incr += origin->hpIncr;
  incr += culture->hpIncr;
  incr += religion->hpIncr;
  incr += profession->hpIncr;
  maxHp += std::max(incr, 0);
}
void Character::manaHeal(int mana) { this->mana = std::min(this->mana + mana, getMaxMana()); }
void Character::incrMaxMana() {
  int incr = 0;
  incr += race->manaIncr;
  incr += origin->manaIncr;
  incr += culture->manaIncr;
  incr += religion->manaIncr;
  incr += profession->manaIncr;
  maxMana += std::max(incr, 0);
}
void Character::incrArmor() {
  int incr = 0;
  incr += race->armorIncr;
  incr += origin->armorIncr;
  incr += culture->armorIncr;
  incr += religion->armorIncr;
  incr += profession->armorIncr;
  armor += std::max(incr, 0);
}
void Character::incrSoulBurnTreshold() {
  int incr = 0;
  incr += race->soulBurnIncr;
  incr += origin->soulBurnIncr;
  incr += culture->soulBurnIncr;
  incr += religion->soulBurnIncr;
  incr += profession->soulBurnIncr;
  soulBurnTreshold += std::max(incr, 0);
}
void Character::incrFlow() {
  int incr = 0;
  incr += race->flowIncr;
  incr += origin->flowIncr;
  incr += culture->flowIncr;
  incr += religion->flowIncr;
  incr += profession->flowIncr;
  flow += std::max(incr, 0);
}
void Character::incrVisionRange() { visionRange++; }
void Character::incrVisionPower() {visionPower++; }
void Character::incrDetectionRange() { detectionRange++; }

void Character::setCurrentMapId(long map_id) { this->current_map_id = map_id; }

void Character::applySoulBurn() {
  int soulBurnReduction = (int) std::max( (float) currentSoulBurn / 10., (float) soulBurnTreshold / 10.);
  if(currentSoulBurn > soulBurnTreshold) {
    hp -= std::min(soulBurnReduction, currentSoulBurn - soulBurnTreshold);
  }
  currentSoulBurn -= soulBurnReduction;
}

void Character::gainGold(long gold) { this->gold += gold; }
void Character::payMana(int cost) {
  mana -= cost;
  currentSoulBurn += cost;
}
void Character::gainXP(long xp) { this->xp += xp; }
void Character::gainLevel() {
  while(level * level * 1000 >= xp) { // INSERT FORMULA HERE
    level++;
    incrMaxHp();
    incrMaxMana();
    incrArmor();
    incrSoulBurnTreshold();
    incrFlow();
    hpHeal(getMaxHp() / 2);
    manaHeal(getMaxMana() / 2);
  }
}

void Character::setAI(AI * ai) { this->ai = ai; }
void Character::setTeam(std::string team) { this->team = team; }

void Character::equip(Item * to_equip) {
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
  if(to_equip != nullptr) {
    Weapon * old_weapon = gear->equip(to_equip);
    if(old_weapon != nullptr) {
      for(auto e : old_weapon->effects) {
        removeEffect(e);
      }
      weapons.push_back(old_weapon);
    }
    for(auto e : to_equip->effects) {
      addEffect(e);
    }
  }
}

void Character::unequip(int type) {
  Item * old_item = gear->unequip(type);
  if(old_item != nullptr) {
    for(auto e : old_item->effects) {
      removeEffect(e);
    }
    items.push_back(old_item);
  }
}

void Character::unequipWeapon() {
  Weapon * old_weapon = gear->unequipWeapon();
  if(old_weapon != nullptr) {
    for(auto e : old_weapon->effects) {
      removeEffect(e);
    }
    weapons.push_back(old_weapon);
  }
}

void Character::addEffect(Effect * e) { effects.push_back(e); }
void Character::addSkill(Skill * s) { skills.push_back(s); }
void Character::removeEffect(Effect * e) { effects.remove(e); }
void Character::removeSkill(Skill * s) { skills.remove(s); }

void Character::setWay(Way * way) {
  switch(way->type) {
    case RACE: race = way;
    case ORIGIN: origin = way;
    case CULTURE: culture = way;
    case RELIGION: religion = way;
    case PROFESSION: profession = way;
    default:;
  }
}

void Character::addItem(Item * i) { items.push_back(i); }
void Character::addWeapon(Weapon * w) { weapons.push_back(w); }
void Character::addAmmunition(Ammunition * a) { ammunitions.push_back(a); }
void Character::removeItem(Item * i) { items.remove(i); }
void Character::removeWeapon(Weapon * w) { weapons.remove(w); }
void Character::removeAmmunition(Ammunition * a) { ammunitions.remove(a); }

int Character::isChanneling() {
  int max = 0;
  for(auto e : effects) {
    if(e->type == CHANNELING) {
      max = std::max(max, e->getTickLeft());
    }
  }
  return max;
}

int Character::isStunned() {
  int max = 0;
  for(auto e : effects) {
    if(e->type == STUNNED) {
      max = std::max(max, e->getTickLeft());
    }
  }
  return max;
}

int Character::isCloaked() {
  int max = 0;
  for(auto e : effects) {
    if(e->type == CLOAKED) {
      max = std::max(max, e->getTickLeft());
    }
  }
  return max;
}

int Character::isInvisible() {
  int max = 0;
  for(auto e : effects) {
    if(e->type == INVISIBLE) {
      max = std::max(max, e->getTickLeft());
    }
  }
  return max;
}

int Character::isSleeping() {
  int max = 0;
  for(auto e : effects) {
    if(e->type == SLEEPING) {
      max = std::max(max, e->getTickLeft());
    }
  }
  return max;
}

int Character::cloakPower() {
  int max = 0;
  for(auto e : effects) {
    if(e->type == CLOAKED) {
      return e->power;
    }
  }
  return 0;
}

bool Character::isInWeakState() {
  for(auto e : effects) {
    if(e->type == STUNNED || e->type == SLEEPING) {
      return true;
    }
  }
  return 0;
}

// Warning : Dangerous
void Character::useSkill(Skill * skill, Character * target, Adventure * adventure, long overcharge) {
  skill->activate(this, target, adventure, overcharge);
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
  return damage;
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

void Character::attack(Character * target) {
  int realDamages[DAMAGE_TYPE_NUMBER];
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    realDamages[damage_type] = getDamageFromType(damage_type);
  }
  target->receiveAttack(realDamages, orientation);
}

Projectile * Character::shoot() {
  if(!gear->getWeapon()->melee) {
    if(!gear->getWeapon()->use_ammo || gear->getWeapon()->getCurrentCapacity() > 0) {
      if(gear->getWeapon()->use_ammo) {
        gear->getWeapon()->useAmmo();
      }
      int realDamages[DAMAGE_TYPE_NUMBER];
      for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
        realDamages[damage_type] = getDamageFromType(damage_type);
      }
      return nullptr;//new Projectile();
    }
  }
  return nullptr;
}

void Character::reload(Ammunition * ammo) {
  Ammunition * oldAmmo = gear->getWeapon()->reload(ammo);
  if(ammo->number == 0) {
    ammunitions.remove(ammo);
    delete ammo;
  }
  if(oldAmmo != nullptr) {
    ammunitions.push_back(oldAmmo);
  }
}

void Character::receiveAttack(int damages[DAMAGE_TYPE_NUMBER], int orientation) {
  if(orientation != NO_ORIENTATION) {
    int diff = abs(orientation - this->orientation) % 8;
    if(diff <= 1 && diff >= 7) {
      return receiveCriticalAttack(damages);
    }
  }
  if(isInWeakState()) {
    return receiveCriticalAttack(damages);
  }
  int damage = 0;
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    if(damage_type == SOUL_DAMAGE) {
      hp -= damage;
      payMana(damage);
    }
    if(damage_type == TRUE_DAMAGE) {
      hp -= damage;
    }
    if(damage_type == NEUTRAL_DAMAGE) {
      damage += damages[damage_type];
    }
    else {
      damage += std::max(0, (int) floor( (float) damages[damage_type] * (1. - getDamageReductionFromType(damage_type))));
    }
  }
  hp -= std::max(0, damage - getArmor());
}

void Character::receiveCriticalAttack(int damages[DAMAGE_TYPE_NUMBER]) {
  int damage = 0;
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    if(damage_type == SOUL_DAMAGE) {
      hp -= damage * 2;
      payMana(damage * 2);
    }
    if(damage_type == TRUE_DAMAGE) {
      hp -= damage * 2;
    }
    if(damage_type == NEUTRAL_DAMAGE) {
      damage += damages[damage_type] * 2;
    }
    else {
      damage += std::max(0, (int) floor( (float) (damages[damage_type] * 2) * (1. - .5 * getDamageReductionFromType(damage_type))));
    }
  }
  hp -= std::max(0, damage - getArmor());
}

std::string Character::to_string(long offsetY, long offsetX) {
  std::string msg = name + ";";
  msg += std::to_string(id) + ";";
  msg += std::to_string(hp) + ";";
  msg += std::to_string(mana) + ";";
  msg += std::to_string(player_character) + ";";
  msg += std::to_string(type) + ";";
  msg += std::to_string(x - offsetX) + ";";
  msg += std::to_string(y - offsetY) + ";";
  msg += std::to_string(orientation) + ";";
  msg += team + ";";
  msg += std::to_string(getArmor()) + ";";
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    msg += std::to_string(getDamageReductionFromType(i)) + ";";
  }
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    msg += std::to_string(getDamageFromType(i)) + ";";
  }
  return msg;
}

CharacterDisplay * Character::from_string(std::string to_read) {
  std::string msg = to_read;
  CharacterDisplay * display = new CharacterDisplay();
  display->name = msg.substr(0, msg.find(';'));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->id = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->hp = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->mana = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  std::string player_character_str = msg.substr(0, msg.find(';'));
  display->player_character = (player_character_str == "1");
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->type = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->x = stol(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->y = stol(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->orientation = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->team = msg.substr(0, msg.find(';'));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damage_reductions[i] = stof(msg.substr(0, msg.find(';')));
    msg = msg.substr(msg.find(';') + 1, msg.length());
  }
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damages[i] = stoi(msg.substr(0, msg.find(';')));
    msg = msg.substr(msg.find(';') + 1, msg.length());
  }
  return display;
}

std::string Character::full_to_string(Adventure * adventure) {
  std::string msg = name + "@";
  msg += std::to_string(player_character) + "@";
  msg += std::to_string(type) + "@";
  msg += std::to_string(x) + "@";
  msg += std::to_string(y) + "@";
  msg += std::to_string(orientation) + "@";
  msg += std::to_string(current_map_id) + "@";
  msg += std::to_string(gold) + "@";
  msg += std::to_string(xp) + "@";
  msg += std::to_string(level) + "@";
  msg += team + "@";
  msg += gear->to_string() + "@";
  for(Item * item : items) {
    msg += item->to_string(); + "|";
  }
  msg += "@";
  for(Weapon * weapon : weapons) {
    msg += weapon->to_string(); + "|";
  }
  msg += "@";
  for(Ammunition * ammo : ammunitions) {
    msg += Projectile::ammo_to_string(ammo); + "|";
  }
  msg += "@";
  for(Effect * effect : effects) {
    msg += effect->to_string(); + "|";
  }
  msg += "@";
  for(Skill * skill : skills) {
    msg += skill->to_string(); + "|";
  }
  msg += "@";
  msg += ( (Attributes *) adventure->getDatabase()->getAttributes(attributes))->to_string() + "@";
  msg += race->to_string() + "@";
  msg += origin->to_string() + "@";
  msg += culture->to_string() + "@";
  msg += religion->to_string() + "@";
  msg += profession->to_string() + "@";
  return msg;
}

Character * Character::full_from_string(std::string to_read) {
  std::string msg = to_read;
  std::string name = msg.substr(0, msg.find('@'));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::string player_character_str = msg.substr(0, msg.find('@'));
  bool player_character = (player_character_str == "1");
  msg = msg.substr(msg.find('@') + 1, msg.length());
  int type = stoi(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  long x = stol(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  long y = stol(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  int orientation = stoi(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  int current_map_id = stoi(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  int gold = stoi(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  int xp = stoi(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  int level = stoi(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::string team = msg.substr(0, msg.find('@'));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  Gear * gear = Gear::from_string(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::list<Item *> * items = new std::list<Item *>();
  std::istringstream isItems(msg.substr(0, msg.find('@')));
  std::string item;
  while(getline(isItems, item, '|') && item != "") {
    items->push_back(Item::from_string(item));
  }
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::list<Weapon *> * weapons = new std::list<Weapon *>();
  std::istringstream isWeapons(msg.substr(0, msg.find('@')));
  std::string weapon;
  while(getline(isWeapons, weapon, '|') && weapon != "") {
    weapons->push_back(Weapon::from_string(weapon));
  }
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::list<Ammunition *> * ammunitions = new std::list<Ammunition *>();
  std::istringstream isAmmunitions(msg.substr(0, msg.find('@')));
  std::string ammo;
  while(getline(isAmmunitions, ammo, '|') && ammo != "") {
    ammunitions->push_back(Projectile::ammo_from_string(ammo));
  }
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::list<Effect *> * effects = new std::list<Effect *>();
  std::istringstream isEffects(msg.substr(0, msg.find('@')));
  std::string effect;
  while(getline(isEffects, effect, '|') && effect != "") {
    effects->push_back(Effect::from_string(effect));
  }
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::list<Skill *> * skills = new std::list<Skill *>();
  std::istringstream isSkills(msg.substr(0, msg.find('@')));
  std::string skill;
  while(getline(isSkills, skill, '|') && skill != "") {
    skills->push_back(Skill::from_string(skill));
  }
  msg = msg.substr(msg.find('@') + 1, msg.length());
  Attributes * attributes = Attributes::from_string(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  Way * race = Way::from_string(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  Way * origin = Way::from_string(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  Way * culture = Way::from_string(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  Way * religion = Way::from_string(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  Way * profession = Way::from_string(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  return new Character(
    name,
    player_character,
    type,
    x,
    y,
    orientation,
    current_map_id,
    gold,
    xp,
    level,
    team,
    gear,
    items,
    weapons,
    ammunitions,
    effects,
    skills,
    attributes,
    race,
    origin,
    culture,
    religion,
    profession
  );
}
