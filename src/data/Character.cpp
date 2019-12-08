#include <algorithm>

#include "src/data/Character.h"

bool Character::isAlive() {return hp > 0;}
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

std::list<Item *> Character::getStuff() {return stuff;}
std::list<Weapon *> Character::getWeapons() {return weapons;}
std::list<Effect *> Character::getEffects() {return effects;}
std::list<Skill *> Character::getSkills() {return skills;}

void Character::move(int orientation) {this->orientation = orientation;}
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

// Warning : Dangerous
void Character::useSkill(Skill * skill, World * world, int overcharge) {
  skill->activate(this, world, overcharge);
}
