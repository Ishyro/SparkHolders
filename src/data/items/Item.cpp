#include <sstream>

#include "data/Adventure.h"
#include "data/Database.h"
#include "data/Effect.h"

#include "data/items/Item.h"
#include "data/items/BasicItem.h"
#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/SerialItem.h"
#include "data/items/AmmunitionItem.h"
#include "data/items/ContainerItem.h"

#include "util/String.h"

Item * Item::init(const Item * item, int32_t tier, int32_t number) {
    switch(item->type) {
      case ITEM_BASIC:
        return new BasicItem( (BasicItem *) item, tier);
      case ITEM_ARMOR:
        return new ArmorItem( (ArmorItem *) item, tier);
      case ITEM_WEAPON:
        return new WeaponItem( (WeaponItem *) item, tier);
      case ITEM_SERIAL:
        return new SerialItem( (SerialItem *) item, tier, number);
      case ITEM_AMMUNITION:
        return new AmmunitionItem( (AmmunitionItem *) item, tier, number);
      case ITEM_CONTAINER:
        return new ContainerItem( (ContainerItem *) item, tier);
      default:
        return nullptr;
    }
}

bool Item::isFood() {
  for(Effect * effect : effects) {
    if(effect->type == EFFECT_HUNGER) {
      return true;
    }
  }
  return false;
}

Item * Item::from_string(std::string to_read, Adventure * adventure) {
  if(to_read == "none") {
    return nullptr;
  }
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int64_t id = String::extract_long(ss);
  int32_t type = String::extract_int(ss);
  int32_t subtype = String::extract_int(ss);
  int32_t tier = String::extract_int(ss);
  int32_t max_tier = String::extract_int(ss);
  float weight = String::extract_float(ss);
  int32_t sizeX = String::extract_int(ss);
  int32_t sizeY = String::extract_int(ss);
  int32_t gold_value = String::extract_int(ss);
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
    case ITEM_BASIC: {
      result = new BasicItem(
        name,
        id,
        type,
        subtype,
        tier,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects
      );
      break;
    }
    case ITEM_WEAPON: {
      int32_t swap_time = String::extract_int(ss);
      int32_t max_durability = String::extract_int(ss);
      int32_t durability = String::extract_int(ss);
      float rangeX = String::extract_float(ss);
      float rangeY = String::extract_float(ss);
      float rangeZ = String::extract_float(ss);
      int32_t strike_time = String::extract_int(ss);
      float status_power = String::extract_float(ss);
      bool use_projectile = String::extract_bool(ss);
      bool use_ammo = String::extract_bool(ss);
      int32_t ammo_type = String::extract_int(ss);
      int32_t capacity = String::extract_int(ss);
      int32_t reload_time = String::extract_int(ss);
      AmmunitionItem * ammo = nullptr;
      std::string ammo_str = String::extract(ss);
      if(ammo_str != "none") {
        ammo = (AmmunitionItem *) from_string(ammo_str, adventure);
      }
      std::array<float, DAMAGE_TYPE_NUMBER> damages;
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages[i] = String::extract_float(ss);
      }
      result = new WeaponItem(
        name,
        id,
        type,
        subtype,
        tier,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        swap_time,
        max_durability,
        durability,
        rangeX,
        rangeY,
        rangeZ,
        strike_time,
        status_power,
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
      int32_t swap_time = String::extract_int(ss);
      int32_t max_durability = String::extract_int(ss);
      int32_t durability = String::extract_int(ss);
      std::array<float, DAMAGE_TYPE_NUMBER> damages;
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages[i] = String::extract_float(ss);
      }
      result = new ArmorItem(
        name,
        id,
        type,
        subtype,
        tier,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        swap_time,
        max_durability,
        durability,
        damages
      );
      break;
    }
    case ITEM_SERIAL: {
      int32_t number = String::extract_int(ss);
      int32_t max = String::extract_int(ss);
      result = new SerialItem(
        name,
        id,
        type,
        subtype,
        tier,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        number,
        max
      );
      break;
    }
    case ITEM_AMMUNITION: {
      int32_t number = String::extract_int(ss);
      int32_t max = String::extract_int(ss);
      Projectile * projectile = (Projectile *) adventure->getDatabase()->getProjectile(String::extract(ss));
      result = new AmmunitionItem(
        name,
        id,
        type,
        subtype,
        tier,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        number,
        max,
        projectile
      );
      break;
    }
    case ITEM_CONTAINER: {
      int32_t swap_time = String::extract_int(ss);
      int32_t max_durability = String::extract_int(ss);
      int32_t durability = String::extract_int(ss);
      bool can_take_from = String::extract_bool(ss);
      bool repercute_weight = String::extract_bool(ss);
      bool limited = String::extract_bool(ss);
      int32_t limit_type = String::extract_int(ss);
      int32_t contentX = String::extract_int(ss);
      int32_t contentY = String::extract_int(ss);
      result = new ContainerItem(
        name,
        id,
        type,
        subtype,
        tier,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        swap_time,
        max_durability,
        durability,
        can_take_from,
        repercute_weight,
        limited,
        limit_type,
        contentX,
        contentY
      );
      std::stringstream * ss_items = new std::stringstream(String::extract(ss));
      while(ss_items->rdbuf()->in_avail() != 0) {
        int32_t x = String::extract_int(ss_items);
        int32_t y = String::extract_int(ss_items);
        Item * item = Item::from_string(String::extract(ss_items), adventure);
        ( (ContainerItem *) result)->add(item, x, y);
      }
      delete ss_items;
      break;
    }
    default:
      return nullptr;
  }
  delete ss;
  delete effects;
  return result;
}
