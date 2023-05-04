#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
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
  if(casted) {
    adventure->getWorld()->getMap(c->getCurrentMapId())->killCharacter(c, c);
  }
  if(c->getX() != origin_x || c->getY() != origin_y) {
    Action * action = new TargetedAction(MOVE, adventure, nullptr, c);
    Target * t = new Target();
    t->type = TILE;
    t->id = c->getCurrentMapId();
    t->x = origin_x;
    t->y = origin_y;
    ((TargetedAction *) action)->setTarget(t);
    return action;
  }
  if(++current_round < target_round) {
    return new BaseAction(IDLE, adventure, nullptr, c);
  }
  else {
    casted = true;
    Action * action = new SkillAction(USE_SKILL, adventure, nullptr, c);
    Target * target = new Target();
    target->type = TILE;
    target->id = c->getCurrentMapId();
    target->x = origin_x;
    target->y = origin_y;
    ((SkillAction *) action)->setTarget(target);
    // EhteralCasters should have only one skill
    ((SkillAction *) action)->setSkill(c->getSkills().front());
    ((SkillAction *) action)->setOverchargePower(1);
    ((SkillAction *) action)->setOverchargeRange(1);
    ((SkillAction *) action)->setOverchargeDuration(1);
    return action;
  }
}
