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
    Attributes(
      std::string name,
      int baseHp,
      int baseMana,
      int baseArmor,
      int baseDamage,
      int baseSoulBurn,
      int baseFlow,
      int baseVisionRange,
      int baseVisionPower,
      int baseDetectionRange,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      Gear * startingGear
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
      effects(effects),
      skills(skills),
      startingGear(startingGear)
    {}
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    Gear * getStartingGear();
    std::string to_string();
    static Attributes * from_string(std::string to_read);
    bool operator == (const Attributes& a) const { return name == a.name; }
    bool operator != (const Attributes& a) const { return !operator==(a); }
  private:
    std::list<Effect *> effects;
    std::list<Skill *> skills;
    Gear * startingGear;
};

#endif // _ATTRIBUTES_H_
