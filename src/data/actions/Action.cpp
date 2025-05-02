#include "data/Adventure.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/Speech.h"
#include "data/Block.h"
#include "data/World.h"
#include "data/Database.h"

#include "data/items/Item.h"

#include "data/actions/Action.h"

#include "data/ways/Way.h"
#include "data/ways/Attributes.h"

#include "util/MathUtil.h"
#include "util/String.h"

/*
    case TALKING:
      if(object != "") {
        ((Character *) target)->setTalkingSpeech(object, adventure->getDatabase());
      }
      break;
    case ECONOMICS:
      ((Character *) target)->trade(user, orientation_z, object, Settings::getBuyingPriceModifier());
      break;
    default: ;
  }

*/
Character * Action::getUser() { return user; }
int32_t Action::getTick() { return tick; }
int32_t Action::getTime() { return time; }
Action * Action::getPrevious() { return previous; }
Action * Action::getNext() { return next; }
void Action::setTick(int32_t tick) { this->tick = tick; }
void Action::setTime(int32_t time) { this->time = time; }
void Action::setPrevious(Action * action) { previous = action; }
void Action::setNext(Action * action) { next = action; }

void Action::computeTick(float tick) {
  if(previous == nullptr) {
    this->tick = MathUtil::round(this->tick - (float) tick);
  }
}
