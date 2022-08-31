#include "data/Attributes.h"

#include "data/Effect.h"
#include "data/Gear.h"
#include "data/skills/Skill.h"

#include "utils/String.h"

std::list<Effect *> Attributes::getEffects() { return effects; }
std::list<Skill *> Attributes::getSkills() { return skills; }
Gear * Attributes::getStartingGear() { return startingGear; }

std::string Attributes::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, baseHp);
  String::insert_int(ss, baseMana);
  String::insert_int(ss, baseArmor);
  String::insert_int(ss, baseSoulBurn);
  String::insert_int(ss, baseFlow);
  String::insert_int(ss, baseVisionRange);
  String::insert_int(ss, baseVisionPower);
  String::insert_int(ss, baseDetectionRange);
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
  String::insert(ss, startingGear->to_string());
  std::string result = ss->str();
  delete ss;
  return result;
}

Attributes * Attributes::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int baseHp = String::extract_int(ss);
  int baseMana = String::extract_int(ss);
  int baseArmor = String::extract_int(ss);
  int baseSoulBurn = String::extract_int(ss);
  int baseFlow = String::extract_int(ss);
  int baseVisionRange = String::extract_int(ss);
  int baseVisionPower = String::extract_int(ss);
  int baseDetectionRange = String::extract_int(ss);
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
  Gear * gear = Gear::from_string(String::extract(ss));
  delete ss;
  Attributes * result = new Attributes(name, baseHp, baseMana, baseArmor, baseSoulBurn, baseFlow, baseVisionRange, baseVisionPower, baseDetectionRange, *effects, *skills, gear);
  delete effects;
  delete skills;
  return result;
}
