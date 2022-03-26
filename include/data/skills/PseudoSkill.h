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
    const int cast_type;
    const int target_type;
    const long overcharge_type;
    const long mana_cost;
    const std::list<Effect *> effects;
    const int range;
    PseudoSkill(std::string name) {
      // TODO XML_READER
    }
    virtual void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L) = 0;
    virtual void desactivate(Character * owner, Character * target, Adventure * adventure);
    long getManaCost(long overcharge);
    long drain();
  private:
    int tick;
    long current_mana_cost;
    long overcharge;
    bool active;
};

#endif // _PSEUDOSKILL_H_
