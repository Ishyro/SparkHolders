#include "data/Adventure.h"
#include "data/Database.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/actions/GearAction.h"

Action * GearAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != BREAKPOINT) {
      next->execute(adventure);
    }
  }
  switch(type) {
    case RELOAD:
      for(Ammunition * ammo : user->getAmmunitions()) {
        if(ammo->projectile->name == piece->name) {
          user->reload(ammo);
        }
      }
      break;
    case SWAP_GEAR:
      if(piece->type == WEAPON) {
        for(Weapon * weapon : user->getWeapons()) {
          if(weapon->id == piece->id) {
            user->equip(weapon);
            break;
          }
        }
      }
      if(piece->type == AMMUNITION) {
        for(Ammunition * ammo : user->getAmmunitions()) {
          if(ammo->projectile->name == piece->name) {
            user->reload(ammo);
            break;
          }
        }
      }
      if(piece->type == ITEM) {
        for(Item * item : user->getItems()) {
          if(item->id == piece->id) {
            user->equip(item);
            break;
          }
        }
      }
      break;
    case GRAB:
      adventure->getWorld()->getMap(user->getCurrentMapId())->takeLoot(user, piece->id);
      break;
    case USE_ITEM:
      user->useItem(piece->id);
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

void GearAction::computeTime(Adventure * adventure) {
  switch(type) {
    case RELOAD:
      time = user->getReloadTime();
      break;
    case SWAP_GEAR:
      time = user->getSwapTime(piece->name);
      break;
    case USE_ITEM:
      time = (float) ( (Item *) adventure->getDatabase()->getItem(piece->name))->use_time / user->getHandActionTimeModifier();
      break;
    case GRAB:
      time = 10.F / user->getHandActionTimeModifier();
      break;
    default:
      time = 0.F;
  }
  tick = time;
}

GearPiece * GearAction::getGearPiece() { return piece; }
void GearAction::setGearPiece(GearPiece * piece) { this->piece = piece; }
