#include "data/skills/SummonSkill.h"

#include "ai/AI.h"
#include "ai/PlayerAI.h"
#include "ai/DiurnalPassiveAI.h"
#include "ai/NocturnalPassiveAI.h"
#include "ai/DiurnalAgressiveAI.h"
#include "ai/NocturnalAgressiveAI.h"
#include "ai/EtheralCasterAI.h"

void SummonSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {
  // copied from FileOpener.cpp
  AI * ai;
  if (ai_str == "DiurnalPassiveAI") {
    ai = new DiurnalPassiveAI(x, y);
  }
  else if (ai_str == "NocturnalPassiveAI") {
    ai = new NocturnalPassiveAI(x, y);
  }
  else if (ai_str == "DiurnalAgressiveAI") {
    ai = new DiurnalAgressiveAI(x, y);
  }
  else if (ai_str == "NocturnalAgressiveAI") {
    ai = new NocturnalAgressiveAI(x, y);
  }
  else if (ai_str == "EtheralCasterAI") {
    ai = new EtheralCasterAI(x, y);
  }
  Character * c = new Character(character, character->name, xp, x, y, owner->getOrientation(), map_id, owner->getTeam(), ai, attributes, race, origin, culture, religion, profession, titles);
  adventure->getWorld()->getMap(map_id)->addCharacter(c);
  if(apparition_type == SOFT) {
    adventure->softMoveCharacterToMap(c, map_id, y, x);
  } else {
    adventure->hardMoveCharacterToMap(c, map_id, y, x);
  }
}

bool SummonSkill::canCast(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range) {
  return true;
}

int SummonSkill::getPower() {
  return 0;
}

int SummonSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float SummonSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}

Character * SummonSkill::getCharacter() { return character; }
std::string SummonSkill::getAI() { return ai_str; }
Way * SummonSkill::getRace() { return race; }
Way * SummonSkill::getOrigin() { return origin; }
Way * SummonSkill::getCulture() { return culture; }
Way * SummonSkill::getReligion() { return religion; }
Way * SummonSkill::getProfession() { return profession; }
Attributes * SummonSkill::getAttributes() { return attributes; }
std::list<Way *> SummonSkill::getTitles() { return titles; }
int SummonSkill::getApparitionType() { return apparition_type; }
int SummonSkill::getXp() { return xp; }

void SummonSkill::setCharacter(Character * character) { this->character = character; }
void SummonSkill::setAI(std::string ai_str) { this->ai_str = ai_str; }
void SummonSkill::setRace(Way * race) { this->race = race; }
void SummonSkill::setOrigin(Way * origin) { this->origin = origin; }
void SummonSkill::setCulture(Way * culture) { this->culture = culture; }
void SummonSkill::setReligion(Way * religion) { this->religion = religion; }
void SummonSkill::setProfession(Way * profession) { this->profession = profession; }
void SummonSkill::setAttributes(Attributes * attributes) { this->attributes = attributes; }
void SummonSkill::setTitles(std::list<Way *> titles) { this->titles = titles; }
void SummonSkill::setApparitionType(int apparition_type) { this->apparition_type = apparition_type; }
void SummonSkill::setXp(int xp) { this->xp = xp; }
