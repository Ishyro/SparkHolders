#include "data/skills/Skill.h"

#include "utils/String.h"

void Skill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_area, int map_id, int x, int y) {
  owner->payMana(getManaCost(overcharge_power, overcharge_duration, overcharge_area));
  for(PseudoSkill * skill : skills) {
    skill->activate(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_area_type, overcharge_power, overcharge_duration, overcharge_area, map_id, x, y);
  }
}

int Skill::getManaCost(int overcharge_power, int overcharge_duration, int overcharge_area) {
  int mana_cost = 0;
  for(PseudoSkill * skill : skills) {
    mana_cost += skill->getManaCost(overcharge_power_type, overcharge_duration_type, overcharge_area_type, overcharge_power, overcharge_duration, overcharge_area);
  }
  return mana_cost;
}

int Skill::getPower() {
  int power = 0;
  for(PseudoSkill * skill : skills) {
    power += skill->power;
  }
  return power;
}

std::string Skill::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, target_type);
  String::insert_int(ss, overcharge_power_type);
  String::insert_int(ss, overcharge_duration_type);
  String::insert_int(ss, overcharge_area_type);
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
  int overcharge_area_type = String::extract_int(ss);
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
    overcharge_area_type,
    range,
    *skills
  );
}
