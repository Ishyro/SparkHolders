#include "data/skills/BlockSwapSkill.h"

void BlockSwapSkill::activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  // Target will always be a Block
  adventure->getWorld()->setBlock(MathUtil::makeVector3i(target->coord), new_block);
}
bool BlockSwapSkill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range) {
  return current_block == nullptr || adventure->getWorld()->getBlock(MathUtil::makeVector3i(target->coord)) == current_block;
}

int32_t BlockSwapSkill::getPower() {
  return 0;
}

int32_t BlockSwapSkill::getDamageFromType(int32_t damage_type, int32_t overcharge_power) {
  return 0;
}

float BlockSwapSkill::getDamageReductionFromType(int32_t damage_type, int32_t overcharge_power) {
  return 0.;
}

Block * BlockSwapSkill::getCurrentBlock() { return current_block; }
Block * BlockSwapSkill::getNewBlock() { return new_block; }
