#include "data/Adventure.h"
#include "data/Map.h"
#include "data/Region.h"
#include "data/World.h"

#include "data/items/Gear.h"

#include "data/actions/BaseAction.h"
#include "data/actions/OrientedAction.h"

#include "util/MathUtil.h"

Action * OrientedAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != ACTION_BREAKPOINT) {
      next->execute(adventure);
    }
  }
  switch(type) {
    case ACTION_MOVE:
      user->getRegion()->setSpeed(user, orientation);
      break;
    default: ;
  }
  if(previous != nullptr) {
    previous->setNext(next);
    if(next != nullptr) {
      next->setPrevious(previous);
    }
  }
  else {
    if(next != nullptr) {
      next->setPrevious(nullptr);
      // tick is in range [0;1]
      next->computeTick(1 - tick);
    }
    if(type == ACTION_MOVE) {
      user->setLegAction(next);
    }
    else {
      user->setAction(next);
    }
  }
  return next;
}

void OrientedAction::computeTime(Adventure * adventure) {
  switch(type) {
    case ACTION_MOVE:
      time = 1.F;
      break;
    default:
      time = 0.F;
  }
  tick = time;
}

float OrientedAction::getOrientation() { return orientation; }
