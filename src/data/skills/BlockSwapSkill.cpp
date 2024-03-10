#include "data/skills/BlockSwapSkill.h"

void BlockSwapSkill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  // Target will always be a Block
  adventure->getWorld()->setBlock(MapUtil::makeVector3i(target->coord), new_block);
}
bool BlockSwapSkill::canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  return current_block == nullptr || adventure->getWorld()->getBlock(MapUtil::makeVector3i(target->coord)) == current_block;
}

int BlockSwapSkill::getPower() {
  return 0;
}

int BlockSwapSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float BlockSwapSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}

Block * BlockSwapSkill::getCurrentBlock() { return current_block; }
Block * BlockSwapSkill::getNewBlock() { return new_block; }
