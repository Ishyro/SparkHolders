#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "ai/AI.h"

#include "ai/NocturnalPassiveAI.h"

Action * NocturnalPassiveAI::getAction(Adventure * adventure, Character * c) {
  Map * visionMap = new Map(adventure->getWorld()->getMap(c->getCurrentMapId()), c, adventure->getDatabase());
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 3);
  int orientation = NO_ORIENTATION;
  if(!threats.empty()) {
    Character * target = threats.front();
    orientation = getFleeOrientation(adventure, c, target->getX(), target->getY());
    MapUtil::Pair pair = MapUtil::getNextPairFromOrientation(orientation, c->getX(), c->getY());
    if(!adventure->getWorld()->getMap(c->getCurrentMapId())->getTile(pair.y, pair.x)->untraversable) {
      delete visionMap;
      return new Action(MOVE, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    }
    else {
      orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
      delete visionMap;
      return new Action(MOVE, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    }
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
    return new Action(REST, c, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
  if(orientation != NO_ORIENTATION) {
    delete visionMap;
    return new Action(MOVE, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  delete visionMap;
  return new Action(REST, c, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
}
