#include "data/Adventure.h"

#include "data/actions/BaseAction.h"

Action * BaseAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != BREAKPOINT) {
      next->execute(adventure);
    }
  }
  switch(type) {
    case IDLE:
    case RESPITE:
    case REST:
      user->rest();
      break;
    case BREAKPOINT:
        break;
    default: ;
  }
  if(previous != nullptr) {
    previous->setNext(next);
    next->setPrevious(previous);
  }
  else if(next != nullptr) {
    next->setPrevious(nullptr);
    // tick is in range [0;1]
    next->computeTick(1 - tick);
  }
  else {
    user->setNeedToUpdateActions(true);
  }
  return next;
}

void BaseAction::computeTime(Adventure * adventure) {
  switch(type) {
    case IDLE:
      time = 1.F;
      break;
    case RESPITE:
      time = user->getStrikeTime() * 2;
      break;
    case REST:
      time = 1.F;
      break;
    case BREAKPOINT:
      time = 0.F;
      break;
    default:
      time = 0.F;
  }
  tick = time;
}
