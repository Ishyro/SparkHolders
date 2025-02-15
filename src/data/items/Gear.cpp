#include "data/Projectile.h"

#include "data/items/Item.h"
#include "data/items/GearItem.h"
#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/Gear.h"

#include "data/Effect.h"

#include "util/String.h"

void Gear::equip(GearItem * item, int32_t slot) {
  switch(slot) {
    case ITEM_SLOT_MANTLE:
      mantle = (ArmorItem *) item;
      break;
    case ITEM_SLOT_HELMET:
      helmet = (ArmorItem *) item;
      break;
    case ITEM_SLOT_GAUNTLETS:
      gauntlets = (ArmorItem *) item;
      break;
    case ITEM_SLOT_ARMOR:
      armor = (ArmorItem *) item;
      break;
    case ITEM_SLOT_BOOTS:
      boots = (ArmorItem *) item;
      break;
    case ITEM_SLOT_LANTERN:
      lantern = (ArmorItem *) item;
      break;
    case ITEM_SLOT_AMULET:
      amulet = (ArmorItem *) item;
      break;
    case ITEM_SLOT_RING_1:
      ring_1 = (ArmorItem *) item;
      break;
    case ITEM_SLOT_RING_2:
      ring_2 = (ArmorItem *) item;
      break;
    case ITEM_SLOT_WEAPON_1:
      weapon_1 = (WeaponItem *) item;
      break;
    case ITEM_SLOT_WEAPON_2:
      weapon_2 = (WeaponItem *) item;
      break;
    case ITEM_SLOT_WEAPON_3:
      backup_weapon = (WeaponItem *) item;
      break;
    case ITEM_SLOT_BAG:
      bag = (ContainerItem *) item;
      break;
  }
}

GearItem * Gear::unequip(int32_t slot) {
  GearItem * old_item = nullptr;
  switch(slot) {
    case ITEM_SLOT_MANTLE:
      old_item = mantle;
      mantle = nullptr;
      return old_item;
    case ITEM_SLOT_HELMET:
      old_item = helmet;
      helmet = nullptr;
      return old_item;
    case ITEM_SLOT_GAUNTLETS:
      old_item = gauntlets;
      gauntlets = nullptr;
      return old_item;
    case ITEM_SLOT_ARMOR:
      old_item = armor;
      armor = nullptr;
      return old_item;
    case ITEM_SLOT_BOOTS:
      old_item = boots;
      boots = nullptr;
      return old_item;
    case ITEM_SLOT_LANTERN:
      old_item = lantern;
      lantern = nullptr;
      return old_item;
    case ITEM_SLOT_AMULET:
      old_item = amulet;
      amulet = nullptr;
      return old_item;
    case ITEM_SLOT_RING_1:
      old_item = ring_1;
      ring_1 = nullptr;
      return old_item;
    case ITEM_SLOT_RING_2:
      old_item = ring_2;
      ring_2 = nullptr;
      return old_item;
    case ITEM_SLOT_WEAPON_1:
      old_item = weapon_1;
      weapon_1 = nullptr;
      return old_item;
    case ITEM_SLOT_WEAPON_2:
      old_item = weapon_2;
      weapon_2 = nullptr;
      return old_item;
    case ITEM_SLOT_WEAPON_3:
      old_item = backup_weapon;
      backup_weapon = nullptr;
      return old_item;
    case ITEM_SLOT_BAG:
      old_item = bag;
      bag = nullptr;
      return old_item;
  }
  return old_item;
}

void Gear::swapWeapon(int32_t slot1, int32_t slot2) {
  WeaponItem * temp;
  if(slot1 == ITEM_SLOT_WEAPON_1) {
    if(slot2 == ITEM_SLOT_WEAPON_3) {
      temp = weapon_1;
      weapon_1 = backup_weapon;
      backup_weapon = temp;
    }
  }
  else if(slot1 == ITEM_SLOT_WEAPON_2) {
    if(slot2 == ITEM_SLOT_WEAPON_3) {
      temp = weapon_2;
      weapon_2 = backup_weapon;
      backup_weapon = temp;
    }
  }
}

Item * Gear::takeItem(ItemSlot * slot) {
  if(slot->slot == ITEM_SLOT_INSIDE_BAG) {
    return bag->remove(slot->x, slot->y);
  }
  else if(slot->slot == ITEM_SLOT_INSIDE_BASE_INVENTORY) {
    return base_inventory->remove(slot->x, slot->y);
  }
  else {
    return unequip(slot->slot);
  }
}

