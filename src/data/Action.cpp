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

void Action::execute(Adventure * adventure) {
  switch(type) {
    case MOVE:
      adventure->getWorld()->getMap(user->getCurrentMapId())->move(user, orientation, adventure);
      break;
    case REST:
      user->rest();
      break;
    case SHOOT: {
      user->setOrientation(orientation);
      Projectile * projectile = user->shoot(target, target_y, target_x);
      if(projectile != nullptr) {
        adventure->getWorld()->getMap(user->getCurrentMapId())->addProjectile(projectile);
      }
      break;
    }
    case FORCE_STRIKE: {
      user->setOrientation(orientation);
      if(target != nullptr) {
        user->attack( (Character *) target);
        if(!( (Character *) target)->isAlive()) {
          adventure->getWorld()->getMap(user->getCurrentMapId())->killCharacter(user, (Character *) target);
        }
      } else {
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
    case CHANGE_MAP:
      if(user->getX() == link->x1 && user->getY() == link->y1 && user->getCurrentMapId() == link->map1->id) {
        adventure->getWorld()->getMap(user->getCurrentMapId())->removeCharacter(user);
        user->move(link->y2, link->x2);
        user->setCurrentMapId(link->map2->id);
        adventure->getWorld()->getMap(user->getCurrentMapId())->addCharacter(user);
      }
      else if(user->getX() == link->x2 && user->getY() == link->y2 && user->getCurrentMapId() == link->map2->id) {
        adventure->getWorld()->getMap(user->getCurrentMapId())->removeCharacter(user);
        user->move(link->y1, link->x1);
        user->setCurrentMapId(link->map1->id);
        adventure->getWorld()->getMap(user->getCurrentMapId())->addCharacter(user);
      }
      break;
    case GRAB:
      adventure->getWorld()->getMap(user->getCurrentMapId())->takeLoot(user);
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
    case ECONOMICS:
      break;
    default: ;
  }
}

Character * Action::getUser() { return user; }
