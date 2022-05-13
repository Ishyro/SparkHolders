#include "data/Gear.h"

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

std::string Gear::to_string() {
  std::string msg = "";
  if(head != nullptr) {
    msg += head->to_string() + "|";
  } else {
    msg += "none|";
  }
  if(arms != nullptr) {
    msg += arms->to_string() + "|";
  } else {
    msg += "none|";
  }
  if(legs != nullptr) {
    msg += legs->to_string() + "|";
  } else {
    msg += "none|";
  }
  if(body != nullptr) {
    msg += body->to_string() + "|";
  } else {
    msg += "none|";
  }
  if(left_ring != nullptr) {
    msg += left_ring->to_string() + "|";
  } else {
    msg += "none|";
  }
  if(right_ring != nullptr) {
    msg += right_ring->to_string() + "|";
  } else {
    msg += "none|";
  }
  if(amulet != nullptr) {
    msg += amulet->to_string() + "|";
  } else {
    msg += "none|";
  }
  if(weapon != nullptr) {
    msg += weapon->to_string() + "|";
  } else {
    msg += "none|";
  }
  return msg;
}

Gear * Gear::from_string(std::string to_read) {
  std::string msg = to_read;
  Item * head = Item::from_string(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  Item * arms = Item::from_string(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  Item * legs = Item::from_string(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  Item * body = Item::from_string(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  Item * left_ring = Item::from_string(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  Item * right_ring = Item::from_string(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  Item * amulet = Item::from_string(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  Weapon * weapon = Weapon::from_string(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
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
