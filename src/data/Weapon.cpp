#include <algorithm>
#include <sstream>

#include "data/Effect.h"
#include "data/Projectile.h"

#include "data/Weapon.h"

#include "util/String.h"

int Weapon::getRawDamage() {
  int power = 0;
  if(ammo != nullptr) {
    power += ammo->projectile->getRawDamage();
  }
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    if(damage_type == NEUTRAL_DAMAGE) {
      power += 3 * damages[damage_type];
    }
    else if(damage_type == MIND_DAMAGE) {
      power += 5 * damages[damage_type];
    }
    else if(damage_type == TRUE_DAMAGE) {
      power += 10 * damages[damage_type];
    }
    else if(damage_type == SOUL_DAMAGE) {
      power += 10 * damages[damage_type];
    }
    else {
      power += damages[damage_type];
    }
  }
  return power;
}

int Weapon::getDamageFromType(int damage_type) {
  int damage = damages[damage_type];
  if(ammo != nullptr) {
    damage += ammo->projectile->getDamageFromType(damage_type);
  }
  for(Effect * e : effects) {
    if(e->type == DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type);
    }
  }
  return damage;
}

int Weapon::getCurrentCapacity() {
  if(this->ammo != nullptr) {
    return ammo->number;
  }
  else {
    return 0;
  }
}

Ammunition * Weapon::getAmmo() { return ammo; }

void Weapon::useAmmo() {
  if(this->ammo != nullptr && --this->ammo->number == 0) {
    delete this->ammo;
    this->ammo = nullptr;
  }
}

Ammunition * Weapon::reload(Ammunition * ammo) {
  Ammunition * to_return = nullptr;
  if(this->ammo == nullptr && ammo_type == ammo->ammo_type) {
    this->ammo = new Ammunition();
    this->ammo->projectile = ammo->projectile;
    this->ammo->gold_value = ammo->gold_value;
    this->ammo->ammo_type = ammo->ammo_type;
    int to_load = std::min(capacity, ammo->number);
    ammo->number -= to_load;
    this->ammo->number = to_load;
  }
  else if(this->ammo != nullptr && this->ammo->projectile->name == ammo->projectile->name) {
    int to_load = std::min(capacity - this->ammo->number, ammo->number);
    ammo->number -= to_load;
    this->ammo->number += to_load;
  }
  else if(this->ammo != nullptr && this->ammo->ammo_type == ammo->ammo_type) {
    to_return = new Ammunition();
    to_return->projectile = this->ammo->projectile;
    to_return->gold_value = this->ammo->gold_value;
    to_return->ammo_type = this->ammo->ammo_type;
    to_return->number = this->ammo->number;

    this->ammo->projectile = ammo->projectile;
    this->ammo->gold_value = ammo->gold_value;
    this->ammo->ammo_type = ammo->ammo_type;
    int to_load = std::min(capacity, ammo->number);
    ammo->number -= to_load;
    this->ammo->number = to_load;
  }
  return to_return;
}

std::string Weapon::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_bool(ss, melee);
  String::insert_float(ss, range);
  String::insert_int(ss, type);
  String::insert_bool(ss, droppable);
  String::insert_float(ss, weight);
  String::insert_int(ss, armor);
  String::insert_int(ss, gold_value);
  String::insert_bool(ss, use_ammo);
  String::insert_int(ss, ammo_type);
  String::insert_int(ss, capacity);
  String::insert_int(ss, strike_time);
  String::insert_int(ss, reload_time);
  String::insert_int(ss, swap_time);
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_int(ss, damages[i]);
  }
  String::insert(ss, Projectile::ammo_to_string(ammo));
  std::string result = ss->str();
  delete ss;
  return result;
}

Weapon * Weapon::from_string(std::string to_read) {
  if(to_read == "none") {
    return nullptr;
  }
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  bool melee = String::extract_bool(ss);
  float range = String::extract_float(ss);
  int type = String::extract_int(ss);
  bool droppable = String::extract_bool(ss);
  float weight = String::extract_float(ss);
  int armor = String::extract_int(ss);
  int gold_value = String::extract_int(ss);
  bool use_ammo = String::extract_bool(ss);
  int ammo_type = String::extract_int(ss);
  int capacity = String::extract_int(ss);
  int strike_time = String::extract_int(ss);
  int reload_time = String::extract_int(ss);
  int swap_time = String::extract_int(ss);
  std::stringstream * ss_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * effects = new std::list<Effect *>();
  while(ss_effects->rdbuf()->in_avail() != 0) {
    effects->push_back(Effect::from_string(String::extract(ss_effects)));
  }
  delete ss_effects;
  int damages[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] = String::extract_int(ss);
  }
  Ammunition * ammo = Projectile::ammo_from_string(String::extract(ss));
  delete ss;
  Weapon * result = new Weapon(
    name,
    melee,
    range,
    type,
    droppable,
    weight,
    armor,
    gold_value,
    use_ammo,
    ammo_type,
    capacity,
    strike_time,
    reload_time,
    swap_time,
    *effects,
    damages,
    ammo
  );
  delete effects;
  return result;
}
