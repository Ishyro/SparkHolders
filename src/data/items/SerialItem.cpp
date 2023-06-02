#include <sstream>

#include "data/Effect.h"

#include "data/items/SerialItem.h"

#include "util/String.h"

bool SerialItem::isFood() {
  for(Effect * effect : effects) {
    if(effect->type == EFFECT_SATIETY) {
      return true;
    }
  }
  return false;
}

int SerialItem::getNumber() { return number; }

int SerialItem::add(int number) { this->number += number; return this->number; }

int SerialItem::reduce(int number) { this->number -= number; return this->number; }

std::string SerialItem::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, type);
  String::insert_int(ss, type2);
  String::insert_int(ss, tier);
  String::insert_int(ss, max_tier);
  String::insert_float(ss, weight);
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
  std::string result = ss->str();
  delete ss;
  return result;
}
