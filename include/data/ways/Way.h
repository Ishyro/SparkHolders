#ifndef _WAY_H_
#define _WAY_H_

#include <list>
#include <string>

#include "Values.h"

class Way {
  public:
    const std::string name;
    const int32_t type;
    const int32_t tier;
    const int32_t baseHp;
    const int32_t baseMana;
    const int32_t baseShield;
    const int32_t baseDamageMult;
    const int32_t baseSoulBurn;
    const int32_t baseFlow;
    const int32_t baseVisionRange;
    const int32_t baseVisionPower;
    const int32_t baseDetectionRange;
    const int32_t hpIncr;
    const int32_t manaIncr;
    const int32_t shieldIncr;
    const int32_t damageMultIncr;
    const int32_t soulBurnIncr;
    const int32_t flowIncr;
    Way(
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
      std::list<std::string> tags
    ):
      name(name),
      type(type),
      tier(tier),
      baseHp(baseHp),
      baseMana(baseMana),
      baseShield(baseShield),
      baseDamageMult(baseDamageMult),
      baseSoulBurn(baseSoulBurn),
      baseFlow(baseFlow),
      baseVisionRange(baseVisionRange),
      baseVisionPower(baseVisionPower),
      baseDetectionRange(baseDetectionRange),
      hpIncr(hpIncr),
      manaIncr(manaIncr),
      shieldIncr(shieldIncr),
      damageMultIncr(damageMultIncr),
      soulBurnIncr(soulBurnIncr),
      flowIncr(flowIncr),
      effects(effects),
      skills(skills),
      tags(tags)
    {}
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    std::list<std::string> getTags();
    bool hasTag(std::string tag);
    bool operator == (const Way& w) const { return name == w.name; }
    bool operator != (const Way& w) const { return !operator==(w); }
  protected:
    std::list<Effect *> effects;
    std::list<Skill *> skills;
    std::list<std::string> tags;
};

#endif // _WAY_H_