void Gear::putItem(ItemSlot * slot) {
  if(slot->slot == ITEM_SLOT_INSIDE_BAG) {
    bag->add(slot->item, slot->x, slot->y);
  }
  else if(slot->slot == ITEM_SLOT_INSIDE_BASE_INVENTORY) {
    base_inventory->add(slot->item, slot->x, slot->y);
  }
  else {
    equip((GearItem *) slot->item, slot->slot);
  }
}

void Gear::swapItem(ItemSlot * slot1, ItemSlot * slot2) {
  Item * temp = nullptr;
  switch(slot2->slot) {
    case ITEM_SLOT_MANTLE:
      temp = mantle;
      mantle = (ArmorItem *) takeItem(slot1);
      if(mantle->subtype != ITEM_MANTLE) {
        mantle = (ArmorItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_HELMET:
      temp = helmet;
      helmet = (ArmorItem *) takeItem(slot1);
      if(helmet->subtype != ITEM_HELMET) {
        helmet = (ArmorItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_ARMOR:
      temp = armor;
      armor = (ArmorItem *) takeItem(slot1);
      if(armor->subtype != ITEM_CUIRASS) {
        armor = (ArmorItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_GAUNTLETS:
      temp = gauntlets;
      gauntlets = (ArmorItem *) takeItem(slot1);
      if(gauntlets->subtype != ITEM_GAUNTLETS) {
        gauntlets = (ArmorItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_BOOTS:
      temp = boots;
      boots = (ArmorItem *) takeItem(slot1);
      if(boots->subtype != ITEM_BOOTS) {
        boots = (ArmorItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_LANTERN:
      temp = lantern;
      lantern = (ArmorItem *) takeItem(slot1);
      if(lantern->subtype != ITEM_LANTERN) {
        lantern = (ArmorItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_AMULET:
      temp = amulet;
      amulet = (ArmorItem *) takeItem(slot1);
      if(amulet->subtype != ITEM_AMULET) {
        amulet = (ArmorItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_RING_1:
      temp = ring_1;
      ring_1 = (ArmorItem *) takeItem(slot1);
      if(ring_1->subtype != ITEM_RING) {
        ring_1 = (ArmorItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_RING_2:
      temp = ring_2;
      ring_2 = (ArmorItem *) takeItem(slot1);
      if(ring_2->subtype != ITEM_RING) {
        ring_2 = (ArmorItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_WEAPON_1:
      temp = weapon_1;
      weapon_1 = (WeaponItem *) takeItem(slot1);
      if(weapon_1->subtype != ITEM_WEAPON) {
        weapon_1 = (WeaponItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_WEAPON_2:
      temp = weapon_2;
      weapon_2 = (WeaponItem *) takeItem(slot1);
      if(weapon_2->subtype != ITEM_WEAPON) {
        weapon_2 = (WeaponItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_WEAPON_3:
      temp = backup_weapon;
      backup_weapon = (WeaponItem *) takeItem(slot1);
      if(backup_weapon->subtype != ITEM_WEAPON) {
        backup_weapon = (WeaponItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_BAG:
      temp = bag;
      bag = (ContainerItem *) takeItem(slot1);
      if(bag->subtype != ITEM_BAG) {
        bag = (ContainerItem *) temp;
        return;
      }
      break;
    case ITEM_SLOT_INSIDE_BASE_INVENTORY:
      temp = base_inventory->remove(slot2->x, slot2->y);
      base_inventory->add(takeItem(slot1), slot2->x, slot2->y);
      break;
    default: ;
    case ITEM_SLOT_INSIDE_BAG:
      temp = bag->remove(slot2->x, slot2->y);
      bag->add(takeItem(slot1), slot2->x, slot2->y);
      break;
  }
  if(temp != nullptr) {
    slot1->item = temp;
    putItem(slot1);
  }
}

float Gear::getDamageReductionFromType(int32_t damage_type) {
  float resistance = 0.;
  if(mantle != nullptr) {
    resistance += mantle->getDamageReductionFromType(damage_type);
  }
  if(helmet != nullptr) {
    resistance += helmet->getDamageReductionFromType(damage_type);
  }
  if(gauntlets != nullptr) {
    resistance += gauntlets->getDamageReductionFromType(damage_type);
  }
  if(boots != nullptr) {
    resistance += boots->getDamageReductionFromType(damage_type);
  }
  if(armor != nullptr) {
    resistance += armor->getDamageReductionFromType(damage_type);
  }
  if(lantern != nullptr) {
    resistance += lantern->getDamageReductionFromType(damage_type);
  }
  if(ring_1 != nullptr) {
    resistance += ring_1->getDamageReductionFromType(damage_type);
  }
  if(ring_2 != nullptr) {
    resistance += ring_2->getDamageReductionFromType(damage_type);
  }
  if(amulet != nullptr) {
    resistance += amulet->getDamageReductionFromType(damage_type);
  }
  return resistance;
}

int32_t Gear::getWeaponsCombination() {
  if(weapon_1 == nullptr && weapon_2 == nullptr) {
    return ITEM_HAND;
  }
  else if(weapon_2 == nullptr) {
    return weapon_1->subtype;
  }
  else if(weapon_1 == nullptr) {
    return weapon_2->subtype;
  }
  else {
    return weapon_1->subtype * weapon_2->subtype;
  }
}

ArmorItem * Gear::getMantle() { return mantle; }
ArmorItem * Gear::getHelmet() { return helmet; }
ArmorItem * Gear::getArmor() { return armor; }
ArmorItem * Gear::getGauntlets() { return gauntlets; }
ArmorItem * Gear::getBoots() { return boots; }
ArmorItem * Gear::getLantern() { return lantern; }
ArmorItem * Gear::getAmulet() { return amulet; }
ArmorItem * Gear::getRing_1() { return ring_1; }
ArmorItem * Gear::getRing_2() { return ring_2; }
WeaponItem * Gear::getWeapon_1() { return weapon_1; }
WeaponItem * Gear::getWeapon_2() { return weapon_2; }
WeaponItem * Gear::getBackupWeapon() { return backup_weapon; }
ContainerItem * Gear::getBaseInventory() { return base_inventory; }
ContainerItem * Gear::getBag() { return bag; }

float Gear::getWeight() {
  float weight = 0.F;
  if(mantle != nullptr) {
    weight += mantle->getWeight();
  }
  if(helmet != nullptr) {
    weight += helmet->getWeight();
  }
  if(gauntlets != nullptr) {
    weight += gauntlets->getWeight();
  }
  if(boots != nullptr) {
    weight += boots->getWeight();
  }
  if(armor != nullptr) {
    weight += armor->getWeight();
  }
  if(lantern != nullptr) {
    weight += lantern->getWeight();
  }
  if(ring_1 != nullptr) {
    weight += ring_1->getWeight();
  }
  if(ring_2 != nullptr) {
    weight += ring_2->getWeight();
  }
  if(amulet != nullptr) {
    weight += amulet->getWeight();
  }
  if(weapon_1 != nullptr) {
    weight += weapon_1->getWeight();
  }
  if(weapon_2 != nullptr) {
    weight += weapon_2->getWeight();
  }
  if(backup_weapon != nullptr) {
    weight += backup_weapon->getWeight();
  }
  if(bag != nullptr) {
    weight += bag->getWeight();
  }
  if(base_inventory != nullptr) {
    weight += base_inventory->getWeight();
  }
  return weight;
}

std::list<Effect *> Gear::getEffects() {
  std::list<Effect *> effects = std::list<Effect *>();
  if(mantle != nullptr) {
    for(Effect * effect : mantle->effects) {
      effects.push_back(effect);
    }
  }
  if(helmet != nullptr) {
    for(Effect * effect : helmet->effects) {
      effects.push_back(effect);
    }
  }
  if(gauntlets != nullptr) {
    for(Effect * effect : gauntlets->effects) {
      effects.push_back(effect);
    }
  }
  if(boots != nullptr) {
    for(Effect * effect : boots->effects) {
      effects.push_back(effect);
    }
  }
  if(armor != nullptr) {
    for(Effect * effect : armor->effects) {
      effects.push_back(effect);
    }
  }
  if(lantern != nullptr) {
    for(Effect * effect : lantern->effects) {
      effects.push_back(effect);
    }
  }
  if(ring_1 != nullptr) {
    for(Effect * effect : ring_1->effects) {
      effects.push_back(effect);
    }
  }
  if(ring_2 != nullptr) {
    for(Effect * effect : ring_2->effects) {
      effects.push_back(effect);
    }
  }
  if(amulet != nullptr) {
    for(Effect * effect : amulet->effects) {
      effects.push_back(effect);
    }
  }
  if(weapon_1 != nullptr) {
    for(Effect * effect : weapon_1->effects) {
      effects.push_back(effect);
    }
  }
  if(weapon_2 != nullptr) {
    for(Effect * effect : weapon_2->effects) {
      effects.push_back(effect);
    }
  }
  if(bag != nullptr) {
    for(Effect * effect : bag->effects) {
      effects.push_back(effect);
    }
  }
  if(base_inventory != nullptr) {
    for(Effect * effect : base_inventory->effects) {
      effects.push_back(effect);
    }
  }
  return effects;
}

void Gear::useItem(int32_t x, int32_t y, int32_t slot, Character * user) {
  Item * item;
  switch(slot) {
    case ITEM_SLOT_INSIDE_BASE_INVENTORY:
      item = base_inventory->remove(x, y);
      if(!item->usable) {
        base_inventory->add(item, x, y);
      }
      else {
        for(Effect * effect : item->effects) {
          effect->activate(user);
        }
        delete item;
      }
    case ITEM_SLOT_INSIDE_BAG:
      item = bag->remove(x, y);
      if(!item->usable) {
        bag->add(item, x, y);
      }
      else {
        for(Effect * effect : item->effects) {
          effect->activate(user);
        }
        delete item;
      }
      break;
    default: ;
  }
}

void Gear::reload(ItemSlot * ammo, int32_t slot_weapon) {

}


std::list<ItemSlot *> Gear::getItems() {
  std::list<ItemSlot *> items = std::list<ItemSlot *>();
  if(mantle != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_MANTLE;
    slot->item = mantle;
    items.push_back(slot);
  }
  if(helmet != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_HELMET;
    slot->item = helmet;
    items.push_back(slot);
  }
  if(gauntlets != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_GAUNTLETS;
    slot->item = gauntlets;
    items.push_back(slot);
  }
  if(boots != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_BOOTS;
    slot->item = boots;
    items.push_back(slot);
  }
  if(armor != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_ARMOR;
    slot->item = armor;
    items.push_back(slot);
  }
  if(lantern != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_LANTERN;
    slot->item = lantern;
    items.push_back(slot);
  }
  if(ring_1 != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_RING_1;
    slot->item = ring_1;
    items.push_back(slot);
  }
  if(ring_2 != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_RING_2;
    slot->item = ring_2;
    items.push_back(slot);
  }
  if(amulet != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_AMULET;
    slot->item = amulet;
    items.push_back(slot);
  }
  if(weapon_1 != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_WEAPON_1;
    slot->item = weapon_1;
    items.push_back(slot);
  }
  if(weapon_2 != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_WEAPON_2;
    slot->item = weapon_2;
    items.push_back(slot);
  }
  if(backup_weapon != nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_WEAPON_3;
    slot->item = backup_weapon;
    items.push_back(slot);
  }
  if(base_inventory != nullptr) {
    for(ItemSlot * slot : base_inventory->getItems()) {
      items.push_back(slot);
    }
  }
  if(bag !=nullptr) {
    ItemSlot * slot = new ItemSlot();
    slot->x = 0;
    slot->y = 0;
    slot->slot = ITEM_SLOT_BAG;
    slot->item = bag;
    items.push_back(slot);
    for(ItemSlot * slot : bag->getItems()) {
      items.push_back(slot);
    }
  }
  return items;
}

std::string Gear::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  if(mantle != nullptr) {
    String::insert(ss, mantle->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(helmet != nullptr) {
    String::insert(ss, helmet->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(armor != nullptr) {
    String::insert(ss, armor->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(gauntlets != nullptr) {
    String::insert(ss, gauntlets->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(boots != nullptr) {
    String::insert(ss, boots->to_string());
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
  if(amulet != nullptr) {
    String::insert(ss, amulet->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(ring_1 != nullptr) {
    String::insert(ss, ring_1->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(ring_2 != nullptr) {
    String::insert(ss, ring_2->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(weapon_1 != nullptr) {
    String::insert(ss, weapon_1->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(weapon_2 != nullptr) {
    String::insert(ss, weapon_2->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(backup_weapon != nullptr) {
    String::insert(ss, backup_weapon->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(base_inventory != nullptr) {
    String::insert(ss, base_inventory->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  if(bag != nullptr) {
    String::insert(ss, bag->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

Gear * Gear::from_string(std::string to_read, Adventure * adventure) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  ArmorItem * mantle = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * helmet = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * armor = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * gauntlets = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * boots = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * lantern = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * amulet = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * ring_1 = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  ArmorItem * ring_2 = (ArmorItem *) Item::from_string(String::extract(ss), adventure);
  WeaponItem * weapon_1 = (WeaponItem *) Item::from_string(String::extract(ss), adventure);
  WeaponItem * weapon_2 = (WeaponItem *) Item::from_string(String::extract(ss), adventure);
  WeaponItem * backup_weapon = (WeaponItem *) Item::from_string(String::extract(ss), adventure);
  ContainerItem * base_inventory = (ContainerItem *) Item::from_string(String::extract(ss), adventure);
  ContainerItem * bag = (ContainerItem *) Item::from_string(String::extract(ss), adventure);
  delete ss;
  Gear * result = new Gear(
    name,
    mantle,
    helmet,
    armor,
    gauntlets,
    boots,
    amulet,
    lantern,
    ring_1,
    ring_2,
    weapon_1,
    weapon_2,
    backup_weapon,
    base_inventory,
    bag,
    std::list<Item *>()
  );
  return result;
}
