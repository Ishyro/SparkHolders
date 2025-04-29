#include "data/Adventure.h"
#include "data/Map.h"
#include "data/Region.h"
#include "data/World.h"

#include "data/items/Gear.h"

#include "data/actions/BaseAction.h"
#include "data/actions/TargetedAction.h"

#include "util/MathUtil.h"

Action * TargetedAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != ACTION_BREAKPOINT) {
      next->execute(adventure);
    }
  }
  setUserOrientationToTarget(adventure);
  switch(type) {
    case ACTION_ACTIVATION: {
      if(target->furniture != nullptr && target->furniture->type != FURNITURE_BASIC) {
        ((ActivableFurniture *) target->furniture)->activate(user, false);
      }
      break;
    }
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
    user->setAction(next);
  }
  return next;
}

void TargetedAction::computeTime(Adventure * adventure) {
  switch(type) {
    case ACTION_ACTIVATION: {
      if(target->furniture != nullptr && target->furniture->type != FURNITURE_BASIC) {
        time = user->getHandActionTimeModifier() * ((ActivableFurniture *) target->furniture)->activation_time;
      }
      break;
    }
    default:
      time = 0.F;
  }
  tick = time;
}

MathUtil::Target * TargetedAction::getTarget() { return target; }

void TargetedAction::setUserOrientationToTarget(Adventure * adventure) {
  if(target->type == TARGET_COORDINATES) {
    user->setOrientationZ(MathUtil::getOrientationToTarget(user->getCoord().x, user->getCoord().y, target->coord.x, target->coord.y));
  }
  else if(target->type == TARGET_CHARACTER) {
    Character * other = target->character;
    user->setOrientationZ(MathUtil::getOrientationToTarget(user->getCoord().x, user->getCoord().y, other->getCoord().x, other->getCoord().y));
  }
  else if(target->type == TARGET_FURNITURE) {
    Furniture * other = target->furniture;
    user->setOrientationZ(MathUtil::getOrientationToTarget(user->getCoord().x, user->getCoord().y, other->getCenter().x, other->getCenter().y));
  }
  else if(target->type == TARGET_FRONT) {
    user->setOrientationZ(target->coord.z);
  }
}

float TargetedAction::rangeFromTarget(Adventure * adventure) {
  if(target->type == TARGET_COORDINATES) {
    return MathUtil::distance2(user->getCoord(), target->coord);
  }
  else if(target->type == TARGET_CHARACTER) {
    Character * other = target->character;
    return MathUtil::distance(user->getCoord(), other->getCoord());
  }
  else if(target->type == TARGET_FURNITURE) {
    Furniture * other = target->furniture;
    return MathUtil::distance(user->getCoord(), other->getCenter());
  }
  return 0.F;
}

