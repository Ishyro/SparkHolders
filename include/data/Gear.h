#ifndef _GEAR_H_
#define _GEAR_H_

#include <list>

#include "data/Item.h"
#include "data/Weapon.h"
#include "Values.h"

class Gear {
  public:
    Gear(
      Item * head,
      Item * arms,
      Item * legs,
      Item * body,
      Item * lantern,
      Item * left_ring,
      Item * right_ring,
      Item * amulet,
      Weapon * weapon
    ):
      head(head),
      arms(arms),
      legs(legs),
      body(body),
      lantern(lantern),
      left_ring(left_ring),
      right_ring(right_ring),
      amulet(amulet),
      weapon(weapon)
    {}
    Gear(const Gear * gear):
      head(nullptr),
      arms(nullptr),
      legs(nullptr),
      body(nullptr),
      lantern(nullptr),
      left_ring(nullptr),
      right_ring(nullptr),
      amulet(nullptr),
      weapon(nullptr)
    {
      if(gear->head != nullptr) {
        head = new Item(gear->head);
      }
      if(gear->arms != nullptr) {
        arms = new Item(gear->arms);
      }
      if(gear->legs != nullptr) {
        legs = new Item(gear->legs);
      }
      if(gear->body != nullptr) {
        body = new Item(gear->body);
      }
      if(gear->lantern != nullptr) {
        lantern = new Item(gear->lantern);
      }
      if(gear->left_ring != nullptr) {
        left_ring = new Item(gear->left_ring);
      }
      if(gear->right_ring != nullptr) {
        right_ring = new Item(gear->right_ring);
      }
      if(gear->amulet != nullptr) {
        amulet = new Item(gear->amulet);
      }
      if(gear->weapon != nullptr) {
        weapon = new Weapon(gear->weapon);
      }
    }
    Gear():
      head(nullptr),
      arms(nullptr),
      legs(nullptr),
      body(nullptr),
      lantern(nullptr),
      left_ring(nullptr),
      right_ring(nullptr),
      amulet(nullptr),
      weapon(nullptr)
    {}
    std::list<Item *> equip(Item * new_item);
    Weapon * equip(Weapon * new_weapon);
    Item * unequip(int type);
    Weapon * unequipWeapon();

    float getDamageReductionFromType(int damage_type);

    Item * getHead();
    Item * getArms();
    Item * getLegs();
    Item * getBody();
    Item * getLantern();
    Item * getLeft_ring();
    Item * getRight_ring();
    Item * getAmulet();
    Weapon * getWeapon();
    float getWeight();
    std::string to_string();
    static Gear * from_string(std::string to_read);
  private:
    Item * head;
    Item * arms;
    Item * legs;
    Item * body;
    Item * lantern;
    Item * left_ring;
    Item * right_ring;
    Item * amulet;
    Weapon * weapon;
};

#endif // _GEAR_H_
