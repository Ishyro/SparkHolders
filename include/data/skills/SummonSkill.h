#ifndef _SUMMONSKILL_H_
#define _SUMMONSKILL_H_

#include "data/skills/PseudoSkill.h"

class SummonSkill : public PseudoSkill {
  public:
    SummonSkill(std::string name):PseudoSkill(name) {
      // TODO INI_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
};

#endif // _SUMMONSKILL_H_
