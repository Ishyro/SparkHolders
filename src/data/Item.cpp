#include <sstream>

#include "data/Effect.h"

#include "data/Item.h"

#include "utils/String.h"

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
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_bool(ss, equipable);
  String::insert_bool(ss, consumable);
  String::insert_int(ss, type);
  String::insert_float(ss, weight);
  String::insert_int(ss, gold_value);
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_float(ss, damage_reductions[i]);
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

Item * Item::from_string(std::string to_read) {
  if(to_read == "none") {
    return nullptr;
  }
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  bool equipable = String::extract_bool(ss);
  bool consumable = String::extract_bool(ss);
  int type = String::extract_int(ss);
  float weight = String::extract_float(ss);
  int gold_value = String::extract_int(ss);
  std::stringstream * ss_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * effects = new std::list<Effect *>();
  while(ss_effects->rdbuf()->in_avail() != 0) {
    effects->push_back(Effect::from_string(String::extract(ss_effects)));
  }
  delete ss_effects;
  float damage_reductions[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damage_reductions[i] = String::extract_float(ss);
  }
  delete ss;
  return new Item(
    name,
    equipable,
    consumable,
    type,
    weight,
    gold_value,
    *effects,
    damage_reductions
  );
}
