#include "data/Effect.h"

#include "data/Item.h"

float Item::getDamageReductionFromType(int damage_type) {
  float reduction = damage_reductions[damage_type];
  for(Effect * e : effects) {
    if(e->type == DAMAGE_REDUCTION) {
      reduction += e->getDamageReductionFromType(damage_type);
    }
  }
  return reduction;
}

std::string Item::to_string() {

}

Item * Item::from_string(std::string to_read) {

}
