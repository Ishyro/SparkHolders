#ifndef _WAY_H_
#define _WAY_H_

#include <list>
#include <string>

#include "Values.h"

class Way {
  public:
    const std::string name;
    const int type;
    const int baseHp;
    const int baseMana;
    const int baseArmorMult;
    const int baseDamageMult;
    const int baseSoulBurn;
    const int baseFlow;
    const int baseVisionRange;
    const int baseVisionPower;
    const int baseDetectionRange;
    const int hpIncr;
    const int manaIncr;
    const int armorMultIncr;
    const int damageMultIncr;
    const int soulBurnIncr;
    const int flowIncr;
    Way(
      std::string name,
      int type,
      int baseHp,
      int baseMana,
      int baseArmorMult,
      int baseDamageMult,
      int baseSoulBurn,
      int baseFlow,
      int baseVisionRange,
      int baseVisionPower,
      int baseDetectionRange,
      int hpIncr,
      int manaIncr,
      int armorMultIncr,
      int damageMultIncr,
      int soulBurnIncr,
      int flowIncr,
      std::list<Effect *> effects,
      std::list<Skill *> skills
    ):
      name(name),
      type(type),
      baseHp(baseHp),
      baseMana(baseMana),
      baseArmorMult(baseArmorMult),
      baseDamageMult(baseDamageMult),
      baseSoulBurn(baseSoulBurn),
      baseFlow(baseFlow),
      baseVisionRange(baseVisionRange),
      baseVisionPower(baseVisionPower),
      baseDetectionRange(baseDetectionRange),
      hpIncr(hpIncr),
      manaIncr(manaIncr),
      armorMultIncr(armorMultIncr),
      damageMultIncr(damageMultIncr),
      soulBurnIncr(soulBurnIncr),
      flowIncr(flowIncr),
      effects(effects),
      skills(skills)
    {}
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    bool operator == (const Way& w) const { return name == w.name; }
    bool operator != (const Way& w) const { return !operator==(w); }
  protected:
    std::list<Effect *> effects;
    std::list<Skill *> skills;
    // race only
};

#endif // _WAY_H_
