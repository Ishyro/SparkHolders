#ifndef _ATTRIBUTES_H_
#define _ATTRIBUTES_H_

#include "data/ways/Way.h"

#include <list>
#include <string>

class Attributes : public Way {
  public:
    Attributes(
      std::string name,
      int32_t type,
      int32_t tier,
      int32_t baseHp,
      int32_t baseMana,
      int32_t baseShield,
      int32_t baseDamageMult,
      int32_t baseSoulBurn,
      int32_t baseFlow,
      int32_t baseVisionRange,
      int32_t baseVisionPower,
      int32_t baseDetectionRange,
      int32_t hpIncr,
      int32_t manaIncr,
      int32_t shieldIncr,
      int32_t damageMultIncr,
      int32_t soulBurnIncr,
      int32_t flowIncr,
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
