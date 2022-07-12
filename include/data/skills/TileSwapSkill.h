#ifndef _TILE_SWAP_SKILL_H_
#define _TILE_SWAP_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TileSwapSkill : public PseudoSkill {
  using PseudoSkill::PseudoSkill;
  public:
    void activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge_power, int overcharge_duration, int overcharge_area, int map_id = 0, int x = 0, int y = 0);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
  private:
    Tile * tile;
};

#endif // _TILE_SWAP_SKILL_H_
