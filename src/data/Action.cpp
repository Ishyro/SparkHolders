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

#include "data/Action.h"

#include <iostream>

Action * Action::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->time == 0) {
      next->execute(adventure);
    }
  }
  switch(type) {
    case MOVE: {
      float ap = 1.F;
      user->setOrientation(orientation);
      while(ap > 0.F) {
        ap = adventure->getWorld()->getMap(user->getCurrentMapId())->move(user, user->getOrientation(), ap, adventure->getWorld());
      }
      break;
    }
    case REST:
      user->rest();
      break;
    case SHOOT: {
      Projectile * projectile = user->shoot(target, target_y, target_x, 0.5F, 0.5F);
      if(projectile != nullptr) {
        adventure->getWorld()->getMap(user->getCurrentMapId())->addProjectile(projectile);
      }
      break;
    }
    case STRIKE: {
      user->setOrientation(orientation);
      if(target != nullptr) {
        user->attack( (Character *) target);
        if(!( (Character *) target)->isAlive()) {
          adventure->getWorld()->getMap(user->getCurrentMapId())->killCharacter(user, (Character *) target);
        }
      }
      else {
        for(Character * c : adventure->getWorld()->getMap(user->getCurrentMapId())->getCharacters()) {
          if(c != nullptr && c != user && !c->isEtheral() && c->getX() == target_x && c->getY() == target_y) {
            user->attack(c);
            if(!c->isAlive()) {
              adventure->getWorld()->getMap(user->getCurrentMapId())->killCharacter(user, c);
            }
            break;
          }
        }
      }
      break;
    }
    case HEAVY_STRIKE: {
      user->setOrientation(orientation);
      if(target != nullptr) {
        user->attack( (Character *) target);
        if(!( (Character *) target)->isAlive()) {
          adventure->getWorld()->getMap(user->getCurrentMapId())->killCharacter(user, (Character *) target);
        }
      }
      else {
        for(Character * c : adventure->getWorld()->getMap(user->getCurrentMapId())->getCharacters()) {
          if(c != nullptr && c != user && !c->isEtheral() && c->getX() == target_x && c->getY() == target_y) {
            user->attack(c);
            if(!c->isAlive()) {
              adventure->getWorld()->getMap(user->getCurrentMapId())->killCharacter(user, c);
            }
            break;
          }
        }
      }
      break;
    }
    case RELOAD:
      for(Ammunition * ammo : user->getAmmunitions()) {
        if(ammo->projectile->name == object) {
          user->reload(ammo);
        }
      }
      break;
    case SWAP_GEAR:
      for(Weapon * weapon : user->getWeapons()) {
        if(weapon->name == object) {
          user->equip(weapon);
        }
      }
      for(Ammunition * ammo : user->getAmmunitions()) {
        if(ammo->projectile->name == object) {
          user->reload(ammo);
        }
      }
      for(Item * item : user->getItems()) {
        if(item->name == object) {
          user->equip(item);
        }
      }
      break;
    case GRAB:
      adventure->getWorld()->getMap(user->getCurrentMapId())->takeLoot(user, (int) orientation);
      break;
    case USE_SKILL: {
      user->setOrientation(orientation);
      if(user->hasSkill( (Skill *) skill) && ( (Skill *) skill)->canCast(user, (Character *) target, adventure, overcharge_power, overcharge_duration, overcharge_range, user->getCurrentMapId(), target_x, target_y)) {
        user->useSkill( (Skill *) skill, (Character *) target, adventure, overcharge_power, overcharge_duration, overcharge_range, target_x, target_y);
      }
      break;
    }
    case USE_ITEM:
      user->useItem(object);
      break;
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
  if(previous != nullptr) {
    previous->next = next;
    next->previous = previous;
  }
  else if(next != nullptr) {
    next->previous = nullptr;
    next->computeTick(0);
  }
  else {
    user->setNeedToUpdateActions(true);
  }
  return next;
}

Character * Action::getUser() { return user; }
int Action::getTick() { return tick; }
void Action::setPrevious(Action * action) { previous = action; }
void Action::setNext(Action * action) { next = action; }

void Action::computeTick(int tick) {
  if(previous == nullptr) {
    this->tick -= (float) tick;
  }
}

void Action::computeTime(Adventure * adventure) {
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
    case RESPITE:
      time = user->getStrikeTime() * 2;
      break;
    case USE_SKILL:
      time = (float) skill->time / user->getSkillTimeModifier();
      break;
    case REST:
      time = 1.F;
      break;
    case RELOAD:
      time = user->getReloadTime();
      break;
    case SWAP_GEAR:
      time = user->getSwapTime(object);
      break;
    case GRAB:
      time = 10.F / user->getHandActionTimeModifier();
      break;
    case USE_ITEM:
      time = (float) ( (Item *) adventure->getDatabase()->getItem(object))->use_time / user->getHandActionTimeModifier();
      break;
    case ECONOMICS:
    case TALKING:
      time = 0.F;
      break;
    default:
      time = 0.F;
  }
  tick = time;
}
