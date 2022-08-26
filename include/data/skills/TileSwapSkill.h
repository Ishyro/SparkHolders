#ifndef _TILE_SWAP_SKILL_H_
#define _TILE_SWAP_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TileSwapSkill : public PseudoSkill {
  using PseudoSkill::PseudoSkill;
  public:
    void activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range);
    bool canCast(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
    Tile * getCurrentTile();
    Tile * getNewTile();
    void setCurrentTile(Tile * current_tile);
    void setNewTile(Tile * new_tile);
  private:
    Tile * current_tile;
    Tile * new_tile;
};

#endif // _TILE_SWAP_SKILL_H_
