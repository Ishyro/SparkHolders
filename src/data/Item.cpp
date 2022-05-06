#include <sstream>

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
  std::string msg = name + ";";
  msg += std::to_string(equipable) + ";";
  msg += std::to_string(consumable) + ";";
  msg += std::to_string(type) + ";";
  msg += std::to_string(gold_value) + ";";
  for(Effect * effect : effects) {
    msg += effect->to_string(); + ",";
  }
  msg += ";";
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    msg += std::to_string(damage_reductions[i]) + ";";
  }
  return msg;
}

Item * Item::from_string(std::string to_read) {
  std::string msg = to_read;
  if(to_read == "none") {
    return nullptr;
  }
  std::string name = msg.substr(0, msg.find(';'));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  std::string equipable_str = msg.substr(0, msg.find(';'));
  bool equipable = (equipable_str == "1");
  msg = msg.substr(msg.find(';') + 1, msg.length());
  std::string consumable_str = msg.substr(0, msg.find(';'));
  bool consumable = (consumable_str == "1");
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int type = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int gold_value = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  std::list<Effect *> * effects = new std::list<Effect *>();
  std::istringstream isEffects(msg.substr(0, msg.find(';')));
  std::string effect;
  while(getline(isEffects, effect, ',') && effect != "") {
    effects->push_back(Effect::from_string(effect));
  }
  msg = msg.substr(msg.find(';') + 1, msg.length());
  float damage_reductions[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damage_reductions[i] = stof(msg.substr(0, msg.find(';')));
    msg = msg.substr(msg.find(';') + 1, msg.length());
  }
  return new Item(
    name,
    equipable,
    consumable,
    type,
    gold_value,
    *effects,
    damage_reductions
  );
}
