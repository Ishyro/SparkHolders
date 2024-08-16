#ifndef _GEAR_H_
#define _GEAR_H_

#include <list>

#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/ContainerItem.h"
#include "data/items/SerialItem.h"
#include "Values.h"

class Gear {
  public:
    const std::string name;
    Gear(
      std::string name,
      ArmorItem * mantle,
      ArmorItem * helmet,
      ArmorItem * armor,
      ArmorItem * gauntlets,
      ArmorItem * boots,
      ArmorItem * lantern,
      ArmorItem * amulet,
      ArmorItem * ring_1,
      ArmorItem * ring_2,
      WeaponItem * weapon_1,
      WeaponItem * weapon_2,
      WeaponItem * backup_weapon_1,
      WeaponItem * backup_weapon_2,
      ContainerItem * bag,
      ContainerItem * belt,
      std::list<Item *> items
    ):
      name(name),
      mantle(mantle),
      helmet(helmet),
      armor(armor),
      gauntlets(gauntlets),
      boots(boots),
      lantern(lantern),
      amulet(amulet),
      ring_1(ring_1),
      ring_2(ring_2),
      weapon_1(weapon_1),
      weapon_2(weapon_2),
      backup_weapon_1(backup_weapon_1),
      backup_weapon_2(backup_weapon_2),
      bag(bag),
      belt(belt),
      items(items)
    {}
    Gear(const Gear * gear):
      name(gear->name),
      mantle(nullptr),
      helmet(nullptr),
      armor(nullptr),
      gauntlets(nullptr),
      boots(nullptr),
      lantern(nullptr),
      amulet(nullptr),
      ring_1(nullptr),
      ring_2(nullptr),
      weapon_1(nullptr),
      weapon_2(nullptr),
      backup_weapon_1(nullptr),
      backup_weapon_2(nullptr),
      bag(nullptr),
      belt(nullptr),
      items(std::list<Item *>())
    {
      if(gear->mantle != nullptr) {
        mantle = new ArmorItem(gear->mantle, gear->mantle->tier);
      }
      if(gear->helmet != nullptr) {
        helmet = new ArmorItem(gear->helmet, gear->helmet->tier);
      }
      if(gear->armor != nullptr) {
        armor = new ArmorItem(gear->armor, gear->armor->tier);
      }
      if(gear->gauntlets != nullptr) {
        gauntlets = new ArmorItem(gear->gauntlets, gear->gauntlets->tier);
      }
      if(gear->boots != nullptr) {
        boots = new ArmorItem(gear->boots, gear->boots->tier);
      }
      if(gear->lantern != nullptr) {
        lantern = new ArmorItem(gear->lantern, gear->lantern->tier);
      }
      if(gear->amulet != nullptr) {
        amulet = new ArmorItem(gear->amulet, gear->amulet->tier);
      }
      if(gear->ring_1 != nullptr) {
        ring_1 = new ArmorItem(gear->ring_1, gear->ring_1->tier);
      }
      if(gear->ring_2 != nullptr) {
        ring_2 = new ArmorItem(gear->ring_2, gear->ring_2->tier);
      }
      if(gear->weapon_1 != nullptr) {
        weapon_1 = new WeaponItem(gear->weapon_1, gear->weapon_1->tier);
      }
      if(gear->weapon_2 != nullptr) {
        weapon_2 = new WeaponItem(gear->weapon_2, gear->weapon_2->tier);
      }
      if(gear->backup_weapon_1 != nullptr) {
        backup_weapon_1 = new WeaponItem(gear->backup_weapon_1, gear->backup_weapon_1->tier);
      }
      if(gear->backup_weapon_2 != nullptr) {
        backup_weapon_2 = new WeaponItem(gear->backup_weapon_2, gear->backup_weapon_2->tier);
      }
      if(gear->bag != nullptr) {
        bag = new ContainerItem(gear->bag, gear->bag->tier);
      }
      if(gear->belt != nullptr) {
        belt = new ContainerItem(gear->belt, gear->belt->tier);
      }
      std::list<Item *> toadds = std::list<Item *>();
      for(Item * item : gear->items) {
        Item * toadd;
        if(item->type == ITEM_SERIAL || item->type == ITEM_AMMUNITION) {
          toadd = Item::init(item, item->tier, ((SerialItem *) item)->getNumber());
        }
        else {
          toadd = Item::init(item, item->tier, 1);
        }
        toadds.push_back(toadd);
      }
      if(!toadds.empty()) {
        belt->add_all(toadds);
      }
    }
    Gear():
      name(""),
      mantle(nullptr),
      helmet(nullptr),
      armor(nullptr),
      gauntlets(nullptr),
      boots(nullptr),
      lantern(nullptr),
      amulet(nullptr),
      ring_1(nullptr),
      ring_2(nullptr),
      weapon_1(nullptr),
      weapon_2(nullptr),
      backup_weapon_1(nullptr),
      backup_weapon_2(nullptr),
      bag(nullptr),
      belt(nullptr),
      items(std::list<Item *>())
    {}
    void equip(GearItem * new_item, int32_t slot);
    GearItem * unequip(int32_t slot);
    void swapWeapon(int32_t slot1, int32_t slot2);
    Item * takeItem(ItemSlot * slot);
    void putItem(ItemSlot * slot);
    void swapItem(ItemSlot * slot1, ItemSlot * slot2);

    float getDamageReductionFromType(int32_t damage_type);

    int32_t getWeaponsCombination();
    ArmorItem * getMantle();
    ArmorItem * getHelmet();
    ArmorItem * getArmor();
    ArmorItem * getGauntlets();
    ArmorItem * getBoots();
    ArmorItem * getLantern();
    ArmorItem * getAmulet();
    ArmorItem * getRing_1();
    ArmorItem * getRing_2();
    WeaponItem * getWeapon_1();
    WeaponItem * getWeapon_2();
    WeaponItem * getBackupWeapon_1();
    WeaponItem * getBackupWeapon_2();
    ContainerItem * getBag();
    ContainerItem * getBelt();
    float getWeight();
    void useItem(int32_t x, int32_t y, int32_t slot, Character * user);
    void reload(ItemSlot * ammo, int32_t slot_weapon);
    std::string to_string();
    std::list<ItemSlot *> getItems();
    static Gear * from_string(std::string to_read, Adventure * adventure);
    bool operator == (const Gear& g) const { return name == g.name; }
    bool operator != (const Gear& g) const { return !operator==(g); }
  private:
    ArmorItem * mantle;
    ArmorItem * helmet;
    ArmorItem * armor;
    ArmorItem * gauntlets;
    ArmorItem * boots;
    ArmorItem * lantern;
    ArmorItem * amulet;
    ArmorItem * ring_1;
    ArmorItem * ring_2;
    WeaponItem * weapon_1;
    WeaponItem * weapon_2;
    WeaponItem * backup_weapon_1;
    WeaponItem * backup_weapon_2;
    ContainerItem * bag;
    ContainerItem * belt;
    std::list<Item *> items;
};

#endif // _GEAR_H_
