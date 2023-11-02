#include <sstream>

#include "data/Effect.h"

#include "data/items/AmmunitionItem.h"

#include "util/String.h"

Projectile * AmmunitionItem::getProjectile() { return projectile; }

std::string AmmunitionItem::to_string() {
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
  String::insert_int(ss, number);
  String::insert_int(ss, max);
  if(projectile != nullptr) {
    String::insert(ss, projectile->name);
  }
  else {
    String::insert(ss, "none");
  }
  std::string result = ss->str();
  delete ss;
  return result;
}
