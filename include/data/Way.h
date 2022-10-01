#ifndef _WAY_H_
#define _WAY_H_

#include <list>
#include <string>

#include "Values.h"

class Way {
  public:
    const std::string name;
    const int type;
    const int hpIncr;
    const int manaIncr;
    const int armorIncr;
    const int damageIncr;
    const int soulBurnIncr;
    const int flowIncr;
    Way(
      std::string name,
      int type,
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
      type(type),
      hpIncr(hpIncr),
      manaIncr(manaIncr),
      armorIncr(armorIncr),
      damageIncr(damageIncr),
      soulBurnIncr(soulBurnIncr),
      flowIncr(flowIncr),
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
