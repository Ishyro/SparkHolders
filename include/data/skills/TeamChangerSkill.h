#ifndef _TEAMCHANGERSKILL_H_
#define _TEAMCHANGERSKILL_H_

#include "data/skills/PseudoSkill.h"

class TeamChangerSkill : public PseudoSkill {
  public:
    TeamChangerSkill(std::string name):PseudoSkill(name) {
      // TODO INI_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
};

#endif // _TEAMCHANGERSKILL_H_
