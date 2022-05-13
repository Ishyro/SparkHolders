#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "ai/AI.h"

#include "ai/NocturnalAgressiveAI.h"

Action * NocturnalAgressiveAI::getAction(Adventure * adventure, Character * c) {
  if(adventure->getLight() <= 6) {
    Map * map = adventure->getWorld()->getMap(c->getCurrentMapId());
    int orientation = getFollowOrientation(c, origin_x, origin_y);
    Character * threat;
    int distance_threat = 100;
    for(Character * other : map->getCharacters()) {
      if(other->getTeam() != c->getTeam()) {
        int distance = std::max(abs(c->getX() - other->getX()), abs(c->getY() - other->getY()));
        if(distance <= c->getVisionRange() && distance < distance_threat) {
          threat = other;
          distance_threat = distance;
          orientation = getFollowOrientation(c, other->getX(), other->getY());
        }
      }
    }
    return new Action(MOVE, c, orientation, nullptr, nullptr, 0, 0, nullptr, "");
  } else {
    return new Action(REST, c, 0, nullptr, nullptr, 0, 0, nullptr, "");
  }
  return nullptr;
}
