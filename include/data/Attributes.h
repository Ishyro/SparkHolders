#ifndef _ATTRIBUTES_H_
#define _ATTRIBUTES_H_

#include <list>
#include <string>

#include "data/Gear.h"

class Attributes {
  public:
    const std::string name;
    const int baseHp;
    const int baseMana;
    const int baseArmor;
    const int baseDamage;
    const int baseSoulBurn;
    const int baseFlow;
    const int baseVisionRange;
    const int baseVisionPower;
    const int baseDetectionRange;
    const int hpIncr;
    const int manaIncr;
    const int armorIncr;
    const int damageIncr;
    const int soulBurnIncr;
    const int flowIncr;
    Attributes(
      std::string name,
      Attributes * archetype,
      int baseHp,
      int baseMana,
      int baseArmor,
      int baseDamage,
      int baseSoulBurn,
      int baseFlow,
      int baseVisionRange,
      int baseVisionPower,
      int baseDetectionRange,
      int hpIncr,
      int manaIncr,
      int armorIncr,
      int damageIncr,
      int soulBurnIncr,
      int flowIncr,
      std::list<Effect *> effects,
      std::list<Skill *> skills
    ):
      name(name),
      baseHp(baseHp),
      baseMana(baseMana),
      baseArmor(baseArmor),
      baseDamage(baseDamage),
      baseSoulBurn(baseSoulBurn),
      baseFlow(baseFlow),
      baseVisionRange(baseVisionRange),
      baseVisionPower(baseVisionPower),
      baseDetectionRange(baseDetectionRange),
      hpIncr(hpIncr),
      manaIncr(manaIncr),
      armorIncr(armorIncr),
      damageIncr(damageIncr),
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
    std::string to_string();
    static Attributes * from_string(std::string to_read);
    bool operator == (const Attributes& a) const { return name == a.name; }
    bool operator != (const Attributes& a) const { return !operator==(a); }
  private:
    Attributes * archetype;
    std::list<Effect *> effects;
    std::list<Skill *> skills;
    void init(Attributes * archetype);
};

#endif // _ATTRIBUTES_H_
