#ifndef _TEAMCHANGERSKILL_H_
#define _TEAMCHANGERSKILL_H_

#include "src/data/skills/PseudoSkill.h"

class TeamChangerSkill : public PseudoSkill {
  public:
    TeamChangerSkill(std::string name):PseudoSkill(name) {
      // TODO XML_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
    void desactivate(Character * owner, Character * target, Adventure * adventure);
};

#endif // _TEAMCHANGERSKILL_H_
