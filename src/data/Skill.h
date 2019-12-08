#ifndef _SKILL_H_
#define _SKILL_H_

#include <list>

#include "src/Values.h"
#include "src/data/World.h"
#include "src/data/Character.h"
#include "src/data/Effect.h"

class Skill {
  public:
    const std::string name;
    const int type;
    const int mana_cost;
    const int overcharge_type;
    const std::list<Effect *> effects;
    const std::list<int> specials;
    Skill(std::string name) {
      // TODO XML_READER
    }
    void activate(Character * owner, World * world, int overcharge);
    void desactivate(Character * owner, World * world);
    int drain();
  private:
    int tick;
    int current_mana_cost;
    bool active;
};

#endif // _SKILL_H_
