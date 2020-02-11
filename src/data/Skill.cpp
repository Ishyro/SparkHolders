#include "src/data/Skill.h"

void Skill::activate(Character * target, Adventure * adventure, int overcharge, long map_id, long x, long y) {
  current_mana_cost = getManaCost(overcharge);
  if (cast_type == TOGGLE) {
    active = true;
  }
  switch(special) {
    case TIMESTOP:
      break;
    case TILESWAP:
      adventure->getWorld()->getMap(map_id)->setTile(x, y, tile);
      break;
    case TELEPORT:
      if (teleport_type == SOFT) {
        adventure->softMoveCharacterToMap(target, map_id, x, y);
      }
      else if (teleport_type == HARD) {
        adventure->hardMoveCharacterToMap(target, map_id, x, y);
      }
      break;
    case MAPLINKER:
      break;
    case TEAMCHANGER:
      break;
    case MINDCONTROL:
      break;
    case PROJECTILE:
      break;
    case SUMMON:
      break;
    case RESURRECTION:
      break;
    default:
      break;
  }
}

void Skill::desactivate(Character * target, Adventure * adventure) {
  current_mana_cost = 0L;
  active = false;
}

long Skill::getManaCost(int overcharge) {
  switch(overcharge_type) {
    case NO_OVERCHARGE:
      return mana_cost;
    case LINEAR:
      return mana_cost * overcharge;
    case EXPONENTIAL:
      return mana_cost * (long) std::pow(2. , (double) overcharge);
  }
}

long Skill::drain() {
  if (cast_type == TOGGLE && active) {
    return current_mana_cost;
  }
  else {
    return 0L;
  }
}
