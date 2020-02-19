#ifndef _TELEPORTSKILL_H_
#define _TELEPORTSKILL_H_

#include "src/data/skills/PseudoSkill.h"

class TeleportSkill : public PseudoSkill {
  public:
    TeleportSkill(std::string name):PseudoSkill(name) {
      // TODO XML_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
    void desactivate(Character * owner, Character * target, Adventure * adventure);
};

#endif // _TELEPORTSKILL_H_
