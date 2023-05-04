#include "data/Adventure.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/actions/BaseAction.h"
#include "data/actions/TargetedAction.h"

#include "util/MapUtil.h"

#include <iostream>

Action * TargetedAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != BREAKPOINT) {
      next->execute(adventure);
    }
  }
  user->setOrientation(adventure->getWorld()->setPathToTarget(user->getCurrentMapId(), user->getX(), user->getY(), target));
  switch(type) {
    case MOVE: {
      float ap = 1.F;
      while(ap > 0.F) {
        ap = adventure->getWorld()->getMap(user->getCurrentMapId())->move(user, user->getOrientation(), ap, adventure->getWorld());
        // collision with other character
        if(ap == -1) {
          break;
        }
        // we took a MapLink, or maybe got stuck on a wall that wasn't here before, but that would update our Actions anyway
        else if(ap > 0.F && target->next != nullptr) {
          Target * temp = target;
          target = target->next;
          delete temp;
          setUserOrientationToTarget(adventure);
        }
      }
      if(target->next == nullptr && rangeFromTarget(adventure) < 0.1 && (next == nullptr || next->type != MOVE)) {
        Action * temp = next;
        next = new TargetedAction(MOVE, adventure, nullptr, user);
        ((TargetedAction *) next)->setTarget(target);
        next->setNext(temp);
      }
      break;
    }
    case SHOOT: {
      Projectile * projectile = user->shoot(target, adventure);
      if(projectile != nullptr) {
        adventure->getWorld()->getMap(user->getCurrentMapId())->addProjectile(projectile);
      }
      break;
    }
    case STRIKE: {
      if(target->type == CHARACTER) {
        Character * other = adventure->getCharacter(target->id); 
        user->attack(other, STRIKE);
        if(!other->isAlive()) {
          adventure->getWorld()->getMap(user->getCurrentMapId())->killCharacter(user, other);
        }
      }
      else {
        /*
        for(Character * c : adventure->getWorld()->getMap(user->getCurrentMapId())->getCharacters()) {
          if(c != nullptr && c != user && !c->isEtheral() && c->getX() == target_x && c->getY() == target_y) {
            user->attack(c);
            if(!c->isAlive()) {
              adventure->getWorld()->getMap(user->getCurrentMapId())->killCharacter(user, c);
            }
            break;
          }
        }
        */
      }
      Action * temp = next;
      next = new BaseAction(RESPITE, adventure, nullptr, user);
      next->setNext(temp);
      break;
    }
    case HEAVY_STRIKE: {
      if(target->type == CHARACTER) {
        Character * other = adventure->getCharacter(target->id); 
        user->attack(other, HEAVY_STRIKE);
        if(!other->isAlive()) {
          adventure->getWorld()->getMap(user->getCurrentMapId())->killCharacter(user, other);
        }
      }
      else {
        /*
        for(Character * c : adventure->getWorld()->getMap(user->getCurrentMapId())->getCharacters()) {
          if(c != nullptr && c != user && !c->isEtheral() && c->getX() == target_x && c->getY() == target_y) {
            user->attack(c);
            if(!c->isAlive()) {
              adventure->getWorld()->getMap(user->getCurrentMapId())->killCharacter(user, c);
            }
            break;
          }
        }
        */
      }
      break;
    }
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

void TargetedAction::computeTime(Adventure * adventure) {
  switch(type) {
    case MOVE:
      time = 1.F;
      break;
    case SHOOT:
      time = user->getStrikeTime();
      break;
    case STRIKE:
      time = user->getStrikeTime();
      break;
    case HEAVY_STRIKE:
      time = user->getStrikeTime() * 5;
      break;
    default:
      time = 0.F;
  }
  tick = time;
}

Target * TargetedAction::getTarget() { return target; }
void TargetedAction::setTarget(Target * target) { this->target = target; }

void TargetedAction::setUserOrientationToTarget(Adventure * adventure) {
  if(target->type == COORDINATES || target->type == TILE) {
    if(target->id == user->getCurrentMapId()) {
      user->setOrientation(MapUtil::getOrientationToTarget(user->getX(), user->getY(), target->x, target->y));
    }
    // shouldn't happen if Target is well built
    else {
      user->setOrientation(adventure->getWorld()->setPathToTarget(user->getCurrentMapId(), user->getX(), user->getY(), target));
    }
  }
  else if(target->type == CHARACTER) {
    if(target->id == user->getCurrentMapId()) {
      Character * other = adventure->getCharacter(target->id);
      user->setOrientation(MapUtil::getOrientationToTarget(user->getX(), user->getY(), other->getX(), other->getY()));
    }
    // shouldn't happen if Target is well built
    else {
      user->setOrientation(adventure->getWorld()->setPathToTarget(user->getCurrentMapId(), user->getX(), user->getY(), target));
    }
  }
}

float TargetedAction::rangeFromTarget(Adventure * adventure) {
  if(target->type == COORDINATES || target->type == TILE) {
    if(target->id == user->getCurrentMapId()) {
      return MapUtil::distance(user->getX(), user->getY(), target->x, target->y);
    }
    else {
      // TODO
      return 100;
    }
  }
  else if(target->type == CHARACTER) {
    if(target->id == user->getCurrentMapId()) {
      Character * other = adventure->getCharacter(target->id);
      return MapUtil::distance(user->getX(), user->getY(), other->getX(), other->getY());
    }
    else {
      // TODO
      return 100;
    }
  }
}

