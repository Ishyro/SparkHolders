#include "data/Adventure.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/Speech.h"
#include "data/Block.h"
#include "data/World.h"
#include "data/Database.h"

#include "data/skills/Skill.h"

#include "data/actions/SkillAction.h"

#include "data/ways/Way.h"
#include "data/ways/Attributes.h"

#include "util/MathUtil.h"
#include "util/String.h"

Action * SkillAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != ACTION_BREAKPOINT) {
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
  else {
    if(next != nullptr) {
      next->setPrevious(nullptr);
      // tick is in range [0;1]
      next->computeTick(1 - tick);
      user->setCurrentAction(next);
    }
    else {
      user->setCurrentAction(nullptr);
    }
  }
  return next;
}

void SkillAction::computeTime(Adventure * adventure) {
  time = (float) skill->time / user->getSkillTimeModifier();
  tick = time;
}

Skill * SkillAction::getSkill() { return skill; }
