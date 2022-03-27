#ifndef _CHARGESKILL_H_
#define _CHARGESKILL_H_

#include "data/skills/PseudoSkill.h"

class ChargeSkill : public PseudoSkill {
  public:
    ChargeSkill(std::string name):PseudoSkill(name) {
      // TODO INI_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
    void desactivate(Character * owner, Character * target, Adventure * adventure);
  private:
    PseudoSkill * real_skill;
};

#endif // _CHARGESKILL_H_
