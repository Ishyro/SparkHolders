#include "data/Character.h"

void Character::applyAttributes(const Attributes * attributes) {
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

long Character::getCurrentMapId() { return current_map_id; }

std::list<Item *> Character::getStuff() { return stuff; }
std::list<Weapon *> Character::getWeapons() { return weapons; }
std::list<Effect *> Character::getEffects() { return effects; }
std::list<Skill *> Character::getSkills() { return skills; }

void Character::move(int orientation) { this->orientation = orientation; }
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

void Character::gainGold(long gold) { this->gold += gold; }
void Character::payMana(int cost) {
  mana -= cost;
  currentSoulBurn += cost;
}
void Character::gainXP(long xp) { this->xp += xp; }
void Character::gainLevel() {
  if(level * level * 1000 >= xp) { // INSERT FORMULA HERE
    level++;
    incrFlow();
    incrArmor();
    incrMaxHp();
    incrMaxMana();
    incrSoulBurnTreshold();
    incrFlow();
    hpHeal(getMaxHp() / 2);
    manaHeal(getMaxMana() / 2);
  }
}

void Character::equip(Item * to_equip) {
  if(to_equip != nullptr) {
    std::list<Item *> items = gear->equip(to_equip);
    for(auto item : items) {
      for(auto e : item->effects) {
        removeEffect(e);
      }
      stuff.push_front(item);
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
      weapons.push_front(old_weapon);
    }
    for(auto e : to_equip->effects) {
      addEffect(e);
    }
  }
}

void Character::equip(Ammunition * to_equip) {
  if(to_equip != nullptr) {
    Ammunition * old_ammunition = gear->equip(to_equip);
    if(old_ammunition != nullptr) {
      ammunitions.push_front(old_ammunition);
    }
  }
}

void Character::unequip(int type) {
  Item * old_item = gear->unequip(type);
  if(old_item != nullptr) {
    for(auto e : old_item->effects) {
      removeEffect(e);
    }
    stuff.push_front(old_item);
  }
}

void Character::unequipWeapon() {
  Weapon * old_weapon = gear->unequipWeapon();
  if(old_weapon != nullptr) {
    for(auto e : old_weapon->effects) {
      removeEffect(e);
    }
    weapons.push_front(old_weapon);
  }
}

void Character::unequipAmmunition() {
  Ammunition * old_ammunition = gear->unequipAmmunition();
  if(old_ammunition != nullptr) {
    ammunitions.push_front(old_ammunition);
  }
}

void Character::addEffect(Effect * e) { effects.push_front(e); }
void Character::addSkill(Skill * s) { skills.push_front(s); }
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

void Character::addItem(Item * i) { stuff.push_front(i); }
void Character::addWeapon(Weapon * w) { weapons.push_front(w); }
void Character::addAmmunition(Ammunition * a) { ammunitions.push_front(a); }
void Character::removeItem(Item * i) { stuff.remove(i); }
void Character::removeWeapon(Weapon * w) { weapons.remove(w); }
void Character::removeAmmunition(Ammunition * a) { ammunitions.remove(a); }

int Character::isChanneling() {
  for(auto e : effects) {
    if(e->type == CHANNELING) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::isStunned() {
  for(auto e : effects) {
    if(e->type == STUNNED) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::isCloaked() {
  for(auto e : effects) {
    if(e->type == CLOAKED) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::isInvisible() {
  for(auto e : effects) {
    if(e->type == INVISIBLE) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::isSleeping() {
  for(auto e : effects) {
    if(e->type == SLEEPING) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::cloakPower() {
  for(auto e : effects) {
    if(e->type == CLOAKED) {
      return e->power;
    }
  }
  return 0;
}

int Character::invisibilityPower() {
  for(auto e : effects) {
    if(e->type == INVISIBLE) {
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

void Character::receiveAttack(int damage, int damage_type, int orientation) {
  if(orientation != NO_ORIENTATION) {
    int diff = abs(orientation - this->orientation) % 8;
    if(diff >= 3 && diff <= 5) {
      return receiveCriticalAttack(damage, damage_type);
    }
  }
  if(isInWeakState()) {
    return receiveCriticalAttack(damage, damage_type);
  }
  if(damage_type == SOUL) {
    hp -= damage;
    mana -= damage;
  }
  if(damage_type == TRUE) {
    hp -= damage;
  }
  else {
    int final_damage = (int) floor(( (float) (damage) * (1. - gear->getDamageReduction(damage_type)) - getArmor()));
    if(final_damage > 0) {
      hp -= final_damage;
    }
  }
}

void Character::receiveCriticalAttack(int damage, int damage_type) {
  if(damage_type == SOUL) {
    hp -= damage * 2;
    mana -= damage * 2;
  }
  if(damage_type == TRUE) {
    hp -= damage * 2;
  }
  else {
    int final_damage = (int) floor(( (float) (damage) * (1. - .5 * gear->getDamageReduction(damage_type)) - getArmor()));
    if(final_damage > 0) {
      hp -= final_damage * 2;
    }
  }
}
