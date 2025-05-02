#include "data/Adventure.h"
#include "data/Character.h"
#include "data/World.h"

#include "data/actions/Action.h"
#include "data/actions/BaseAction.h"
#include "data/actions/EconomicsAction.h"
#include "data/actions/GearAction.h"
#include "data/actions/SkillAction.h"
#include "data/actions/TalkingAction.h"
#include "data/actions/TargetedAction.h"

#include "ai/AI.h"

#include "ai/EtheralCasterAI.h"

Action * EtheralCasterAI::getActions(Adventure * adventure, Character * c) {
  return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
  /*
  Action * result;
  if(casted) {
    adventure->getWorld()->getMap(c->getCurrentMap()->id)->killCharacter(c, c);
  }
  if(c->getX() != origin_x || c->getY() != origin_y) {
    MathUtil::Target * t = new MathUtil::Target();
    t->type = TARGET_COORDINATES;
    t->id = c->getCurrentMap()->id;
    t->x = origin_x;
    t->y = origin_y;
    result = new TargetedAction(ACTION_MOVE, adventure, nullptr, c, t);
    c->setAction(result);
    return result;
  }
  if(++current_round < target_round) {
    result = new BaseAction(ACTION_IDLE, adventure, nullptr, c);
    c->setAction(result);
    return result;
  }
  else {
    casted = true;
    MathUtil::Target * target = new MathUtil::Target();
    target->type = TARGET_COORDINATES;
    target->id = c->getCurrentMap()->id;
    target->x = origin_x;
    target->y = origin_y;
    // EhteralCasters should have only one skill
    result = new SkillAction(ACTION_USE_SKILL, adventure, nullptr, c, target, c->getSkills().front(), 1, 1, 1);
    c->setAction(result);
    return result;
  }
  */
}
