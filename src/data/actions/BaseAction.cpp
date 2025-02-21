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
    case ACTION_RUN:
      user->run();
      break;
    case ACTION_JUMP:
      user->jump();
      break;
    case ACTION_BREAKPOINT:
      break;
    case ACTION_CHANNEL:
      user->channel(-1);
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
    if(type == ACTION_RUN || type == ACTION_JUMP) {
      user->setLegAction(next);
    }
    else {
      user->setAction(next);
    }
  }
  return next;
}

void BaseAction::computeTime(Adventure * adventure) {
  switch(type) {
    case ACTION_CHANNEL:
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
    case ACTION_RUN:
    case ACTION_JUMP:
      time = 0.F;
      break;
    default:
      time = 0.F;
  }
  tick = time;
}
