#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "ai/AI.h"

#include "ai/GuardAI.h"

Action * GuardAI::getAction(Adventure * adventure, Character * c) {
  Map * visionMap = new Map(adventure->getWorld()->getMap(c->getCurrentMapId()), c, adventure->getDatabase());
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 5);
  float orientation = 0.F;
  if(!threats.empty() && visionMap->getTile(origin_y - visionMap->offsetY, origin_x - visionMap->offsetX)->name != "TXT_MIST") {
    delete visionMap;
    return attack(adventure, threats, c);
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
  if(sleepy && adventure->getLight() < 4) {
    delete visionMap;
    return new Action(REST, c, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
  if(orientation != 360.F) {
    delete visionMap;
    return new Action(MOVE, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  delete visionMap;
  return new Action(REST, c, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
}
