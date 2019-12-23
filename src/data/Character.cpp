#include <algorithm>

#include "src/data/Character.h"

bool Character::isAlive() {return hp > 0 && mana > 0;}
bool Character::isSoulBurning() {return currentSoulBurn >= soulBurnTreshold;}
int Character::getX() {return x;}
int Character::getY() {return y;}
int Character::getOrientation() {return orientation;}
int Character::getHp() {return hp;}
int Character::getMaxHp() {
  int bonus = 0;
  for (auto e : effects)
    if (e->type == HP_MAX)
      bonus += e->power;
  return maxHp + bonus;
}

int Character::getMana() {return mana;}

int Character::getMaxMana() {
  int bonus = 0;
  for (auto e : effects)
    if (e->type == MANA_MAX)
      bonus += e->power;
  return maxMana + bonus;
}

int Character::getArmor() {
  int bonus = 0;
  for (auto e : effects)
    if (e->type == ARMOR)
      bonus += e->power;
  return armor + bonus;
}

int Character::getSoulBurnTreshold() {
  int bonus = 0;
  for (auto e : effects)
    if (e->type == SOULBURNTRESHOLD)
      bonus += e->power;
  return soulBurnTreshold + bonus;
}

int Character::getCurrentSoulBurn() {return currentSoulBurn;}

int Character::getFlow() {
  int bonus = 0;
  for (auto e : effects)
    if (e->type == FLOW)
      bonus += e->power;
  return flow + bonus;
}

int Character::getVision() {
  int bonus = 0;
  for (auto e : effects)
    if (e->type == VISION)
      bonus += e->power;
  return vision + bonus;
}

int Character::getDarkVision() {
  int bonus = 0;
  for (auto e : effects)
    if (e->type == DARK_VISION)
      bonus += e->power;
  return dark_vision + bonus;
}

long Character::getGold() {return gold;}

long Character::getCurrentMapId() {return current_map_id;}

std::list<Item *> Character::getStuff() {return stuff;}
std::list<Weapon *> Character::getWeapons() {return weapons;}
std::list<Effect *> Character::getEffects() {return effects;}
std::list<Skill *> Character::getSkills() {return skills;}

void Character::move(int orientation) {this->orientation = orientation;}
void Character::move(int x, int y) {
  this->x = x;
  this->y = y;
}
void Character::move(int x, int y, int orientation) {
  this->x = x;
  this->y = y;
  this->orientation = orientation;
}

void Character::hpHeal(int hp) {this->hp = std::min(this->hp + hp, getMaxHp());}
void Character::incrMaxHp() {maxHp += attributes->hpIncr;}
void Character::manaHeal(int mana) {this->mana = std::min(this->mana + mana, getMaxMana());}
void Character::incrMaxMana() {maxMana += attributes->manaIncr;}
void Character::incrArmor() {armor += attributes->armorIncr;}
void Character::incrSoulBurnTreshold() {soulBurnTreshold += attributes->soulBurnIncr;}
void Character::incrFlow() {flow += attributes->flowIncr;}
void Character::setVision(int vision) {this->vision = vision;}
void Character::setDarkVision(int dark_vision) {this->dark_vision = dark_vision;}
void Character::setCurrentMapId(long map_id) {this->current_map_id = map_id;}

void Character::equip(Item * to_equip) {
  if (to_equip != nullptr) {
    std::list<Item *> items = gear->equip(to_equip);
    for (auto item : items) {
      for (auto e : item->effects) {
        removeEffect(e);
      }
      stuff.push_front(item);
    }
    for (auto e : to_equip->effects) {
      addEffect(e);
    }
  }
}

void Character::equip(Weapon * to_equip) {
  if (to_equip != nullptr) {
    Weapon * old_weapon = gear->equip(to_equip);
    for (auto e : old_weapon->effects) {
      removeEffect(e);
    }
    weapons.push_front(old_weapon);
    for (auto e : to_equip->effects) {
      addEffect(e);
    }
  }
}

void Character::unequip(int type) {
  Item * old_item = gear->unequip(type);
  if (old_item != nullptr) {
    stuff.push_front(old_item);
  }
}

void Character::unequipWeapon() {
  Weapon * old_weapon = gear->unequipWeapon();
  if (old_weapon != nullptr) {
    weapons.push_front(old_weapon);
  }
}

void Character::addEffect(Effect * e) {effects.push_front(e);}
void Character::addSkill(Skill * s) {skills.push_front(s);}
void Character::removeEffect(Effect * e) {effects.remove(e);}
void Character::removeSkill(Skill * s) {skills.remove(s);}

void Character::changeWay(std::string old_way, std::string new_way) {
  // TODO
}

void Character::addItem(Item * i) {stuff.push_front(i);}
void Character::addWeapon(Weapon * w) {weapons.push_front(w);}
void Character::removeItem(Item * i) {stuff.remove(i);}
void Character::removeWeapon(Weapon * w) {weapons.remove(w);}

int Character::isChanneling() {
  for (auto e : effects) {
    if (e->type == CHANNELING) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::isStunned() {
  for (auto e : effects) {
    if (e->type == STUNNED) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::isCloaked() {
  for (auto e : effects) {
    if (e->type == CLOAKED) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::isInvisible() {
  for (auto e : effects) {
    if (e->type == INVISIBLE) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::isSleeping() {
  for (auto e : effects) {
    if (e->type == SLEEPING) {
      return e->getTickLeft();
    }
  }
  return 0;
}

int Character::cloakPower() {
  for (auto e : effects) {
    if (e->type == CLOAKED) {
      return e->power;
    }
  }
  return 0;
}

int Character::invisibilityPower() {
  for (auto e : effects) {
    if (e->type == INVISIBLE) {
      return e->power;
    }
  }
  return 0;
}

bool Character::isInWeakState() {
  for (auto e : effects) {
    if (e->type == STUNNED || e->type == SLEEPING) {
      return true;
    }
  }
  return 0;
}

bool Character::isTileLighted(World * world) {
  return world->getMap(current_map_id)->isTileLighted(x, y);
}

Quest * Character::getQuest() {return quest;}

// Warning : Dangerous
void Character::useSkill(Skill * skill, World * world, int overcharge) {
  skill->activate(this, world, overcharge);
}

void Character::receiveAttack(int damage, int damage_type) {
  if (isInWeakState()) {
    return receiveCriticalAttack(damage, damage_type);
  }
  if (damage_type == SOUL) {
    hp -= damage;
    mana -= damage;
  }
  if (damage_type == TRUE) {
    hp -= damage;
  }
  else {
    int final_damage = (int) floor(( (float) (damage) * (1. - gear->getDamageReduction(damage_type)) - getArmor()));
    if (final_damage > 0)
      hp -= final_damage;
  }
}

void Character::receiveCriticalAttack(int damage, int damage_type) {
  if (damage_type == SOUL) {
    hp -= damage * 2;
    mana -= damage * 2;
  }
  if (damage_type == TRUE) {
    hp -= damage * 2;
  }
  else {
    int final_damage = (int) floor(( (float) (damage) * (1. - .5 * gear->getDamageReduction(damage_type)) - getArmor()));
    if (final_damage > 0)
      hp -= final_damage * 2;
  }
}
