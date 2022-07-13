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

int PseudoSkill::getManaCost(int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range) {
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
  switch(overcharge_range_type) {
    case NO_OVERCHARGE:
      cost += mana_cost;
      break;
    case LINEAR:
      cost += mana_cost * overcharge_range;
      break;
    case SQUARE:
      cost += mana_cost * overcharge_range * overcharge_range;
      break;
    case EXPONENTIAL:
      cost += mana_cost * (int) std::pow(2. , (double) overcharge_range - 1);
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
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  switch(skill_type) {
    case PROJECTILE_SKILL:
      String::insert(ss, ((ProjectileSkill *) this)->getProjectile()->full_to_string());
      break;
    default:
      ;
  }
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
  std::stringstream * ss_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * effects = new std::list<Effect *>();
  while(ss_effects->rdbuf()->in_avail() != 0) {
    effects->push_back(Effect::from_string(String::extract(ss_effects)));
  }
  delete ss_effects;
  PseudoSkill * result = nullptr;
  switch(skill_type) {
    case CHANNELED_SKILL:
      result = new ChanneledSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    case INSTANT_SKILL:
      result = new InstantSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    case MAP_LINKER_SKILL:
      result = new MapLinkerSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    case MIND_CONTROL_SKILL:
      result = new MindControlSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    case PROJECTILE_SKILL:
      result = new ProjectileSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      ((ProjectileSkill *) result)->setProjectile(Projectile::full_from_string(String::extract(ss)));
      break;
    case RESURRECTION_SKILL:
      result = new ResurrectionSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    case SIMPLE_SKILL:
      result = new SimpleSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    case SUMMON_SKILL:
      result = new SummonSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    case TEAM_CHANGER_SKILL:
      result = new TeamChangerSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    case TELEPORT_SKILL:
      result = new TeleportSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    case TILE_SWAP_SKILL:
      result = new TileSwapSkill(
        name,
        skill_type,
        target_type,
        mana_cost,
        *effects
      );
      break;
    default:
      ;
  }
  delete ss;
  return result;
}
