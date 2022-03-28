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
    Skill(
      std::string name,
      int cast_type,
      int target_type,
      long overcharge_type,
      long mana_cost,
      std::list<PseudoSkill *> effects,
      int range,
      int tick,
      long current_mana_cost,
      long overcharge
    ):
      name(name),
      cast_type(cast_type),
      target_type(target_type),
      overcharge_type(overcharge_type),
      mana_cost(mana_cost),
      effects(effects),
      range(range),
      tick(tick),
      current_mana_cost(current_mana_cost),
      overcharge(overcharge)
    {
      active=false;
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
