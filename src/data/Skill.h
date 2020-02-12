#ifndef _SKILL_H_
#define _SKILL_H_

#include <list>
#include <cmath>

#include "src/Values.h"
#include "src/data/Adventure.h"
#include "src/data/Character.h"
#include "src/data/Effect.h"

class Skill {
  public:
    const std::string name;
    const int cast_type;
    const int target_type;
    const int overcharge_type;
    const long mana_cost;
    const std::list<Effect *> effects;
    const int special;
    const int range;
    const Tile * tile;
    const int teleport_type;
    Skill(std::string name) {
      // TODO XML_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, int overcharge, long map_id = 0L, long x = 0L, long y = 0L);
    void desactivate(Character * owner, Character * target, Adventure * adventure);
    long getManaCost(int overcharge);
    long drain();
  private:
    int tick;
    long current_mana_cost;
    bool active;
};

#endif // _SKILL_H_
