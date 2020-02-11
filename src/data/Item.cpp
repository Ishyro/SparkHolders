#include "src/data/Item.h"

float Item::getDamageReduction(int damage_type) { 
  if (this != nullptr) { 
    return damage_reductions[damage_type];
  }
  return 0.;
}
