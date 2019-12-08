#include "src/data/Weapon.h"

int Weapon::getRawDamage() {
  int power = 0;
  for (int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += damages[i];
  }
  return power;
}

int Weapon::getCurrentCapacity() {return current_capacity;}
