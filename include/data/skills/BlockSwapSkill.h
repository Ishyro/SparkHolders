#ifndef _TILE_SWAP_SKILL_H_
#define _TILE_SWAP_SKILL_H_

#include "data/skills/PseudoSkill.h"

class BlockSwapSkill : public PseudoSkill {
  public:
    BlockSwapSkill(
      std::string name,
      int skill_type,
      int target_type,
      int mana_cost,
      std::list<Effect *> effects,
      Block * current_block,
      Block * new_block
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, effects),
      current_block(current_block),
      new_block(new_block)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range);
    int getPower();
    int getDamageFromType(int dammage_type, int overcharge_power);
    float getDamageReductionFromType(int dammage_type, int overcharge_power);
    Block * getCurrentBlock();
    Block * getNewBlock();
  private:
    Block * current_block;
    Block * new_block;
};

#endif // _TILE_SWAP_SKILL_H_
