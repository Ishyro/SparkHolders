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
  user->setOrientation(adventure->getWorld()->setPathToTarget(user->getRegion(), user->getCoord().x, user->getCoord().y, target));
  switch(type) {
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
    }
    */
    case ACTION_ACTIVATION: {
      Furniture * furniture = user->getRegion()->getFurniture(MathUtil::makeVector3i(target->coord));
      if(furniture != nullptr && furniture->type != FURNITURE_BASIC) {
        ((ActivableFurniture *) furniture)->activate(user, false);
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
    case ACTION_MOVE:
      time = 1.F;
      break;
    case ACTION_STRIKE:
      time = user->getStrikeTime(ITEM_SLOT_WEAPON_1);
      break;
    case ACTION_ACTIVATION: {
      Furniture * furniture = user->getRegion()->getFurniture(MathUtil::makeVector3i(target->coord));
      if(furniture != nullptr && furniture->type != FURNITURE_BASIC) {
        time = user->getHandActionTimeModifier() * ((ActivableFurniture *) furniture)->activation_time;
      }
      break;
    }
    default:
      time = 0.F;
  }
  tick = time;
}

Target * TargetedAction::getTarget() { return target; }

void TargetedAction::setUserOrientationToTarget(Adventure * adventure) {
  if(target->type == TARGET_COORDINATES || target->type == TARGET_BLOCK) {
    user->setOrientation(MathUtil::getOrientationToTarget(user->getCoord().x, user->getCoord().y, target->coord.x, target->coord.y));
  }
  else if(target->type == TARGET_CHARACTER) {
    Character * other = target->character;
    user->setOrientation(MathUtil::getOrientationToTarget(user->getCoord().x, user->getCoord().y, other->getCoord().x, other->getCoord().y));
  }
}

float TargetedAction::rangeFromTarget(Adventure * adventure) {
  if(target->type == TARGET_COORDINATES || target->type == TARGET_BLOCK) {
    return MathUtil::distance2(user->getCoord(), target->coord);
  }
  else if(target->type == TARGET_CHARACTER) {
    Character * other = target->character;
    return MathUtil::distance(user->getCoord(), other->getCoord());
  }
  return 0.F;
}

