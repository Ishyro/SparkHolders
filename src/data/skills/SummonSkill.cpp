#include "data/skills/SummonSkill.h"

#include "ai/AI.h"
#include "ai/PlayerAI.h"
#include "ai/DiurnalPassiveAI.h"
#include "ai/NocturnalPassiveAI.h"
#include "ai/DiurnalAgressiveAI.h"
#include "ai/NocturnalAgressiveAI.h"
#include "ai/EtheralCasterAI.h"

void SummonSkill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  // Target will be a Tile
  AI * ai;
  if (ai_str == "DiurnalPassiveAI") {
    ai = new DiurnalPassiveAI(target->x, target->y);
  }
  else if (ai_str == "NocturnalPassiveAI") {
    ai = new NocturnalPassiveAI(target->x, target->y);
  }
  else if (ai_str == "DiurnalAgressiveAI") {
    ai = new DiurnalAgressiveAI(target->x, target->y);
  }
  else if (ai_str == "NocturnalAgressiveAI") {
    ai = new NocturnalAgressiveAI(target->x, target->y);
  }
  else if (ai_str == "EtheralCasterAI") {
    ai = new EtheralCasterAI(target->x, target->y);
  }
  Character * c = new Character(
    character,
    character->name,
    xp,
    0,
    (int) target->x,
    (int) target->y,
    owner->getOrientation(),
    target->id,
    team == "" ? owner->getTeam() : team,
    ai,
    attributes,
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
  adventure->getWorld()->getMap(target->id)->addCharacter(c);
  if(apparition_type == APPARITION_SOFT) {
    adventure->softMoveCharacterToMap(c, target->id, target->y, target->x);
  }
  else {
    adventure->hardMoveCharacterToMap(c, target->id, target->y, target->x);
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
