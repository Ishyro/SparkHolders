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

#include "ai/GuardAI.h"

Action * GuardAI::getActions(Adventure * adventure, Character * c) {
  Map * visionMap = updateMap(adventure, c);
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 5);
  float orientation = 0.F;
  if(!threats.empty() && visionMap->getTile(origin_x - visionMap->offsetX, origin_y - visionMap->offsetY)->name != "TXT_MIST") {
    return attack(adventure, threats, c);
  }
  selectHungriness(c);
  selectTiredness(c);
  if(hungry) {
    Action * eat_food = eat(adventure, c);
    if(eat_food != nullptr) {
      return eat_food;
    }
  }
  if(sleepy && adventure->getLight() < 4) {
    return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
  }
  orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
  if(orientation != 360.F) {
    Target * t = new Target();
    t->type = TARGET_TILE;
    t->id = c->getCurrentMap()->id;
    t->x = origin_x;
    t->y = origin_y;
    return new TargetedAction(ACTION_MOVE, adventure, nullptr, c, t);
  }
  return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
}
