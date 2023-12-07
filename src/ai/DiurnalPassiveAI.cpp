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

#include "ai/DiurnalPassiveAI.h"

Action * DiurnalPassiveAI::getActions(Adventure * adventure, Character * c) {
  return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
  /*
  Map * visionMap = updateMap(adventure, c);
  Action * result;
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 3);
  float orientation = 0.F;
  if(!threats.empty()) {
    Character * target = threats.front();
    orientation = getFleeOrientation(adventure, c, target->getX(), target->getY());
    /*MapUtil::Pair pair = MapUtil::getNextPairFromOrientation(orientation, c->getX(), c->getY());
    if(!adventure->getWorld()->getMap(c->getCurrentMap()->id)->getBlock(pair.x, pair.y)->unwalkable) {
      //return new Action(MOVE, adventure, nullptr, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    //}
    //else {
      // orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
      Target * t = new Target();
      t->type = TARGET_TILE;
      t->id = c->getCurrentMap()->id;
      t->x = origin_x;
      t->y = origin_y;
      result = new TargetedAction(ACTION_MOVE, adventure, nullptr, c, t);
      c->setCurrentAction(result);
      return result;
    //}
  }
  selectHungriness(c);
  selectTiredness(c);
  if(hungry) {
    Action * eat_food = eat(adventure, c);
    if(eat_food != nullptr) {
      result = eat_food;
      c->setCurrentAction(result);
      return result;
    }
  }
  if(sleepy && adventure->getLight() < 4) {
    result = new BaseAction(ACTION_IDLE, adventure, nullptr, c);
    c->setCurrentAction(result);
    return result;
  }
  orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
  if(orientation != 360.F) {
    Target * t = new Target();
    t->type = TARGET_TILE;
    t->id = c->getCurrentMap()->id;
    t->x = origin_x;
    t->y = origin_y;
    result = new TargetedAction(ACTION_MOVE, adventure, nullptr, c, t);
    c->setCurrentAction(result);
    return result;
  }
  result = new BaseAction(ACTION_IDLE, adventure, nullptr, c);
  c->setCurrentAction(result);
  return result;
  */
}
