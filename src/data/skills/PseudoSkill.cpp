#include "data/skills/PseudoSkill.h"

#include "data/skills/ChanneledSkill.h"
#include "data/skills/InstantSkill.h"
#include "data/skills/MapLinkerSkill.h"
#include "data/skills/MindControlSkill.h"
#include "data/skills/ProjectileSkill.h"
#include "data/skills/ResurrectionSkill.h"
#include "data/skills/SimpleSkill.h"
#include "data/skills/SummonSkill.h"
#include "data/skills/TeamChangerSkill.h"
#include "data/skills/TeleportSkill.h"
#include "data/skills/TileSwapSkill.h"

#include "utils/String.h"

int PseudoSkill::getManaCost(int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge_power, int overcharge_duration, int overcharge_area) {
  int cost = 0;
  switch(overcharge_power_type) {
    case NO_OVERCHARGE:
      cost += mana_cost;
      break;
    case LINEAR:
      cost += mana_cost * overcharge_power;
      break;
    case SQUARE:
      cost += mana_cost * overcharge_power * overcharge_power;
      break;
    case EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge_power - 1);
      break;
    default:
      ;
  }
  switch(overcharge_duration_type) {
    case NO_OVERCHARGE:
      cost += mana_cost;
      break;
    case LINEAR:
      cost += mana_cost * overcharge_duration;
      break;
    case SQUARE:
      cost += mana_cost * overcharge_duration * overcharge_duration;
      break;
    case EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge_duration - 1);
    default:
      ;
  }
  switch(overcharge_area_type) {
    case NO_OVERCHARGE:
      cost += mana_cost;
      break;
    case LINEAR:
      cost += mana_cost * overcharge_area;
      break;
    case SQUARE:
      cost += mana_cost * overcharge_area * overcharge_area;
      break;
    case EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge_area - 1);
      break;
    default:
      ;
  }
  return cost - 2 * mana_cost;
}

std::string PseudoSkill::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, skill_type);
  String::insert_int(ss, target_type);
  String::insert_int(ss, mana_cost);
  String::insert_int(ss, getPower());
  std::string result = ss->str();
  delete ss;
  return result;
}

PseudoSkill * PseudoSkill::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int skill_type = String::extract_int(ss);
  int target_type = String::extract_int(ss);
  int mana_cost = String::extract_int(ss);
  int power = String::extract_int(ss);
  std::list<Effect *> * effects = new std::list<Effect *>();
  delete ss;
  switch(skill_type) {
    case CHANNELED_SKILL:
      return new ChanneledSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case INSTANT_SKILL:
      return new InstantSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case MAP_LINKER_SKILL:
      return new MapLinkerSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case MIND_CONTROL_SKILL:
      return new MindControlSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case PROJECTILE_SKILL:
      return new ProjectileSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case RESURRECTION_SKILL:
      return new ResurrectionSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case SIMPLE_SKILL:
      return new SimpleSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case SUMMON_SKILL:
      return new SummonSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case TEAM_CHANGER_SKILL:
      return new TeamChangerSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case TELEPORT_SKILL:
      return new TeleportSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    case TILE_SWAP_SKILL:
      return new TileSwapSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects,
        power
      );
    default:
      return nullptr;
  }
}
