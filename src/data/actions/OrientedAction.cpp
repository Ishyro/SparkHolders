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
      user->getRegion()->setSpeed(user, orientation_z, orientation_x);
      break;
      /*
    case ACTION_STRIKE: {
      if(user->getGear()->getWeapon_1()->use_projectile || user->getGear()->getWeapon_1()->use_projectile) {
        Projectile * projectile = user->shoot(target, adventure, ITEM_SLOT_WEAPON_1);
        if(projectile != nullptr) {
          //adventure->getWorld()->getMap(user->getCurrentMap()->id)->addProjectile(projectile);
        }
      }
      else {
        if(target->type == TARGET_CHARACTER) {
          Character * other = target->character; 
          user->attack(other, adventure, ACTION_STRIKE);
          if(!other->isAlive()) {
            //adventure->getWorld()->getMap(user->getCurrentMap()->id)->killCharacter(user, other);
          }
        }
        else {
          for(Character * c : adventure->getWorld()->getMap(user->getCurrentMap()->id)->getCharacters()) {
            if(c != nullptr && c != user && !c->isEtheral() && c->getX() == target_x && c->getY() == target_y) {
              user->attack(c);
              if(!c->isAlive()) {
                adventure->getWorld()->getMap(user->getCurrentMap()->id)->killCharacter(user, c);
              }
              break;
            }
          }
        }
      }
      Action * temp = next;
      next = new BaseAction(ACTION_RESPITE, adventure, nullptr, user);
      next->setNext(temp);
      break;
    } */
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
    case ACTION_STRIKE:
      time = user->getStrikeTime(ITEM_SLOT_WEAPON_1);
      break;
    default:
      time = 0.F;
  }
  tick = time;
}

float OrientedAction::getOrientationZ() { return orientation_z; }
