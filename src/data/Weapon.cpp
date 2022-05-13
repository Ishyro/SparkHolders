#include <algorithm>
#include <sstream>

#include "data/Effect.h"
#include "data/Projectile.h"

#include "data/Weapon.h"

int Weapon::getRawDamage() {
  int power = 0;
  if(ammo != nullptr) {
    power += ammo->projectile->getRawDamage();
  }
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += damages[i];
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
  } else {
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
  } else if(this->ammo != nullptr && this->ammo->projectile->name == ammo->projectile->name) {
    int to_load = std::min(capacity - this->ammo->number, ammo->number);
    ammo->number -= to_load;
    this->ammo->number += to_load;
  } else if(this->ammo != nullptr && this->ammo->ammo_type == ammo->ammo_type) {
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
  std::string msg = name + ";";
  msg += std::to_string(melee) + ";";
  msg += std::to_string(range) + ";";
  msg += std::to_string(type) + ";";
  msg += std::to_string(weight) + ";";
  msg += std::to_string(gold_value) + ";";
  msg += std::to_string(use_ammo) + ";";
  msg += std::to_string(ammo_type) + ";";
  msg += std::to_string(capacity) + ";";
  for(Effect * effect : effects) {
    msg += effect->to_string(); + ",";
  }
  msg += ";";
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    msg += std::to_string(damages[i]) + ";";
  }
  return msg;
}

Weapon * Weapon::from_string(std::string to_read) {
  std::string msg = to_read;
  if(to_read == "none") {
    return nullptr;
  }
  std::string name = msg.substr(0, msg.find(';'));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  std::string melee_str = msg.substr(0, msg.find(';'));
  bool melee = (melee_str == "1");
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int range = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int type = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int weight = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int gold_value = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  std::string use_ammo_str = msg.substr(0, msg.find(';'));
  bool use_ammo = (use_ammo_str == "1");
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int ammo_type = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int capacity = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  std::list<Effect *> * effects = new std::list<Effect *>();
  std::istringstream isEffects(msg.substr(0, msg.find(';')));
  std::string effect;
  while(getline(isEffects, effect, ',') && effect != "") {
    effects->push_back(Effect::from_string(effect));
  }
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int damages[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] = stoi(msg.substr(0, msg.find(';')));
    msg = msg.substr(msg.find(';') + 1, msg.length());
  }
  return new Weapon(
    name,
    melee,
    range,
    type,
    weight,
    gold_value,
    use_ammo,
    ammo_type,
    capacity,
    *effects,
    damages
  );
}
