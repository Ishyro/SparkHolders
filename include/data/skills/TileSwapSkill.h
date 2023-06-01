#ifndef _TILE_SWAP_SKILL_H_
#define _TILE_SWAP_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TileSwapSkill : public PseudoSkill {
  public:
    TileSwapSkill(
      std::string name,
      int skill_type,
      int target_type,
      int mana_cost,
      std::list<Effect *> effects,
      Tile * current_tile,
      Tile * new_tile
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, effects),
      current_tile(current_tile),
      new_tile(new_tile)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
    Tile * getCurrentTile();
    Tile * getNewTile();
  private:
    Tile * current_tile;
    Tile * new_tile;
};

#endif // _TILE_SWAP_SKILL_H_
