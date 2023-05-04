#include "data/skills/TileSwapSkill.h"

void TileSwapSkill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  // Target will always be a Tile
  adventure->getWorld()->getMap(target->id)->setTile((int) target->y, (int) target->x, new_tile);
}
bool TileSwapSkill::canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  return current_tile == nullptr || adventure->getWorld()->getMap(target->id)->getTile((int) target->y, (int) target->x) == current_tile;
}

int TileSwapSkill::getPower() {
  return 0;
}

int TileSwapSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float TileSwapSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}

Tile * TileSwapSkill::getCurrentTile() { return current_tile; }
Tile * TileSwapSkill::getNewTile() { return new_tile; }

void TileSwapSkill::setCurrentTile(Tile * current_tile) { this->current_tile = current_tile; }
void TileSwapSkill::setNewTile(Tile * new_tile) { this->new_tile = new_tile; }
