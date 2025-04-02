#include "data/skills/BlockSwapSkill.h"

void BlockSwapSkill::activate(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range, bool toggle_state) {
  // MathUtil::Target will always be a Block
  int32_t lightening = adventure->getWorld()->getLightening(MathUtil::Vector3i(target->coord));
  // TODO
  // doesn't work when there wasn't a block (lightening == LIGHTENING_NO)
  adventure->getWorld()->setBlock(MathUtil::Vector3i(target->coord), new_block, lightening);
}
bool BlockSwapSkill::canCast(Character * owner, MathUtil::Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range) {
  return current_block == nullptr || adventure->getWorld()->getBlock(MathUtil::Vector3i(target->coord)) == current_block;
}

float BlockSwapSkill::getPower() {
  return 0.F;
}

float BlockSwapSkill::getDamageFromType(int32_t damage_type, Character * owner, float overcharge) {
  return 0.F;
}

Block * BlockSwapSkill::getCurrentBlock() { return current_block; }
Block * BlockSwapSkill::getNewBlock() { return new_block; }
