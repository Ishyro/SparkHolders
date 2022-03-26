#ifndef _TILESWAPSKILL_H_
#define _TILESWAPSKILL_H_

#include "data/skills/PseudoSkill.h"

class TileSwapSkill : public PseudoSkill {
  public:
    TileSwapSkill(std::string name):PseudoSkill(name) {
      // TODO XML_READER
    }
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
    void desactivate(Character * owner, Character * target, Adventure * adventure);
  private:
    Tile * tile;
};

#endif // _TILESWAPSKILL_H_
