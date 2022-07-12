#include "data/Way.h"

#include "data/Effect.h"
#include "data/skills/Skill.h"

#include "utils/String.h"

std::list<Effect *> Way::getEffects() { return effects; }
std::list<Skill *> Way::getSkills() { return skills; }

std::string Way::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, type);
  String::insert_int(ss, hpIncr);
  String::insert_int(ss, manaIncr);
  String::insert_int(ss, armorIncr);
  String::insert_int(ss, soulBurnIncr);
  String::insert_int(ss, flowIncr);
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  std::stringstream * ss_skills = new std::stringstream();
  for(Skill * skill : skills) {
    String::insert(ss_skills, skill->to_string());
  }
  String::insert(ss, ss_skills->str());
  delete ss_skills;
  std::string result = ss->str();
  delete ss;
  return result;
}

Way * Way::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int type = String::extract_int(ss);
  int hpIncr = String::extract_int(ss);
  int manaIncr = String::extract_int(ss);
  int armorIncr = String::extract_int(ss);
  int soulBurnIncr = String::extract_int(ss);
  int flowIncr = String::extract_int(ss);
  std::stringstream * ss_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * effects = new std::list<Effect *>();
  while(ss_effects->rdbuf()->in_avail() != 0) {
    effects->push_back(Effect::from_string(String::extract(ss_effects)));
  }
  delete ss_effects;
  std::stringstream * ss_skills = new std::stringstream(String::extract(ss));
  std::list<Skill *> * skills = new std::list<Skill *>();
  while(ss_skills->rdbuf()->in_avail() != 0) {
    skills->push_back(Skill::from_string(String::extract(ss_skills)));
  }
  delete ss_skills;
  delete ss;
  return new Way(name, type, hpIncr, manaIncr, armorIncr, soulBurnIncr, flowIncr, *effects, *skills);
}
