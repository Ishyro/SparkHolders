#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Item.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/skills/Skill.h"
#include "data/Speech.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/Weapon.h"
#include "data/World.h"
#include "data/Database.h"

#include "data/Action.h"

#include <iostream>

void Action::execute(Adventure * adventure) {
  switch(type) {
    case MOVE:
      adventure->getWorld()->getMap(user->getCurrentMapId())->move(user, orientation, adventure);
      break;
    case CHANGE_MAP:
      if(user->getX() == link->x1 && user->getY() == link->y1 && user->getCurrentMapId() == link->map1->id) {
        adventure->getWorld()->getMap(user->getCurrentMapId())->removeCharacter(user);
        user->move(link->x2, link->y2, link->orientation2);
        user->setCurrentMapId(link->map2->id);
        adventure->getWorld()->getMap(user->getCurrentMapId())->addCharacter(user);
      }
      else if(user->getX() == link->x2 && user->getY() == link->y2 && user->getCurrentMapId() == link->map2->id) {
        adventure->getWorld()->getMap(user->getCurrentMapId())->removeCharacter(user);
        user->move(link->x1, link->y1, link->orientation1);
        user->setCurrentMapId(link->map1->id);
        adventure->getWorld()->getMap(user->getCurrentMapId())->addCharacter(user);
      }
      break;
    case GRAB:
      adventure->getWorld()->getMap(user->getCurrentMapId())->takeLoot(user);
      break;
    case REST:
      break;
    default: ;
  }
}

Character * Action::getUser() { return user; }
