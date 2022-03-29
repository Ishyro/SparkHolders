#ifndef _PSEUDOSKILL_H_
#define _PSEUDOSKILL_H_

#include <list>
#include <cmath>

#include "Values.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Effect.h"

class PseudoSkill {
  public:
    const int target_type;
    const long overcharge_type;
    const long mana_cost;
    const std::list<Effect *> effects;
    const int range;
    PseudoSkill(
      int target_type,
      long overcharge_type,
      long mana_cost,
      std::list<Effect *> effects,
      int range
    ):
      target_type(target_type),
      overcharge_type(overcharge_type),
      mana_cost(mana_cost),
      effects(effects),
      range(range)
    {}

    virtual void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L) = 0;
    long getManaCost(long overcharge);
};

#endif // _PSEUDOSKILL_H_
