#ifndef _MAPLINKERSKILL_H_
#define _MAPLINKERSKILL_H_

#include "data/skills/PseudoSkill.h"

class MapLinkerSkill : public PseudoSkill {
  public:
    MapLinkerSkill(std::string name):PseudoSkill(name) {
      // TODO INI_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
    void desactivate(Character * owner, Character * target, Adventure * adventure);
};

#endif // _MAPLINKERSKILL_H_
