#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "ai/AI.h"

#include "ai/DiurnalAgressiveAI.h"

Action * DiurnalAgressiveAI::getActions(Adventure * adventure, Character * c) {
  Map * visionMap = new Map(adventure->getWorld()->getMap(c->getCurrentMapId()), c, adventure->getDatabase());
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 5);
  float orientation = 0.F;
  if(!threats.empty()) {
    Character * target = threats.front();
    orientation = getFollowOrientation(adventure, c, target->getX(), target->getY());
    delete visionMap;
    return new Action(MOVE, nullptr, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
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
    return new Action(REST, nullptr, c, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
  if(orientation != 360.F) {
    delete visionMap;
    return new Action(MOVE, nullptr, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  delete visionMap;
  return new Action(REST, nullptr, c, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
}
