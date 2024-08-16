#ifndef _SUMMON_SKILL_H_
#define _SUMMON_SKILL_H_

#include "data/skills/PseudoSkill.h"

class SummonSkill : public PseudoSkill {
  public:
    const std::string ai_str;
    const std::string team;
    const int32_t apparition_type;
    const int32_t xp;
    SummonSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      int32_t scalling_type,
      std::array<float, DAMAGE_TYPE_NUMBER> damage_multipliers,
      std::list<Effect *> effects,
      Character * character,
      std::string ai_str,
      std::string team,
      Race * race,
      std::list<Race *> race_modifiers,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession,
      Attributes * attributes,
      std::list<Way *> titles,
      int32_t apparition_type,
      int32_t xp
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, scalling_type, damage_multipliers, effects),
      character(character),
      ai_str(ai_str),
      team(team),
      race(race),
      race_modifiers(race_modifiers),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession),
      attributes(attributes),
      titles(titles),
      apparition_type(apparition_type),
      xp(xp)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range);
    float getPower();
    float getDamageFromType(int32_t dammage_type, Character * owner, float overcharge);
    float getDamageReductionFromType(int32_t dammage_type, float overcharge);
    Character * getCharacter();
    std::string getAI();
    Race * getRace();
    std::list<Race *> getRaceModifiers();
    Way * getOrigin();
    Way * getCulture();
    Way * getReligion();
    Way * getProfession();
    Attributes * getAttributes();
    std::list<Way *> getTitles();
  private:
    Character * character;
    Race * race;
    std::list<Race *> race_modifiers;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
    Attributes * attributes;
    std::list<Way *> titles;
};

#endif // _SUMMON_SKILL_H_
