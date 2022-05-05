#include <algorithm>

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

int Weapon::getCurrentCapacity() { return ammo->number; }

void Weapon::useAmmo() {
  if(this->ammo != nullptr && --this->ammo->number == 0) {
    delete this->ammo;
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

}

Weapon * Weapon::from_string(std::string to_read) {
  
}
