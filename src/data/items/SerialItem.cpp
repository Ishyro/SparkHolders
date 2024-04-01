#include <sstream>

#include "data/Effect.h"

#include "data/items/SerialItem.h"

#include "util/String.h"

int32_t SerialItem::getNumber() { return number; }

int32_t SerialItem::add(int32_t number) { this->number += number; return this->number; }

int32_t SerialItem::reduce(int32_t number) { this->number -= number; return this->number; }

float SerialItem::getWeight() { return weight * number; }

std::string SerialItem::to_string() {
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
  std::string result = ss->str();
  delete ss;
  return result;
}
