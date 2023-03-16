#include "data/Projectile.h"

#include "data/Gear.h"

#include "utils/String.h"

std::list<Item *> Gear::equip(Item * new_item) {
  std::list<Item *> unequip;
  switch(new_item->type) {
    case HEAD:
      unequip.push_back(head);
      head = new_item;
      break;
    case ARMS:
      unequip.push_back(arms);
      arms = new_item;
      break;
    case BODY:
      unequip.push_back(body);
      body = new_item;
      break;
    case LEGS:
      unequip.push_back(legs);
      legs = new_item;
      break;
    case FULL_BODY:
      unequip.push_back(head);
      unequip.push_back(body);
      unequip.push_back(arms);
      unequip.push_back(legs);
      head = nullptr;
      arms = nullptr;
      legs = nullptr;
      body = new_item;
      break;
    case LANTERN:
      unequip.push_back(lantern);
      lantern = new_item;
      break;
    case RING:
      if(left_ring == nullptr) {
        left_ring = new_item;
      }
      else if(right_ring == nullptr) {
        right_ring = new_item;
      }
      else {
        unequip.push_back(left_ring);
        left_ring = new_item;
      }
      break;
    case AMULET:
      unequip.push_back(amulet);
      amulet = new_item;
      break;
    case UNEQUIPABLE:
      return unequip;
  }
  return unequip;
}

Weapon * Gear::equip(Weapon * new_weapon) {
  Weapon * temp = weapon;
  weapon = new_weapon;
  return temp;
}

Item * Gear::unequip(int type) {
  Item * old_item;
  switch(type) {
    case HEAD:
      old_item = head;
      head = nullptr;
      break;
    case ARMS:
      old_item = arms;
      arms = nullptr;
      break;
    case BODY:
      old_item = body;
      body = nullptr;
      break;
    case LEGS:
      old_item = legs;
      legs = nullptr;
      break;
    case FULL_BODY:
      old_item = body;
      body = nullptr;
      break;
    case LANTERN:
      old_item = lantern;
      lantern = nullptr;
      break;
    case LEFT_RING:
      old_item = left_ring;
      left_ring = nullptr;
      break;
    case RIGHT_RING:
      old_item = right_ring;
      right_ring = nullptr;
      break;
    case AMULET:
      old_item = amulet;
      amulet = nullptr;
      break;
  }
  return old_item;
}

Weapon * Gear::unequipWeapon() {
  Weapon * temp = weapon;
  weapon = nullptr;
  return temp;
}

float Gear::getDamageReductionFromType(int damage_type) {
  float resistance = 0.;
  if(head != nullptr) {
    resistance += head->getDamageReductionFromType(damage_type);
  }
  if(arms != nullptr) {
    resistance += arms->getDamageReductionFromType(damage_type);
  }
  if(body != nullptr) {
    resistance += body->getDamageReductionFromType(damage_type);
  }
  if(legs != nullptr) {
    resistance += legs->getDamageReductionFromType(damage_type);
  }
  if(lantern != nullptr) {
    resistance += lantern->getDamageReductionFromType(damage_type);
  }
  if(left_ring != nullptr) {
    resistance += left_ring->getDamageReductionFromType(damage_type);
  }
  if(right_ring != nullptr) {
    resistance += right_ring->getDamageReductionFromType(damage_type);
  }
  if(amulet != nullptr) {
    resistance += amulet->getDamageReductionFromType(damage_type);
  }
  return resistance;
}

Item * Gear::getHead() { return head; }
Item * Gear::getArms() { return arms; }
Item * Gear::getLegs() { return legs; }
Item * Gear::getBody() { return body; }
Item * Gear::getLantern() { return lantern; }
Item * Gear::getLeft_ring() { return left_ring; }
Item * Gear::getRight_ring() { return right_ring; }
Item * Gear::getAmulet() { return amulet; }
Weapon * Gear::getWeapon() { return weapon; }
std::list<Item *> Gear::getItems() { return items; }
std::list<Weapon *> Gear::getWeapons() { return weapons; }
std::list<Ammunition *> Gear::getAmmunitions() { return ammunition; }

float Gear::getWeight() {
  float weight = 0.F;
  if(head != nullptr) {
    weight += head->weight;
  }
  if(arms != nullptr) {
    weight += arms->weight;
  }
  if(body != nullptr) {
    weight += body->weight;
  }
  if(legs != nullptr) {
    weight += legs->weight;
  }
  if(lantern != nullptr) {
    weight += lantern->weight;
  }
  if(weapon != nullptr) {
    weight += weapon->weight;
  }
  return std::max(1.F, weight);
}

std::string Gear::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  if(head != nullptr) {
    String::insert(ss, head->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(arms != nullptr) {
    String::insert(ss, arms->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(legs != nullptr) {
    String::insert(ss, legs->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(body != nullptr) {
    String::insert(ss, body->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(lantern != nullptr) {
    String::insert(ss, lantern->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(left_ring != nullptr) {
    String::insert(ss, left_ring->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(right_ring != nullptr) {
    String::insert(ss, right_ring->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(amulet != nullptr) {
    String::insert(ss, amulet->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(weapon != nullptr) {
    String::insert(ss, weapon->to_string());
  }
  else {
    String::insert(ss, "none");
  }
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
  std::string result = ss->str();
  delete ss;
  return result;
}

Gear * Gear::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  Item * head = Item::from_string(String::extract(ss));
  Item * arms = Item::from_string(String::extract(ss));
  Item * legs = Item::from_string(String::extract(ss));
  Item * body = Item::from_string(String::extract(ss));
  Item * lantern = Item::from_string(String::extract(ss));
  Item * left_ring = Item::from_string(String::extract(ss));
  Item * right_ring = Item::from_string(String::extract(ss));
  Item * amulet = Item::from_string(String::extract(ss));
  Weapon * weapon = Weapon::from_string(String::extract(ss));
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
  delete ss;
  Gear * result = new Gear(
    name,
    head,
    arms,
    legs,
    body,
    lantern,
    left_ring,
    right_ring,
    amulet,
    weapon,
    *items,
    *weapons,
    *ammunition
  );
  delete items;
  delete weapons;
  delete ammunition;
  return result;
}
