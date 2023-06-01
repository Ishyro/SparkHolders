#ifndef _GEAR_H_
#define _GEAR_H_

#include <list>

#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "Values.h"

class Gear {
  public:
    const std::string name;
    Gear(
      std::string name,
      ArmorItem * head,
      ArmorItem * arms,
      ArmorItem * legs,
      ArmorItem * body,
      ArmorItem * lantern,
      ArmorItem * left_ring,
      ArmorItem * right_ring,
      ArmorItem * amulet,
      WeaponItem * weapon,
      std::list<Item *> items
    ):
      name(name),
      head(head),
      arms(arms),
      legs(legs),
      body(body),
      lantern(lantern),
      left_ring(left_ring),
      right_ring(right_ring),
      amulet(amulet),
      weapon(weapon),
      items(items)
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
      weapon(nullptr),
      items(std::list<Item *>())
    {
      if(gear->head != nullptr) {
        head = new ArmorItem(gear->head, gear->head->tier);
      }
      if(gear->arms != nullptr) {
        arms = new ArmorItem(gear->arms, gear->arms->tier);
      }
      if(gear->legs != nullptr) {
        legs = new ArmorItem(gear->legs, gear->legs->tier);
      }
      if(gear->body != nullptr) {
        body = new ArmorItem(gear->body, gear->body->tier);
      }
      if(gear->lantern != nullptr) {
        lantern = new ArmorItem(gear->lantern, gear->lantern->tier);
      }
      if(gear->left_ring != nullptr) {
        left_ring = new ArmorItem(gear->left_ring, gear->left_ring->tier);
      }
      if(gear->right_ring != nullptr) {
        right_ring = new ArmorItem(gear->right_ring, gear->right_ring->tier);
      }
      if(gear->amulet != nullptr) {
        amulet = new ArmorItem(gear->amulet, gear->amulet->tier);
      }
      if(gear->weapon != nullptr) {
        weapon = new WeaponItem(gear->weapon, gear->weapon->tier);
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
      weapon(nullptr),
      items(std::list<Item *>())
    {}
    std::list<GearItem *> equip(GearItem * new_item);
    GearItem * unequip(int type, int type2);

    float getDamageReductionFromType(int damage_type);

    ArmorItem * getHead();
    ArmorItem * getArms();
    ArmorItem * getLegs();
    ArmorItem * getBody();
    ArmorItem * getLantern();
    ArmorItem * getLeft_ring();
    ArmorItem * getRight_ring();
    ArmorItem * getAmulet();
    WeaponItem * getWeapon();
    std::list<Item *> getItems();
    float getWeight();
    int getArmor();
    std::string to_string();
    static Gear * from_string(std::string to_read, Adventure * adventure);
    bool operator == (const Gear& g) const { return name == g.name; }
    bool operator != (const Gear& g) const { return !operator==(g); }
  private:
    ArmorItem * head;
    ArmorItem * arms;
    ArmorItem * legs;
    ArmorItem * body;
    ArmorItem * lantern;
    ArmorItem * left_ring;
    ArmorItem * right_ring;
    ArmorItem * amulet;
    WeaponItem * weapon;
    std::list<Item *> items;
};

#endif // _GEAR_H_
