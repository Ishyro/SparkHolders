#include <sstream>

#include "ai/AI.h"
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

#include "utils/String.h"

void Character::applyAttributes(const Attributes * attributes, bool applyGear) {
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
  if(applyGear) {
    gear = new Gear(attributes->startingGear);
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
    void setMana(int mana);

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
int Character::getLevel() { return level; }

AI * Character::getAI() { return ai; }
std::string Character::getTeam() { return team; }

int Character::getCurrentMapId() { return current_map_id; }

Gear * Character::getGear() { return gear; }
std::list<Item *> Character::getItems() { return items; }
std::list<Weapon *> Character::getWeapons() { return weapons; }
std::list<Ammunition *> Character::getAmmunitions() { return ammunition; }
std::list<Effect *> Character::getEffects() { return effects; }
std::list<Skill *> Character::getSkills() { return skills; }

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
  int incr = 0;
  incr += race->hpIncr;
  incr += origin->hpIncr;
  incr += culture->hpIncr;
  incr += religion->hpIncr;
  incr += profession->hpIncr;
  maxHp += std::max(incr, 0);
}
void Character::setHp(int hp) { this->hp = hp; }
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
void Character::setMana(int mana) { this->mana = mana; }
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
void Character::setCurrentSoulBurn(int soulBurn) { this->currentSoulBurn = soulBurn; }
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

void Character::setCurrentMapId(int map_id) { this->current_map_id = map_id; }

void Character::applySoulBurn() {
  int soulBurnReduction = (int) std::max( (float) currentSoulBurn / 10., (float) soulBurnTreshold / 10.);
  if(currentSoulBurn > soulBurnTreshold) {
    hp -= std::min(soulBurnReduction, currentSoulBurn - soulBurnTreshold);
  }
  currentSoulBurn = std::max(0, currentSoulBurn - soulBurnReduction);
}

void Character::gainGold(long gold) { this->gold += gold; }
void Character::payMana(int cost) {
  mana -= cost;
  currentSoulBurn += cost;
}
void Character::gainXP(long xp) { this->xp += xp; }
void Character::gainLevel() {
  while(xp >= level * level * 1000) { // INSERT FORMULA HERE
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
bool Character::hasSkill(Skill * s) {
  for(Skill * skill : skills) {
    if(skill->name == s->name) {
      return true;
    }
  }
  return false;
}
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
void Character::addAmmunition(Ammunition * a) { ammunition.push_back(a); }
void Character::removeItem(Item * i) { items.remove(i); }
void Character::removeWeapon(Weapon * w) { weapons.remove(w); }
void Character::removeAmmunition(Ammunition * a) { ammunition.remove(a); }

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
void Character::useSkill(Skill * skill, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_area, int x, int y) {
  skill->activate(this, target, adventure, overcharge_power, overcharge_duration, overcharge_area, current_map_id, x, y);
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

Projectile * Character::shoot(const Character * target, int y, int x) {
  if(!gear->getWeapon()->melee) {
    if(!gear->getWeapon()->use_ammo || gear->getWeapon()->getCurrentCapacity() > 0) {
      int realDamages[DAMAGE_TYPE_NUMBER];
      for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
        realDamages[damage_type] = getDamageFromType(damage_type);
      }
      Projectile * base_projectile = gear->getWeapon()->getAmmo()->projectile;
      if(gear->getWeapon()->use_ammo) {
        gear->getWeapon()->useAmmo();
      }
      int proj_x = this->x;
      int proj_y = this->y;
      switch(orientation) {
        case NORTH:
          proj_y++;
          break;
        case NORTH_EAST:
          proj_y++;
          proj_x++;
          break;
        case EAST:
          proj_x++;
          break;
        case SOUTH_EAST:
          proj_y--;
          proj_x++;
          break;
        case SOUTH:
          proj_y--;
          break;
        case SOUTH_WEST:
          proj_y--;
          proj_x--;
          break;
        case WEST:
          proj_x--;
          break;
        case NORTH_WEST:
          proj_y++;
          proj_x--;
          break;
      }
      return new Projectile(base_projectile, realDamages, current_map_id, proj_x, proj_y, x, y, (Character *) target, this, orientation, 1, 1, 1);
    }
  }
  return nullptr;
}

void Character::reload(Ammunition * ammo) {
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

void Character::receiveAttack(int damages[DAMAGE_TYPE_NUMBER], int orientation) {
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
    if(damage_type == SOUL_DAMAGE) {
      hp -= damages[damage_type];
      payMana(damages[damage_type]);
    }
    if(damage_type == TRUE_DAMAGE) {
      hp -= damages[damage_type];
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
      hp -= damages[damage_type] * 2;
      payMana(damages[damage_type] * 2);
    }
    if(damage_type == TRUE_DAMAGE) {
      hp -= damages[damage_type] * 2;
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

std::string Character::to_string(int offsetY, int offsetX) {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, getHp());
  String::insert_int(ss, getMaxHp());
  String::insert_int(ss, getMana());
  String::insert_int(ss, getMaxMana());
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
  String::insert(ss, name);
  String::insert_bool(ss, player_character);
  String::insert_int(ss, type);
  String::insert_int(ss, x);
  String::insert_int(ss, y);
  String::insert_int(ss, orientation);
  String::insert_int(ss, current_map_id);
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

  String::insert(ss, ( (Attributes *) adventure->getDatabase()->getAttributes(attributes))->to_string());
  String::insert(ss, race->to_string());
  String::insert(ss, origin->to_string());
  String::insert(ss, culture->to_string());
  String::insert(ss, religion->to_string());
  String::insert(ss, profession->to_string());
  std::string result = ss->str();
  delete ss;
  return result;
}

Character * Character::full_from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string  name = String::extract(ss);
  bool player_character = String::extract_bool(ss);
  int type = String::extract_int(ss);
  int x = String::extract_int(ss);
  int y = String::extract_int(ss);
  int orientation = String::extract_int(ss);
  int current_map_id = String::extract_int(ss);
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
  Attributes * attributes = Attributes::from_string(String::extract(ss));
  Way * race = Way::from_string(String::extract(ss));
  Way * origin = Way::from_string(String::extract(ss));
  Way * culture = Way::from_string(String::extract(ss));
  Way * religion = Way::from_string(String::extract(ss));
  Way * profession = Way::from_string(String::extract(ss));
  delete ss;
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
    *items,
    *weapons,
    *ammunition,
    *effects,
    *skills,
    attributes,
    race,
    origin,
    culture,
    religion,
    profession
  );
}
