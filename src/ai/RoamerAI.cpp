#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "ai/AI.h"

#include "ai/RoamerAI.h"

#include <random>

Action * RoamerAI::getAction(Adventure * adventure, Character * c) {
  Map * visionMap = new Map(adventure->getWorld()->getMap(c->getCurrentMapId()), c, adventure->getDatabase());
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 5);
  int orientation = NO_ORIENTATION;
  if(!threats.empty()) {
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
  if(orientation != NO_ORIENTATION) {
    delete visionMap;
    return new Action(MOVE, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  // we are at destination
  origin_x = rand() % adventure->getWorld()->getMap(c->getCurrentMapId())->sizeX;
  origin_y = rand() % adventure->getWorld()->getMap(c->getCurrentMapId())->sizeY;
  delete visionMap;
  return new Action(REST, c, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
}
