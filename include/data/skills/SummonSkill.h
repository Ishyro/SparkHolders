#ifndef _SUMMON_SKILL_H_
#define _SUMMON_SKILL_H_

#include "data/skills/PseudoSkill.h"

class SummonSkill : public PseudoSkill {
  using PseudoSkill::PseudoSkill;
  public:
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
    int getApparitionType();
    int getXp();
    void setCharacter(Character * character);
    void setAI(std::string ai_str);
    void setRace(Way * race);
    void setOrigin(Way * origin);
    void setCulture(Way * culture);
    void setReligion(Way * religion);
    void setProfession(Way * profession);
    void setAttributes(Attributes * attributes);
    void setTitles(std::list<Way *> titles);
    void setApparitionType(int apparition_type);
    void setXp(int xp);
  private:
    Character * character;
    std::string team;
    std::string ai_str;
    Way * race;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
    Attributes * attributes;
    std::list<Way *> titles;
    int apparition_type;
    int xp;
};

#endif // _SUMMON_SKILL_H_
