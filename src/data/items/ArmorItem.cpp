#include <sstream>

#include "data/Effect.h"

#include "data/items/ArmorItem.h"

#include "util/String.h"

float ArmorItem::getDamageReductionFromType(int32_t damage_type) {
  float reduction = damage_reductions[damage_type];
  for(Effect * e : effects) {
    if(e->type == EFFECT_DAMAGE_REDUCTION) {
      reduction += e->getDamageReductionFromType(damage_type);
    }
  }
  return reduction;
}

std::string ArmorItem::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, type);
  String::insert_int(ss, type2);
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
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_float(ss, damage_reductions[i]);
  }
  std::string result = ss->str();
  delete ss;
  return result;
}
