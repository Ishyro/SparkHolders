#include "data/Projectile.h"

#include "data/items/Item.h"
#include "data/items/GearItem.h"
#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/Gear.h"

#include "util/String.h"

std::list<GearItem *> Gear::equip(GearItem * new_item) {
  std::list<GearItem *> unequip = std::list<GearItem *>();
  if(new_item->type == ITEM_WEAPON) {
      unequip.push_back(weapon);
      weapon = (WeaponItem *) new_item;
  }
  else if(new_item->type == ITEM_ARMOR) {
    switch(new_item->type2) {
      case ITEM_HEAD:
        unequip.push_back(head);
        head = (ArmorItem *) new_item;
        break;
      case ITEM_ARMS:
        unequip.push_back(arms);
        arms = (ArmorItem *) new_item;
        break;
      case ITEM_BODY:
        unequip.push_back(body);
        body = (ArmorItem *) new_item;
        break;
      case ITEM_LEGS:
        unequip.push_back(legs);
        legs = (ArmorItem *) new_item;
        break;
      case ITEM_FULL_BODY:
        unequip.push_back(head);
        unequip.push_back(body);
        unequip.push_back(arms);
        unequip.push_back(legs);
        head = nullptr;
        arms = nullptr;
        legs = nullptr;
        body = (ArmorItem *) new_item;
        break;
      case ITEM_LANTERN:
        unequip.push_back(lantern);
        lantern = (ArmorItem *) new_item;
        break;
      case ITEM_RING:
        if(left_ring == nullptr) {
          left_ring = (ArmorItem *) new_item;
        }
        else if(right_ring == nullptr) {
          right_ring = (ArmorItem *) new_item;
        }
        else {
          unequip.push_back(left_ring);
          left_ring = (ArmorItem *) new_item;
        }
        break;
      case ITEM_AMULET:
        unequip.push_back(amulet);
        amulet = (ArmorItem *) new_item;
        break;
    }
  }
  return unequip;
}

GearItem * Gear::unequip(int type, int type2) {
  GearItem * old_item;
  if(type == ITEM_WEAPON) {
      old_item = weapon;
      weapon = nullptr;
  }
  else if(type == ITEM_ARMOR) {
    switch(type2) {
      case ITEM_HEAD:
        old_item = head;
        head = nullptr;
        break;
      case ITEM_ARMS:
        old_item = arms;
        arms = nullptr;
        break;
      case ITEM_BODY:
        old_item = body;
        body = nullptr;
        break;
      case ITEM_LEGS:
        old_item = legs;
        legs = nullptr;
        break;
      case ITEM_FULL_BODY:
        old_item = body;
        body = nullptr;
        break;
      case ITEM_LANTERN:
        old_item = lantern;
        lantern = nullptr;
        break;
      case ITEM_LEFT_RING:
        old_item = left_ring;
        left_ring = nullptr;
        break;
      case ITEM_RIGHT_RING:
        old_item = right_ring;
        right_ring = nullptr;
        break;
      case ITEM_AMULET:
        old_item = amulet;
        amulet = nullptr;
        break;
    }
  }
  return old_item;
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

ArmorItem * Gear::getHead() { return head; }
ArmorItem * Gear::getArms() { return arms; }
ArmorItem * Gear::getLegs() { return legs; }
ArmorItem * Gear::getBody() { return body; }
ArmorItem * Gear::getLantern() { return lantern; }
ArmorItem * Gear::getLeft_ring() { return left_ring; }
ArmorItem * Gear::getRight_ring() { return right_ring; }
ArmorItem * Gear::getAmulet() { return amulet; }
WeaponItem * Gear::getWeapon() { return weapon; }
std::list<Item *> Gear::getItems() { return items; }

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
  std::string result = ss->str();
  delete ss;
  return result;
}

Gear * Gear::from_string(std::string to_read, Adventure * adventure) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  ArmorItem * head = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * arms = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * legs = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * body = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * lantern = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * left_ring = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * right_ring = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * amulet = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  WeaponItem * weapon = (WeaponItem *) Item::from_string(String::extract(ss), adventure);
  std::stringstream * ss_items = new std::stringstream(String::extract(ss));
  std::list<Item *> * items = new std::list<Item *>();
  while(ss_items->rdbuf()->in_avail() != 0) {
    items->push_back(Item::from_string(String::extract(ss_items), adventure));
  }
  delete ss_items;
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
    *items
  );
  delete items;
  return result;
}
