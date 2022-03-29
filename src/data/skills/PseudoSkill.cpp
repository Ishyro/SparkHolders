#include "data/skills/PseudoSkill.h"

long PseudoSkill::getManaCost(long overcharge) {
  switch(overcharge_type) {
    case NO_OVERCHARGE:
      return mana_cost;
    case LINEAR:
      return mana_cost * overcharge;
    case SQUARE:
      return mana_cost * overcharge * overcharge;
    case EXPONENTIAL:
      return mana_cost * (long) std::pow(2. , (double) overcharge);
  }
}
