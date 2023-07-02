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
    adventure->getWorld()->getMap(c->getCurrentMap()->id)->killCharacter(c, c);
  }
  if(c->getX() != origin_x || c->getY() != origin_y) {
    Target * t = new Target();
    t->type = TARGET_TILE;
    t->id = c->getCurrentMap()->id;
    t->x = origin_x;
    t->y = origin_y;
    return new TargetedAction(ACTION_MOVE, adventure, nullptr, c, t);
  }
  if(++current_round < target_round) {
    return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
  }
  else {
    casted = true;
    Target * target = new Target();
    target->type = TARGET_TILE;
    target->id = c->getCurrentMap()->id;
    target->x = origin_x;
    target->y = origin_y;
    // EhteralCasters should have only one skill
    return new SkillAction(ACTION_USE_SKILL, adventure, nullptr, c, target, c->getSkills().front(), 1, 1, 1);
  }
}
