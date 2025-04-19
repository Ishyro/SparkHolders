#include <algorithm>
#include <sstream>

#include "data/Effect.h"
#include "data/Projectile.h"

#include "data/items/AmmunitionItem.h"

#include "data/items/WeaponItem.h"

#include "util/String.h"

float WeaponItem::getRawDamage() {
  float power = 0;
  if(ammo != nullptr) {
    power += ammo->getProjectile()->getRawDamage();
  }
  for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    if(damage_type == DAMAGE_ACID) {
      power += 3 * damages[damage_type];
    }
    else if(damage_type == DAMAGE_MIND) {
      power += 5 * damages[damage_type];
    }
    else if(damage_type == DAMAGE_SOLAR) {
      power += 10 * damages[damage_type];
    }
    else if(damage_type == DAMAGE_AETHER) {
      power += 10 * damages[damage_type];
    }
    else {
      power += damages[damage_type];
    }
  }
  return power;
}

float WeaponItem::getDamageFromType(int32_t damage_type) {
  float damage = damages[damage_type];
  if(ammo != nullptr) {
    damage += ammo->getProjectile()->getDamageFromType(damage_type);
  }
  return damage;
}

int32_t WeaponItem::getCurrentCapacity() {
  if(this->ammo != nullptr) {
    return ammo->getNumber();
  }
  else {
    return 0;
  }
}

AmmunitionItem * WeaponItem::getAmmo() { return ammo; }

void WeaponItem::useAmmo() {
  if(ammo != nullptr && ammo->reduce(1) == 0) {
    delete ammo;
    ammo = nullptr;
  }
}

AmmunitionItem * WeaponItem::reload(AmmunitionItem * ammo) {
  AmmunitionItem * to_return = nullptr;
  if(this->ammo == nullptr && ammo_type == ammo->subtype) {
    int32_t to_load = std::min(capacity, ammo->getNumber());
    ammo->reduce(to_load);
    this->ammo = new AmmunitionItem(ammo, ammo->tier, to_load);
  }
  else if(this->ammo != nullptr && this->ammo->name == ammo->name && this->ammo->tier == ammo->tier) {
    int32_t to_load = std::min(capacity, ammo->getNumber());
    ammo->reduce(to_load);
    this->ammo->add(to_load);
  }
  else if(this->ammo != nullptr && ammo_type == ammo->subtype) {
    to_return = this->ammo;
    int32_t to_load = std::min(capacity, ammo->getNumber());
    ammo->reduce(to_load);
    this->ammo = new AmmunitionItem(ammo, ammo->tier, to_load);
  }
  return to_return;
}

std::string WeaponItem::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, type);
  String::insert_int(ss, subtype);
  String::insert_int(ss, tier);
  String::insert_int(ss, max_tier);
  String::insert_float(ss, weight);
  String::insert_int(ss, sizeX);
  String::insert_int(ss, sizeY);
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
  String::insert_int(ss, swap_time);
  String::insert_int(ss, max_durability);
  String::insert_int(ss, durability);
  String::insert_float(ss, range.x);
  String::insert_float(ss, range.y);
  String::insert_float(ss, range.z);
  String::insert_int(ss, strike_time);
  String::insert_float(ss, status_power);
  String::insert_float(ss, pierce_power);
  String::insert_bool(ss, use_projectile);
  String::insert_bool(ss, use_ammo);
  String::insert_int(ss, ammo_type);
  String::insert_int(ss, capacity);
  String::insert_int(ss, reload_time);
  if(ammo != nullptr) {
    String::insert(ss, ammo->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_float(ss, damages[i]);
  }
  std::string result = ss->str();
  delete ss;
  return result;
}
