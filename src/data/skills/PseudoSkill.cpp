#include "data/skills/PseudoSkill.h"

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

void PseudoSkill::desactivate(Character * owner, Character * target, Adventure * adventure) {
  current_mana_cost = 0L;
  active = false;
  switch(cast_type) {
    case INSTANT:
      break;
    case TOGGLE:
      for (auto effect : effects) {
        target->removeEffect(effect);
      }
      break;
  }
  overcharge = 0L;
}
