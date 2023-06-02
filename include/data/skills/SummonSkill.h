#ifndef _SUMMON_SKILL_H_
#define _SUMMON_SKILL_H_

#include "data/skills/PseudoSkill.h"

class SummonSkill : public PseudoSkill {
  public:
    const std::string ai_str;
    const std::string team;
    const int apparition_type;
    const int xp;
    SummonSkill(
      std::string name,
      int skill_type,
      int target_type,
      int mana_cost,
      std::list<Effect *> effects,
      Character * character,
      std::string ai_str,
      std::string team,
      Way * race,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession,
      Attributes * attributes,
      std::list<Way *> titles,
      int apparition_type,
      int xp
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, effects),
      character(character),
      ai_str(ai_str),
      team(team),
      race(race),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession),
      attributes(attributes),
      titles(titles),
      apparition_type(apparition_type),
      xp(xp)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
    Character * getCharacter();
    std::string getAI();
    Way * getRace();
    Way * getOrigin();
    Way * getCulture();
    Way * getReligion();
    Way * getProfession();
    Attributes * getAttributes();
    std::list<Way *> getTitles();
  private:
    Character * character;
    Way * race;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
    Attributes * attributes;
    std::list<Way *> titles;
};

#endif // _SUMMON_SKILL_H_
