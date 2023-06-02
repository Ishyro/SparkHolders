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

#include "ai/NocturnalAgressiveAI.h"

Action * NocturnalAgressiveAI::getActions(Adventure * adventure, Character * c) {
  Map * visionMap = new Map(adventure->getWorld()->getMap(c->getCurrentMapId()), c, adventure->getDatabase());
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 5);
  float orientation = 0.F;
  if(!threats.empty()) {
    Character * target = threats.front();
    // orientation = getFollowOrientation(adventure, c, target->getX(), target->getY());
    delete visionMap;
    Target * t = new Target();
    t->type = TARGET_CHARACTER;
    t->id = c->getCurrentMapId();
    t->x = target->getX();
    t->y = target->getY();
    return new TargetedAction(ACTION_MOVE, adventure, nullptr, c, t);
  }
  selectHungriness(c);
  selectTiredness(c);
  if(hungry) {
    Action * eat_food = eat(adventure, c);
    if(eat_food != nullptr) {
      delete visionMap;
      return eat_food;
    }
  }
  if(sleepy && adventure->getLight() > 6) {
    delete visionMap;
    return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
  }
  orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
  if(orientation != 360.F) {
    delete visionMap;
    Target * t = new Target();
    t->type = TARGET_TILE;
    t->id = c->getCurrentMapId();
    t->x = origin_x;
    t->y = origin_y;
    return new TargetedAction(ACTION_MOVE, adventure, nullptr, c, t);
  }
  delete visionMap;
    return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
}
