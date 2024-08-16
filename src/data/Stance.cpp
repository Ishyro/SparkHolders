#include "data/Stance.h"

#include "data/actions/Action.h"
#include "data/skills/Skill.h"

Skill * Stance::getAttack(int32_t indice) { return attack_skills[indice]; }
Skill * Stance::getBlock() { return block_skill; }

bool Stance::isValid(int32_t weapon_type) {
  for(int32_t type : weapon_types) {
    if(type == weapon_type) {
      return true;
    }
  }
  return false;
}
