#ifndef _WAY_H_
#define _WAY_H_

#include <list>
#include <string>

#include "data/skills/Skill.h"
#include "data/Event.h"

class Way {
  public:
    const std::string name;
    const int type;
    const int baseHp;
    const int baseMana;
    const int baseArmor;
    const int baseSoulBurn;
    const int baseFlow;
    Way(std::string name) {
      // TODO XML_READER
    }
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
  private:
    std::list<Effect *> effects;
    std::list<Skill *> skills;
};

#endif // _WAY_H_
