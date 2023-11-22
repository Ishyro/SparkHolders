#ifndef _WAY_H_
#define _WAY_H_

#include <list>
#include <string>

#include "Values.h"

class Way {
  public:
    const std::string name;
    const int type;
    const int tier;
    const int baseHp;
    const int baseMana;
    const int baseShield;
    const int baseDamageMult;
    const int baseSoulBurn;
    const int baseFlow;
    const int baseVisionRange;
    const int baseVisionPower;
    const int baseDetectionRange;
    const int hpIncr;
    const int manaIncr;
    const int shieldIncr;
    const int damageMultIncr;
    const int soulBurnIncr;
    const int flowIncr;
    Way(
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
