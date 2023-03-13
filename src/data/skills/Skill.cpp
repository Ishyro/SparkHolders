#include "data/skills/Skill.h"

#include "data/skills/TileSwapSkill.h"

#include "utils/String.h"

void Skill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y) {
  owner->payMana(getManaCost(overcharge_power, overcharge_duration, overcharge_range));
  for(PseudoSkill * skill : skills) {
    skill->activate(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, map_id, x, y, range);
  }
}
bool Skill::canCast(Character * owner, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range, int map_id, int x, int y) {
  // using mana beyond flow is forbidden for instant skills
  if(is_instant) {
    if(getManaCost(overcharge_power, overcharge_duration, overcharge_range) > owner->getAvaillableMana(false)) {
      return false;
    }
  }
  else {
    if(getManaCost(overcharge_power, overcharge_duration, overcharge_range) >= owner->getAvaillableMana(true)) {
      return false;
    }
  }
  for(PseudoSkill * skill : skills) {
    if(!skill->canCast(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, map_id, x, y, range)) {
      return false;
    }
  }
  return true;
}

int Skill::getManaCost(int overcharge_power, int overcharge_duration, int overcharge_range) {
  int mana_cost = 0;
  for(PseudoSkill * skill : skills) {
    mana_cost += skill->getManaCost(overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range);
  }
  return mana_cost;
}

int Skill::getPower() {
  int power = 0;
  for(PseudoSkill * skill : skills) {
    power += skill->getPower();
  }
  return power;
}

int Skill::getDamageFromType(int damage_type, int overcharge_power) {
  int damage = 0;
  for(PseudoSkill * s : skills) {
    damage += s->getDamageFromType(damage_type, overcharge_power);
  }
  return damage;
}

float Skill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  float reduction = 0.;
  for(PseudoSkill * s : skills) {
    reduction += s->getDamageReductionFromType(damage_type, overcharge_power);
  }
  return reduction;
}

Tile * Skill::isEatingSkill() {
  bool is_eating_skill = false;
  Tile * target = nullptr;
  for(PseudoSkill * s : skills) {
    if(s->skill_type == TILE_SWAP_SKILL) {
      target = ( (TileSwapSkill *) s)->getCurrentTile();
    }
    if(s->skill_type == SIMPLE_SKILL) {
      for(Effect * effect : s->effects) {
        if(effect->type == SATIETY) {
          is_eating_skill = true;
        }
      }
    }
  }
  if(is_eating_skill) {
    return target;
  }
  return nullptr;
}


int Skill::isTeleportSkill() {
  for(PseudoSkill * s : skills) {
    if(s->skill_type == TELEPORT_SKILL) {
      return range;
    }
  }
  return 0;
}

std::string Skill::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, level);
  String::insert(ss, attributes);
  String::insert_int(ss, target_type);
  String::insert_bool(ss, is_instant);
  String::insert_int(ss, overcharge_power_type);
  String::insert_int(ss, overcharge_duration_type);
  String::insert_int(ss, overcharge_range_type);
  String::insert_int(ss, range);
  String::insert_int(ss, time);
  std::stringstream * ss_skills = new std::stringstream();
  for(PseudoSkill * skill : skills) {
    String::insert(ss_skills, skill->to_string());
  }
  String::insert(ss, ss_skills->str());
  delete ss_skills;
  std::string result = ss->str();
  delete ss;
  return result;
}

Skill * Skill::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int level = String::extract_int(ss);
  std::string attributes = String::extract(ss);
  int target_type = String::extract_int(ss);
  bool is_instant = String::extract_bool(ss);
  int overcharge_power_type = String::extract_int(ss);
  int overcharge_duration_type = String::extract_int(ss);
  int overcharge_range_type = String::extract_int(ss);
  int range = String::extract_int(ss);
  int time = String::extract_int(ss);
  std::stringstream * ss_skills = new std::stringstream(String::extract(ss));
  std::list<PseudoSkill *> * skills = new std::list<PseudoSkill *>();
  while(ss_skills->rdbuf()->in_avail() != 0) {
    skills->push_back(PseudoSkill::from_string(String::extract(ss_skills)));
  }
  delete ss_skills;
  delete ss;
  Skill * result = new Skill(
    name,
    level,
    attributes,
    target_type,
    is_instant,
    overcharge_power_type,
    overcharge_duration_type,
    overcharge_range_type,
    range,
    time,
    *skills
  );
  delete skills;
  return result;
}
