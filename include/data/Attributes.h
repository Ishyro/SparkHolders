#ifndef _ATTRIBUTES_H_
#define _ATTRIBUTES_H_

#include <list>
#include <string>

#include "data/items/Gear.h"

class Attributes {
  public:
    const std::string name;
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
    Attributes(
      std::string name,
      Attributes * archetype,
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
      std::list<Skill *> skills
    ):
      name(name),
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
      skills(skills)
    {
      init(archetype);
    }
    Attributes * getArchetype();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    bool operator == (const Attributes& a) const { return name == a.name; }
    bool operator != (const Attributes& a) const { return !operator==(a); }
  private:
    Attributes * archetype;
    std::list<Effect *> effects;
    std::list<Skill *> skills;
    void init(Attributes * archetype);
};

#endif // _ATTRIBUTES_H_
