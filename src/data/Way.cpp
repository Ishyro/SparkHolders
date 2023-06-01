#include "data/Way.h"

#include "data/Effect.h"
#include "data/skills/Skill.h"

#include "util/String.h"

std::list<Item *> Way::getLoot() { return loot; }
std::list<Effect *> Way::getEffects() { return effects; }
std::list<Skill *> Way::getSkills() { return skills; }

std::string Way::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, type);
  String::insert_int(ss, baseHp);
  String::insert_int(ss, baseMana);
  String::insert_int(ss, baseArmorMult);
  String::insert_int(ss, baseDamageMult);
  String::insert_int(ss, baseSoulBurn);
  String::insert_int(ss, baseFlow);
  String::insert_int(ss, baseVisionRange);
  String::insert_int(ss, baseVisionPower);
  String::insert_int(ss, baseDetectionRange);
  String::insert_int(ss, hpIncr);
  String::insert_int(ss, manaIncr);
  String::insert_int(ss, armorMultIncr);
  String::insert_int(ss, damageMultIncr);
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
  if(type == RACE) {
    String::insert_int(ss, baseArmor);
    String::insert_float(ss, size);
    String::insert_bool(ss, need_to_eat);
    String::insert_bool(ss, can_eat_food);
    String::insert_bool(ss, need_to_sleep);
    String::insert_float(ss, action_time_modifier);
    String::insert_float(ss, strike_time_modifier);
    String::insert_float(ss, skill_time_modifier);
    String::insert_float(ss, movement_time_modifier);
    std::stringstream * ss_loot = new std::stringstream();
    for(Item * item : loot) {
      String::insert(ss_loot, item->to_string());
    }
    String::insert(ss, ss_loot->str());
    delete ss_loot;
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

Way * Way::from_string(std::string to_read, Adventure * adventure) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int type = String::extract_int(ss);
  int baseHp = String::extract_int(ss);
  int baseMana = String::extract_int(ss);
  int baseArmorMult = String::extract_int(ss);
  int baseDamageMult = String::extract_int(ss);
  int baseSoulBurn = String::extract_int(ss);
  int baseFlow = String::extract_int(ss);
  int baseVisionRange = String::extract_int(ss);
  int baseVisionPower = String::extract_int(ss);
  int baseDetectionRange = String::extract_int(ss);
  int hpIncr = String::extract_int(ss);
  int manaIncr = String::extract_int(ss);
  int armorMultIncr = String::extract_int(ss);
  int damageMultIncr = String::extract_int(ss);
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
  if(type == RACE) {
    int baseArmor = String::extract_int(ss);
    float size = String::extract_float(ss);
    bool need_to_eat = String::extract_bool(ss);
    bool can_eat_food = String::extract_bool(ss);
    bool need_to_sleep = String::extract_bool(ss);
    float action_time_modifier = String::extract_float(ss);
    float strike_time_modifier = String::extract_float(ss);
    float skill_time_modifier = String::extract_float(ss);
    float movement_time_modifier = String::extract_float(ss);
    std::stringstream * ss_loot = new std::stringstream(String::extract(ss));
    std::list<Item *> * loot = new std::list<Item *>();
    while(ss_loot->rdbuf()->in_avail() != 0) {
      loot->push_back(Item::from_string(String::extract(ss_loot), adventure));
    }
    delete ss_loot;
    delete ss;
    Way * result = new Way(
      name,
      type,
      baseHp,
      baseMana,
      baseArmor,
      baseArmorMult,
      baseDamageMult,
      baseSoulBurn,
      baseFlow,
      baseVisionRange,
      baseVisionPower,
      baseDetectionRange,
      hpIncr,
      manaIncr,
      armorMultIncr,
      damageMultIncr,
      soulBurnIncr,
      flowIncr,
      size,
      need_to_eat,
      can_eat_food,
      need_to_sleep,
      action_time_modifier,
      strike_time_modifier,
      skill_time_modifier,
      movement_time_modifier,
      *loot,
      *effects,
      *skills
    );
    delete loot;
    delete effects;
    delete skills;
    return result;
  }
  else {
    delete ss;
    Way * result = new Way(
      name,
      type,
      baseHp,
      baseMana,
      baseArmorMult,
      baseDamageMult,
      baseSoulBurn,
      baseFlow,
      baseVisionRange,
      baseVisionPower,
      baseDetectionRange,
      hpIncr,
      manaIncr,
      armorMultIncr,
      damageMultIncr,
      soulBurnIncr,
      flowIncr,
      *effects,
      *skills
    );
    delete effects;
    delete skills;
    return result;
  }
}
