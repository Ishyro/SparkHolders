#include "data/skills/Skill.h"

#include "utils/String.h"

void Skill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y) {
  owner->payMana(getManaCost(overcharge_power, overcharge_duration, overcharge_range));
  for(PseudoSkill * skill : skills) {
    skill->activate(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, map_id, x, y, range);
  }
}

int Skill::getManaCost(int overcharge_power, int overcharge_duration, int overcharge_range) {
  int mana_cost = 0;
  for(PseudoSkill * skill : skills) {
    mana_cost += skill->getManaCost(overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range);
  }
  return mana_cost;
}

int Skill::getPower() {
  int power = 0;
  for(PseudoSkill * skill : skills) {
    power += skill->getPower();
  }
  return power;
}

int Skill::getDamageFromType(int damage_type, int overcharge_power) {
  int damage = 0;
  for(PseudoSkill * s : skills) {
    damage += s->getDamageFromType(damage_type, overcharge_power);
  }
  return damage;
}

float Skill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  float reduction = 0.;
  for(PseudoSkill * s : skills) {
    reduction += s->getDamageReductionFromType(damage_type, overcharge_power);
  }
  return reduction;
}

std::string Skill::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, target_type);
  String::insert_int(ss, overcharge_power_type);
  String::insert_int(ss, overcharge_duration_type);
  String::insert_int(ss, overcharge_range_type);
  String::insert_int(ss, range);
  std::stringstream * ss_skills = new std::stringstream();
  for(PseudoSkill * skill : skills) {
    String::insert(ss_skills, skill->to_string());
  }
  String::insert(ss, ss_skills->str());
  delete ss_skills;
  std::string result = ss->str();
  delete ss;
  return result;
}

Skill * Skill::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int target_type = String::extract_int(ss);
  int overcharge_power_type = String::extract_int(ss);
  int overcharge_duration_type = String::extract_int(ss);
  int overcharge_range_type = String::extract_int(ss);
  int range = String::extract_int(ss);
  std::stringstream * ss_skills = new std::stringstream(String::extract(ss));
  std::list<PseudoSkill *> * skills = new std::list<PseudoSkill *>();
  while(ss_skills->rdbuf()->in_avail() != 0) {
    skills->push_back(PseudoSkill::from_string(String::extract(ss_skills)));
  }
  delete ss_skills;
  delete ss;
  return new Skill(
    name,
    target_type,
    overcharge_power_type,
    overcharge_duration_type,
    overcharge_range_type,
    range,
    *skills
  );
}
