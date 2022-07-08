#include "data/skills/PseudoSkill.h"

#include "utils/String.h"

int PseudoSkill::getManaCost(int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge) {
  int cost = -2 * mana_cost;
  switch(overcharge_power_type) {
    case NO_OVERCHARGE:
      cost += mana_cost;
    case LINEAR:
      cost += mana_cost * overcharge;
    case SQUARE:
      cost += mana_cost * overcharge * overcharge;
    case EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge - 1);
    default:
      ;
  }
  switch(overcharge_duration_type) {
    case NO_OVERCHARGE:
      cost += mana_cost;
    case LINEAR:
      cost += mana_cost * overcharge;
    case SQUARE:
      cost += mana_cost * overcharge * overcharge;
    case EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge - 1);
    default:
      ;
  }
  switch(overcharge_area_type) {
    case NO_OVERCHARGE:
      cost += mana_cost;
    case LINEAR:
      cost += mana_cost * overcharge;
    case SQUARE:
      cost += mana_cost * overcharge * overcharge;
    case EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge - 1);
    default:
      ;
  }
  return cost;
}


void PseudoSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge, int map_id, int x, int y) {}

int PseudoSkill::getPower() {
  int power = 0;
  for(Effect * effect : effects) {
    if(effect->power != 0) {
      power += effect->power;
    } else {
      power += effect->getRawDamage();
    }
  }
  return power;
}

std::string PseudoSkill::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, skill_type);
  String::insert_int(ss, target_type);
  String::insert_int(ss, mana_cost);
  String::insert_int(ss, getPower());
  std::string result = ss->str();
  delete ss;
  return result;
}

PseudoSkill * PseudoSkill::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int skill_type = String::extract_int(ss);
  int target_type = String::extract_int(ss);
  int mana_cost = String::extract_int(ss);
  int power = String::extract_int(ss);
  std::list<Effect *> * effects = new std::list<Effect *>();
  delete ss;
  return new PseudoSkill(
    name,
    skill_type,
    target_type,
    mana_cost,
    *effects,
    power
  );
}
