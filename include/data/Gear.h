#ifndef _GEAR_H_
#define _GEAR_H_

#include <list>

#include "Values.h"

class Gear {
  public:
    Gear(
      Item * head,
      Item * arms,
      Item * legs,
      Item * body,
      Item * left_ring,
      Item * right_ring,
      Item * amulet,
      Weapon * weapon,
      Ammunition * ammunition
    ):
      head(head),
      arms(arms),
      legs(legs),
      body(body),
      left_ring(left_ring),
      right_ring(right_ring),
      amulet(amulet),
      weapon(weapon),
      ammunition(ammunition)
    {}
    Gear(const Gear * gear):
      head(gear->head),
      arms(gear->arms),
      legs(gear->legs),
      body(gear->body),
      left_ring(gear->left_ring),
      right_ring(gear->right_ring),
      amulet(gear->amulet),
      weapon(gear->weapon),
      ammunition(gear->ammunition)
    {}
    std::list<Item *> equip(Item * new_item);
    Weapon * equip(Weapon * new_weapon);
    Ammunition * equip(Ammunition * new_ammunition);
    Item * unequip(int type);
    Weapon * unequipWeapon();
    Ammunition * unequipAmmunition();

    float getDamageReduction(int damage_type);

    Item * getHead();
    Item * getArms();
    Item * getLegs();
    Item * getBody();
    Item * getLeft_ring();
    Item * getRight_ring();
    Item * getAmulet();
    Weapon * getWeapon();
    Ammunition * getAmmunition();
  private:
    Item * head;
    Item * arms;
    Item * legs;
    Item * body;
    Item * left_ring;
    Item * right_ring;
    Item * amulet;
    Weapon * weapon;
    Ammunition * ammunition;
};

#endif // _GEAR_H_
