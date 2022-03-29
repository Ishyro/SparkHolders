#ifndef _MINDCONTROLSKILL_H_
#define _MINDCONTROLSKILL_H_

#include "data/skills/PseudoSkill.h"

class MindControlSkill : public PseudoSkill {
  public:
    MindControlSkill(std::string name):PseudoSkill(name) {
      // TODO INI_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
};

#endif // _MINDCONTROLSKILL_H_
