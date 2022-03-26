#ifndef _SKILL_H_
#define _SKILL_H_

#include <list>

#include "Values.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/skills/PseudoSkill.h"

class Skill {
  public:
    const std::string name;
    const int cast_type;
    const int target_type;
    const long overcharge_type;
    const long mana_cost;
    const std::list<PseudoSkill *> effects;
    const int range;
    Skill(std::string name) {
      // TODO XML_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
    void desactivate(Character * owner, Character * target, Adventure * adventure);
    long getManaCost(long overcharge);
    long drain();
  private:
    int tick;
    long current_mana_cost;
    long overcharge;
    bool active;
};

#endif // _SKILL_H_
