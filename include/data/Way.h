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
    // race only
    const float size;
    const bool need_to_eat;
    const bool can_eat_food;
    const bool need_to_sleep;
    Way(
      std::string name,
      int type,
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
      float size,
      bool need_to_eat,
      bool can_eat_food,
      bool need_to_sleep,
      std::list<Effect *> effects,
      std::list<Skill *> skills
    ):
      name(name),
      type(type),
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
      size(size),
      need_to_eat(need_to_eat),
      can_eat_food(can_eat_food),
      need_to_sleep(need_to_sleep),
      effects(effects),
      skills(skills)
    {}
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    std::string to_string();
    static Way * from_string(std::string to_read);
    bool operator == (const Way& w) const { return name == w.name; }
    bool operator != (const Way& w) const { return !operator==(w); }
  private:
    std::list<Effect *> effects;
    std::list<Skill *> skills;
};

#endif // _WAY_H_
