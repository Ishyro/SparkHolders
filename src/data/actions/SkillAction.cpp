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
  
  switch(type) {
    case ACTION_ATTACK:
      skill = user->getAttack();
      if(skill->canCast(user, target, adventure, mana_cost)) {
        user->useSkill(skill, target, adventure, mana_cost);
        user->incrAttack();
      }
      break;
    case ACTION_BLOCK:
      skill = user->getDefense();
      if(skill->canCast(user, target, adventure, mana_cost)) {
        user->useSkill(skill, target, adventure, mana_cost);
      }
      break;
    case ACTION_USE_SKILL:
      if(user->hasSkill(skill) && skill->canCast(user, target, adventure, mana_cost)) {
        user->useSkill(skill, target, adventure, mana_cost);
      }
      break;
  }
  if(previous != nullptr) {
    previous->setNext(next);
    if(next != nullptr) {
      next->setPrevious(previous);
    }
  }
  else {
    if(next != nullptr) {
      next->setPrevious(nullptr);
      // tick is in range [0;1]
      next->computeTick(1 - tick);
    }
    user->setAction(next);
  }
  return next;
}

void SkillAction::computeTime(Adventure * adventure) {
  if(type == ACTION_ATTACK) {
    skill = user->getAttack();
  }
  else if(type == ACTION_BLOCK) {
    skill = user->getDefense();
  }
  time = (float) skill->getTime(user) / user->getSkillTimeModifier();
  tick = time;
}

Skill * SkillAction::getSkill() { return skill; }
