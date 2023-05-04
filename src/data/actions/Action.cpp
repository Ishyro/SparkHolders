#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Item.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/Speech.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/Weapon.h"
#include "data/World.h"
#include "data/Database.h"

#include "data/actions/Action.h"

#include "util/MapUtil.h"
#include "util/String.h"

#include <iostream>

/*
    case TALKING:
      if(object != "") {
        ((Character *) target)->setTalkingSpeech(object, adventure->getDatabase());
      }
      break;
    case ECONOMICS:
      ((Character *) target)->trade(user, orientation, object, Settings::getBuyingPriceModifier());
      break;
    default: ;
  }

*/
Character * Action::getUser() { return user; }
int Action::getTick() { return tick; }
int Action::getTime() { return time; }
Action * Action::getPrevious() { return previous; }
Action * Action::getNext() { return next; }
void Action::setTick(int tick) { this->tick = tick; }
void Action::setTime(int time) { this->time = time; }
void Action::setPrevious(Action * action) { previous = action; }
void Action::setNext(Action * action) { next = action; }

void Action::computeTick(float tick) {
  if(previous == nullptr) {
    this->tick = MapUtil::round(this->tick - (float) tick);
  }
}
