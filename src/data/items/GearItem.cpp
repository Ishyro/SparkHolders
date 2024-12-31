#include "data/items/GearItem.h"


float GearItem::getWeight() { return weight; }
int32_t GearItem::getDurability() { return durability; }
bool GearItem::reduceDurability(int32_t damage) { return (durability -= damage) <= 0; }
