#include "src/data/skills/PseudoSkill.h"

long PseudoSkill::getManaCost(long overcharge) {
  switch(overcharge_type) {
    case NO_OVERCHARGE:
      return mana_cost;
    case LINEAR:
      return mana_cost * (overcharge + 1); // overcharge = 1 is an overcharge
    case EXPONENTIAL:
      return mana_cost * (long) std::pow(2. , (double) overcharge);
  }
}

long PseudoSkill::drain() {
  if (cast_type == TOGGLE && active) {
    return current_mana_cost;
  }
  else {
    return 0L;
  }
}
