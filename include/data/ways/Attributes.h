#ifndef _ATTRIBUTES_H_
#define _ATTRIBUTES_H_

#include "data/ways/Way.h"

#include <list>
#include <string>

class Attributes : public Way {
  public:
    Attributes(
      std::string name,
      int type,
      int tier,
      int baseHp,
      int baseMana,
      int baseShield,
      int baseDamageMult,
      int baseSoulBurn,
      int baseFlow,
      int baseVisionRange,
      int baseVisionPower,
      int baseDetectionRange,
      int hpIncr,
      int manaIncr,
      int shieldIncr,
      int damageMultIncr,
      int soulBurnIncr,
      int flowIncr,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      std::list<std::string> tags,
      std::list<std::string> and_requirements,
      std::list<std::string> or_requirements,
      std::list<std::string> not_requirements
    ):
      Way(
        name,
        type,
        tier,
        baseHp,
        baseMana,
        baseShield,
        baseDamageMult,
        baseSoulBurn,
        baseFlow,
        baseVisionRange,
        baseVisionPower,
        baseDetectionRange,
        hpIncr,
        manaIncr,
        shieldIncr,
        damageMultIncr,
        soulBurnIncr,
        flowIncr,
        effects,
        skills,
        tags
      ),
      and_requirements(and_requirements),
      or_requirements(or_requirements),
      not_requirements(not_requirements)
    {}
  private:
    bool respectRequirements(Character * character);
    std::list<std::string> and_requirements;
    std::list<std::string> or_requirements;
    std::list<std::string> not_requirements;
};

#endif // _ATTRIBUTES_H_
