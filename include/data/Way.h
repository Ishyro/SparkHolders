#ifndef _WAY_H_
#define _WAY_H_

#include <list>
#include <string>

#include "Values.h"

#include "data/skills/Skill.h"
#include "data/Event.h"

#include "utils/FileOpener.h"

class Way {
  public:
    const std::string name;
    const int type;
    const int hpIncr;
    const int manaIncr;
    const int armorIncr;
    const int soulBurnIncr;
    const int flowIncr;
    Way(
      std::string name,
      int type,
      int hpIncr,
      int manaIncr,
      int armorIncr,
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
      soulBurnIncr(soulBurnIncr),
      flowIncr(flowIncr),
      effects(effects),
      skills(skills)
    {}
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
  private:
    std::list<Effect *> effects;
    std::list<Skill *> skills;
};

#endif // _WAY_H_
