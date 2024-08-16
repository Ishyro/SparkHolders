#include "data/skills/SummonSkill.h"

#include "ai/AI.h"
#include "ai/PlayerAI.h"
#include "ai/DiurnalPassiveAI.h"
#include "ai/NocturnalPassiveAI.h"
#include "ai/DiurnalAgressiveAI.h"
#include "ai/NocturnalAgressiveAI.h"
#include "ai/EtheralCasterAI.h"

void SummonSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {
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
    (int32_t) target->coord.x,
    (int32_t) target->coord.y,
    (int32_t) target->coord.z,
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

bool SummonSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {
  return true;
}

float SummonSkill::getPower() {
  return 0.F;
}

float SummonSkill::getDamageFromType(int32_t damage_type, Character * owner, float overcharge) {
  return 0.F;
}

float SummonSkill::getDamageReductionFromType(int32_t damage_type, float overcharge) {
  return 0.F;
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
