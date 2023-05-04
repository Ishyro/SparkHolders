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

#include "ai/NocturnalPassiveAI.h"

Action * NocturnalPassiveAI::getActions(Adventure * adventure, Character * c) {
  Map * visionMap = new Map(adventure->getWorld()->getMap(c->getCurrentMapId()), c, adventure->getDatabase());
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 3);
  float orientation = 0.F;
  if(!threats.empty()) {
    Character * target = threats.front();
    orientation = getFleeOrientation(adventure, c, target->getX(), target->getY());
    /*MapUtil::Pair pair = MapUtil::getNextPairFromOrientation(orientation, c->getX(), c->getY());
    if(!adventure->getWorld()->getMap(c->getCurrentMapId())->getTile(pair.y, pair.x)->untraversable) {*/
      delete visionMap;
      //return new Action(MOVE, adventure, nullptr, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    //}
    //else {
      orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
      delete visionMap;
      Action * action = new TargetedAction(MOVE, adventure, nullptr, c);
      Target * t = new Target();
      t->type = TILE;
      t->id = c->getCurrentMapId();
      t->x = origin_x;
      t->y = origin_y;
      ((TargetedAction *) action)->setTarget(t);
      return action;
    //}
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
    return new BaseAction(IDLE, adventure, nullptr, c);
  }
  orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
  if(orientation != 360.F) {
    delete visionMap;
    Action * action = new TargetedAction(MOVE, adventure, nullptr, c);
    Target * t = new Target();
    t->type = TILE;
    t->id = c->getCurrentMapId();
    t->x = origin_x;
    t->y = origin_y;
    ((TargetedAction *) action)->setTarget(t);
    return action;
  }
  delete visionMap;
    return new BaseAction(IDLE, adventure, nullptr, c);
}
