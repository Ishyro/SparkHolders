#include "data/Effect.h"

#include "data/Weapon.h"

int Weapon::getRawDamage() {
  int power = 0;
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += damages[i];
  }
  return power;
}

int Weapon::getDamageFromType(int damage_type) {
  int damage = damages[damage_type];
  for(Effect * e : effects) {
    if(e->type == DAMAGE_BUFF) {
      damage += e->getDamageFromType(damage_type);
    }
  }
  return damage;
}

int Weapon::getCurrentCapacity() { return current_capacity; }
