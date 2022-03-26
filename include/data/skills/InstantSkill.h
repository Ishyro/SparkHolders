#ifndef _INSTANTSKILL_H_
#define _INSTANTSKILL_H_

#include "data/skills/PseudoSkill.h"

class InstantSkill : public PseudoSkill {
  public:
    InstantSkill(std::string name):PseudoSkill(name) {
      // TODO XML_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
};

#endif // _INSTANTSKILL_H_
