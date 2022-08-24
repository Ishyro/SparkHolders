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
Item * Gear::getLeft_ring() { return left_ring; }
Item * Gear::getRight_ring() { return right_ring; }
Item * Gear::getAmulet() { return amulet; }
Weapon * Gear::getWeapon() { return weapon; }

float Gear::getWeight() {
  float weight = 1.F;
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
  if(weapon != nullptr) {
    weight += weapon->weight;
  }
  return weight;
}

std::string Gear::to_string() {
  std::stringstream * ss = new std::stringstream();
  if(head != nullptr) {
    String::insert(ss, head->to_string());
  } else {
    String::insert(ss, "none");
  }
  if(arms != nullptr) {
    String::insert(ss, arms->to_string());
  } else {
    String::insert(ss, "none");
  }
  if(legs != nullptr) {
    String::insert(ss, legs->to_string());
  } else {
    String::insert(ss, "none");
  }
  if(body != nullptr) {
    String::insert(ss, body->to_string());
  } else {
    String::insert(ss, "none");
  }
  if(left_ring != nullptr) {
    String::insert(ss, left_ring->to_string());
  } else {
    String::insert(ss, "none");
  }
  if(right_ring != nullptr) {
    String::insert(ss, right_ring->to_string());
  } else {
    String::insert(ss, "none");
  }
  if(amulet != nullptr) {
    String::insert(ss, amulet->to_string());
  } else {
    String::insert(ss, "none");
  }
  if(weapon != nullptr) {
    String::insert(ss, weapon->to_string());
  } else {
    String::insert(ss, "none");
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

Gear * Gear::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  Item * head = Item::from_string(String::extract(ss));
  Item * arms = Item::from_string(String::extract(ss));
  Item * legs = Item::from_string(String::extract(ss));
  Item * body = Item::from_string(String::extract(ss));
  Item * left_ring = Item::from_string(String::extract(ss));
  Item * right_ring = Item::from_string(String::extract(ss));
  Item * amulet = Item::from_string(String::extract(ss));
  Weapon * weapon = Weapon::from_string(String::extract(ss));
  return new Gear(
    head,
    arms,
    legs,
    body,
    left_ring,
    right_ring,
    amulet,
    weapon
  );
}
