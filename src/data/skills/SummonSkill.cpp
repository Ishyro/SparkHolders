#include "data/skills/SummonSkill.h"

#include "ai/AI.h"
#include "ai/PlayerAI.h"
#include "ai/DiurnalPassiveAI.h"
#include "ai/NocturnalPassiveAI.h"
#include "ai/DiurnalAgressiveAI.h"
#include "ai/NocturnalAgressiveAI.h"
#include "ai/EtheralCasterAI.h"

void SummonSkill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  // Target will be a Block
  AI * ai;
  if (ai_str == "DiurnalPassiveAI") {
    ai = new DiurnalPassiveAI(target->coord.x, target->coord.y);
  }
  else if (ai_str == "NocturnalPassiveAI") {
    ai = new NocturnalPassiveAI(target->coord.x, target->coord.y);
  }
  else if (ai_str == "DiurnalAgressiveAI") {
    ai = new DiurnalAgressiveAI(target->coord.x, target->coord.y);
  }
  else if (ai_str == "NocturnalAgressiveAI") {
    ai = new NocturnalAgressiveAI(target->coord.x, target->coord.y);
  }
  else if (ai_str == "EtheralCasterAI") {
    ai = new EtheralCasterAI(target->coord.x, target->coord.y);
  }
  Character * c = new Character(
    character,
    character->name,
    xp,
    0,
    (int) target->coord.x,
    (int) target->coord.y,
    (int) target->coord.z,
    owner->getOrientation(),
    nullptr,
    team == "" ? owner->getTeam() : team,
    ai,
    attributes,
    nullptr,
    nullptr,
    (Gear *) adventure->getDatabase()->getGear("TXT_NO_GEAR"),
    race,
    race_modifiers,
    origin,
    culture,
    religion,
    profession,
    titles
  );
  if(apparition_type == APPARITION_SOFT) {
    adventure->softMoveCharacterToMap(c, target->coord);
  }
  else {
    adventure->hardMoveCharacterToMap(c, target->coord);
  }
}

bool SummonSkill::canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
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
Race * SummonSkill::getRace() { return race; }
std::list<Race *> SummonSkill::getRaceModifiers() { return race_modifiers; }
Way * SummonSkill::getOrigin() { return origin; }
Way * SummonSkill::getCulture() { return culture; }
Way * SummonSkill::getReligion() { return religion; }
Way * SummonSkill::getProfession() { return profession; }
Attributes * SummonSkill::getAttributes() { return attributes; }
std::list<Way *> SummonSkill::getTitles() { return titles; }
