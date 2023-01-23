#include "data/Attributes.h"

#include "data/Effect.h"
#include "data/Gear.h"
#include "data/skills/Skill.h"

#include "utils/String.h"

void Attributes::init(Attributes * archetype) {
  this->archetype = archetype;
}

Attributes * Attributes::getArchetype() { return archetype; }
std::list<Item *> Attributes::getItems() { return items; }
std::list<Weapon *> Attributes::getWeapons() { return weapons; }
std::list<Ammunition *> Attributes::getAmmunitions() { return ammunition; }
std::list<Effect *> Attributes::getEffects() { return effects; }
std::list<Skill *> Attributes::getSkills() { return skills; }
Gear * Attributes::getStartingGear() { return startingGear; }

std::string Attributes::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  if(archetype != nullptr) {
    String::insert(ss, archetype->to_string());
  }
  else {
    String::insert(ss, "none");
  }
  String::insert_int(ss, baseHp);
  String::insert_int(ss, baseMana);
  String::insert_int(ss, baseArmor);
  String::insert_int(ss, baseDamage);
  String::insert_int(ss, baseSoulBurn);
  String::insert_int(ss, baseFlow);
  String::insert_int(ss, baseVisionRange);
  String::insert_int(ss, baseVisionPower);
  String::insert_int(ss, baseDetectionRange);
  String::insert_int(ss, hpIncr);
  String::insert_int(ss, manaIncr);
  String::insert_int(ss, armorIncr);
  String::insert_int(ss, damageIncr);
  String::insert_int(ss, soulBurnIncr);
  String::insert_int(ss, flowIncr);
  std::stringstream * ss_items = new std::stringstream();
  for(Item * item : items) {
    String::insert(ss_items, item->to_string());
  }
  String::insert(ss, ss_items->str());
  delete ss_items;
  std::stringstream * ss_weapons = new std::stringstream();
  for(Weapon * weapon : weapons) {
    String::insert(ss_weapons, weapon->to_string());
  }
  String::insert(ss, ss_weapons->str());
  delete ss_weapons;
  std::stringstream * ss_ammunition = new std::stringstream();
  for(Ammunition * ammo : ammunition) {
    String::insert(ss_ammunition, Projectile::ammo_to_string(ammo));
  }
  String::insert(ss, ss_ammunition->str());
  delete ss_ammunition;
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
  Attributes * archetype = nullptr;
  std::string archetype_str = String::extract(ss);
  if(archetype_str != "none") {
    archetype = from_string(archetype_str);
  }
  int baseHp = String::extract_int(ss);
  int baseMana = String::extract_int(ss);
  int baseArmor = String::extract_int(ss);
  int baseDamage = String::extract_int(ss);
  int baseSoulBurn = String::extract_int(ss);
  int baseFlow = String::extract_int(ss);
  int baseVisionRange = String::extract_int(ss);
  int baseVisionPower = String::extract_int(ss);
  int baseDetectionRange = String::extract_int(ss);
  int hpIncr = String::extract_int(ss);
  int manaIncr = String::extract_int(ss);
  int armorIncr = String::extract_int(ss);
  int damageIncr = String::extract_int(ss);
  int soulBurnIncr = String::extract_int(ss);
  int flowIncr = String::extract_int(ss);
  std::stringstream * ss_items = new std::stringstream(String::extract(ss));
  std::list<Item *> * items = new std::list<Item *>();
  while(ss_items->rdbuf()->in_avail() != 0) {
    items->push_back(Item::from_string(String::extract(ss_items)));
  }
  delete ss_items;
  std::stringstream * ss_weapons = new std::stringstream(String::extract(ss));
  std::list<Weapon *> * weapons = new std::list<Weapon *>();
  while(ss_weapons->rdbuf()->in_avail() != 0) {
    weapons->push_back(Weapon::from_string(String::extract(ss_weapons)));
  }
  delete ss_weapons;
  std::stringstream * ss_ammunition = new std::stringstream(String::extract(ss));
  std::list<Ammunition *> * ammunition = new std::list<Ammunition *>();
  while(ss_ammunition->rdbuf()->in_avail() != 0) {
    ammunition->push_back(Projectile::ammo_from_string(String::extract(ss_ammunition)));
  }
  delete ss_ammunition;
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
  Attributes * result = new Attributes(
    name,
    archetype,
    baseHp,
    baseMana,
    baseArmor,
    baseDamage,
    baseSoulBurn,
    baseFlow,
    baseVisionRange,
    baseVisionPower,
    baseDetectionRange,
    hpIncr,
    manaIncr,
    armorIncr,
    damageIncr,
    soulBurnIncr,
    flowIncr,
    *items,
    *weapons,
    *ammunition,
    *effects,
    *skills,
    gear
  );
  delete items;
  delete weapons;
  delete ammunition;
  delete effects;
  delete skills;
  return result;
}
