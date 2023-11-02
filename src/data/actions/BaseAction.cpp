#include "data/Adventure.h"

#include "data/actions/BaseAction.h"

Action * BaseAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != ACTION_BREAKPOINT) {
      next->execute(adventure);
    }
  }
  switch(type) {
    case ACTION_IDLE:
    case ACTION_RESPITE:
    case ACTION_REST:
      user->rest();
      break;
    case ACTION_BREAKPOINT:
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
    case ACTION_IDLE:
      time = 1.F;
      break;
    case ACTION_RESPITE:
      time = user->getStrikeTime(ITEM_SLOT_WEAPON_1) * 2;
      break;
    case ACTION_REST:
      time = 1.F;
      break;
    case ACTION_BREAKPOINT:
      time = 0.F;
      break;
    default:
      time = 0.F;
  }
  tick = time;
}
