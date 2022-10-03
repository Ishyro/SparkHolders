#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "ai/AI.h"

#include "ai/DiurnalPassiveAI.h"

Action * DiurnalPassiveAI::getAction(Adventure * adventure, Character * c) {
  selectHungriness(c);
  selectTiredness(c);
  if(hungry) {
    Action * eat_food = eat(adventure, c);
    if(eat_food != nullptr) {
      return eat_food;
    }
  }
  if(adventure->getLight() >= 4) {
    Map * map = adventure->getWorld()->getMap(c->getCurrentMapId());
    int orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
    Character * threat;
    int distance_threat = 100;
    for(Character * other : map->getCharacters()) {
      if(other->getTeam() != c->getTeam()) {
        int distance = std::max(abs(c->getX() - other->getX()), abs(c->getY() - other->getY()));
        if(distance <= c->getVisionRange() && distance < distance_threat) {
          threat = other;
          distance_threat = distance;
          orientation = getFleeOrientation(adventure, c, other->getX(), other->getY());
        }
      }
    }
    if(orientation != NO_ORIENTATION) {
      return new Action(MOVE, c, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    }
  }
  return new Action(REST, c, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
}
