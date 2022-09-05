#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "ai/AI.h"

#include "ai/EtheralCasterAI.h"

Action * EtheralCasterAI::getAction(Adventure * adventure, Character * c) {
  if(casted) {
    adventure->getWorld()->getMap(c->getCurrentMapId())->killCharacter(c, c);
  }
  if(c->getX() != origin_x || c->getY() != origin_y) {
    return new Action(MOVE, c, getFollowOrientation(c, origin_x, origin_y), nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  if(++current_round < target_round) {
    return new Action(REST, c, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  } else {
    casted = true;
    return new Action(USE_SKILL, c, NO_ORIENTATION, c->getSkills().front(), nullptr, origin_x, origin_y, nullptr, "", 1, 1, 1);
  }
}