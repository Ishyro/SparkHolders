#ifndef _GEAR_H_
#define _GEAR_H_

#include "src/data/Item.h"
#include "src/data/Weapon.h"
#include "src/Values.h"

class Gear {
  public:
    Gear(Item head, Item arms, Item legs, Item left_ring, Item right_ring, Item amulet, Weapon weapon);
    Item equip(Item * new_item);
    Item getHead();
    Item getArms();
    Item getLegs();
    Item getBody();
    Item getLeft_ring();
    Item getRight_ring();
    Item getAmulet();
    Weapon getWeapon();
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
