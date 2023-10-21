#include <sstream>

#include "data/Adventure.h"
#include "data/Database.h"
#include "data/Effect.h"

#include "data/items/Item.h"
#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/SerialItem.h"
#include "data/items/AmmunitionItem.h"

#include "util/String.h"

Item * Item::init(const Item * item, int tier, int number) {
    switch(item->type) {
      case ITEM_WEAPON:
        return new WeaponItem( (WeaponItem *) item, tier);
      case ITEM_ARMOR:
        return new ArmorItem( (ArmorItem *) item, tier);
      case ITEM_MISCELLANEOUS:
      case ITEM_CONSUMABLE:
        return new SerialItem( (SerialItem *) item, tier, number);
      case ITEM_AMMUNITION:
        return new AmmunitionItem( (AmmunitionItem *) item, tier, number);
      default:
        return nullptr;
    }
}

std::string Item::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, type);
  String::insert_int(ss, type2);
  String::insert_int(ss, tier);
  String::insert_int(ss, max_tier);
  String::insert_float(ss, weight);
  String::insert_int(ss, gold_value);
  String::insert_bool(ss, droppable);
  String::insert_bool(ss, usable);
  String::insert_bool(ss, consumable);
  String::insert_int(ss, use_time);
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  std::string result = ss->str();
  delete ss;
  return result;
}

Item * Item::from_string(std::string to_read, Adventure * adventure) {
  if(to_read == "none") {
    return nullptr;
  }
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  long id = String::extract_long(ss);
  int type = String::extract_int(ss);
  int type2 = String::extract_int(ss);
  int tier = String::extract_int(ss);
  int max_tier = String::extract_int(ss);
  float weight = String::extract_float(ss);
  int gold_value = String::extract_int(ss);
  bool droppable = String::extract_bool(ss);
  bool usable = String::extract_bool(ss);
  bool consumable = String::extract_bool(ss);
  float use_time = String::extract_int(ss);
  std::stringstream * ss_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * effects = new std::list<Effect *>();
  while(ss_effects->rdbuf()->in_avail() != 0) {
    effects->push_back(Effect::from_string(String::extract(ss_effects)));
  }
  delete ss_effects;
  Item * result;
  switch(type) {
    case ITEM_WEAPON: {
      int swap_time = String::extract_int(ss);
      float range = String::extract_float(ss);
      int strike_time = String::extract_int(ss);
      bool use_projectile = String::extract_bool(ss);
      bool use_ammo = String::extract_bool(ss);
      int ammo_type = String::extract_int(ss);
      int capacity = String::extract_int(ss);
      int reload_time = String::extract_int(ss);
      AmmunitionItem * ammo = nullptr;
      std::string ammo_str = String::extract(ss);
      if(ammo_str != "none") {
        ammo = (AmmunitionItem *) from_string(ammo_str, adventure);
      }
      int damages[DAMAGE_TYPE_NUMBER];
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages[i] = String::extract_int(ss);
      }
      result = new WeaponItem(
        name,
        id,
        type,
        type2,
        tier,
        max_tier,
        weight,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        swap_time,
        range,
        strike_time,
        use_projectile,
        use_ammo,
        ammo_type,
        capacity,
        reload_time,
        ammo,
        damages
      );
      break;
    }
    case ITEM_ARMOR: {
      int swap_time = String::extract_int(ss);
      float damage_reductions[DAMAGE_TYPE_NUMBER];
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damage_reductions[i] = String::extract_float(ss);
      }
      result = new ArmorItem(
        name,
        id,
        type,
        type2,
        tier,
        max_tier,
        weight,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        swap_time,
        damage_reductions
      );
      break;
    }
    case ITEM_MISCELLANEOUS:
    case ITEM_CONSUMABLE: {
      int number = String::extract_int(ss);
      result = new SerialItem(
        name,
        id,
        type,
        type2,
        tier,
        max_tier,
        weight,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        number
      );
      break;
    }
    case ITEM_AMMUNITION: {
      int number = String::extract_int(ss);
      Projectile * projectile = (Projectile *) adventure->getDatabase()->getProjectile(String::extract(ss));
      result = new AmmunitionItem(
        name,
        id,
        type,
        type2,
        tier,
        max_tier,
        weight,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        number,
        projectile
      );
      break;
    }
    default:
      return nullptr;
  }
  delete ss;
  delete effects;
  return result;
}
