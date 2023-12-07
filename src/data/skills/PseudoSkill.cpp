#include "data/skills/PseudoSkill.h"

#include "data/skills/ChanneledSkill.h"
#include "data/skills/MapLinkerSkill.h"
#include "data/skills/MindControlSkill.h"
#include "data/skills/ProjectileSkill.h"
#include "data/skills/ResurrectionSkill.h"
#include "data/skills/SimpleSkill.h"
#include "data/skills/SummonSkill.h"
#include "data/skills/TeamChangerSkill.h"
#include "data/skills/TeleportSkill.h"
#include "data/skills/BlockSwapSkill.h"

#include "util/String.h"

int PseudoSkill::getManaCost(int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range) {
  int cost = 0;
  switch(overcharge_power_type) {
    case OVERCHARGE_NONE:
      cost += mana_cost;
      break;
    case OVERCHARGE_LINEAR:
      cost += mana_cost * overcharge_power;
      break;
    case OVERCHARGE_SQUARE:
      cost += mana_cost * overcharge_power * overcharge_power;
      break;
    case OVERCHARGE_EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge_power - 1);
      break;
    default:
      ;
  }
  switch(overcharge_duration_type) {
    case OVERCHARGE_NONE:
      cost += mana_cost;
      break;
    case OVERCHARGE_LINEAR:
      cost += mana_cost * overcharge_duration;
      break;
    case OVERCHARGE_SQUARE:
      cost += mana_cost * overcharge_duration * overcharge_duration;
      break;
    case OVERCHARGE_EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge_duration - 1);
    default:
      ;
  }
  switch(overcharge_range_type) {
    case OVERCHARGE_NONE:
      cost += mana_cost;
      break;
    case OVERCHARGE_LINEAR:
      cost += mana_cost * overcharge_range;
      break;
    case OVERCHARGE_SQUARE:
      cost += mana_cost * overcharge_range * overcharge_range;
      break;
    case OVERCHARGE_EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge_range - 1);
      break;
    default:
      ;
  }
  return cost - 2 * mana_cost;
}
