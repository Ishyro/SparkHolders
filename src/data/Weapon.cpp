#include "data/Weapon.h"

int Weapon::getRawDamage() { 
  int power = 0;
  for (int i = 0; i < DAMAGE_TYPE_NUMBER; i++) { 
    power += damages[i];
  }
  return power;
}

int Weapon::getDamageType(int damage_type) { return damages[damage_type]; }

int Weapon::getCurrentCapacity() { return current_capacity; }
