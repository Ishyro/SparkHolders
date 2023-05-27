#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Item.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/Speech.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/Weapon.h"
#include "data/World.h"
#include "data/Database.h"


#include "data/skills/Skill.h"

#include "data/actions/SkillAction.h"

#include "util/MapUtil.h"
#include "util/String.h"

Action * SkillAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != BREAKPOINT) {
      next->execute(adventure);
    }
  }
  setUserOrientationToTarget(adventure);
  if(user->hasSkill( (Skill *) skill) && ( (Skill *) skill)->canCast(user, target, adventure, overcharge_power, overcharge_duration, overcharge_range)) {
    user->useSkill( (Skill *) skill, target, adventure, overcharge_power, overcharge_duration, overcharge_range);
  }
  if(previous != nullptr) {
    previous->setNext(next);
    next->setPrevious(previous);
  }
  else if(next != nullptr) {
    next->setPrevious(nullptr);
    // tick is in range [0;1]
    next->computeTick(1 - tick);
  }
  else {
    user->setNeedToUpdateActions(true);
  }
  return next;
}

void SkillAction::computeTime(Adventure * adventure) {
  time = (float) skill->time / user->getSkillTimeModifier();
  tick = time;
}

Skill * SkillAction::getSkill() { return skill; }
int SkillAction::getOverchargePower() { return overcharge_power; }
int SkillAction::getOverchargeDuration() { return overcharge_duration; }
int SkillAction::getOverchargeRange() { return overcharge_range; }
void SkillAction::setSkill(Skill * skill) { this->skill = skill; }
void SkillAction::setOverchargePower(int overcharge) { overcharge_power = overcharge ;}
void SkillAction::setOverchargeDuration(int overcharge) { overcharge_duration = overcharge ;}
void SkillAction::setOverchargeRange(int overcharge) { overcharge_range = overcharge ;}
