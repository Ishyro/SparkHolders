#ifndef _GEAR_H_
#define _GEAR_H_

#include <list>

#include "src/data/Item.h"
#include "src/data/Weapon.h"
#include "src/Values.h"

class Gear {
  public:
    Gear(Item * head, Item * arms, Item * legs, Item * body, Item * left_ring, Item * right_ring, Item * amulet, Weapon * weapon):
      head(head),
      arms(arms),
      legs(legs),
      body(body),
      left_ring(left_ring),
      right_ring(right_ring),
      amulet(amulet),
      weapon(weapon) {}
    std::list<Item *> equip(Item * new_item);
    Weapon * equip(Weapon * new_weapon);
    Item * unequip(int type);
    Weapon * unequipWeapon();
    Item * getHead();
    Item * getArms();
    Item * getLegs();
    Item * getBody();
    Item * getLeft_ring();
    Item * getRight_ring();
    Item * getAmulet();
    Weapon * getWeapon();
  private:
    Item * head;
    Item * arms;
    Item * legs;
    Item * body;
    Item * left_ring;
    Item * right_ring;
    Item * amulet;
    Weapon * weapon;
};

#endif // _GEAR_H_
