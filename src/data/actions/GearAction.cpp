#include "data/Adventure.h"
#include "data/Database.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/items/Item.h"
#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/SerialItem.h"
#include "data/items/AmmunitionItem.h"

#include "data/actions/GearAction.h"

Action * GearAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != ACTION_BREAKPOINT) {
      next->execute(adventure);
    }
  }
  switch(type) {
    case ACTION_RELOAD:
      for(Item * ammo : user->getItems()) {
        if(ammo->type == ITEM_AMMUNITION && ((AmmunitionItem *) ammo)->id == item_id) {
          user->reload((AmmunitionItem *) ammo);
          break;
        }
      }
      break;
    case ACTION_SWAP_GEAR:
      for(Item * item : user->getItems()) {
        if(item->id == item_id) {
          user->equip((GearItem *) item);
          break;
        }
      }
      break;
    case ACTION_GRAB:
      adventure->getWorld()->getMap(user->getCurrentMap()->id)->takeLoot(user, item_id);
      break;
    case ACTION_USE_ITEM:
      user->useItem(item_id);
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
    case ACTION_RELOAD:
      time = user->getReloadTime();
      break;
    case ACTION_SWAP_GEAR:
      time = user->getSwapTime(item_id);
      break;
    case ACTION_USE_ITEM:
      time = user->getUseTime(item_id); //(float) ( (Item *) adventure->getDatabase()->getItem(piece->name))->use_time / user->getHandActionTimeModifier();
      break;
    case ACTION_GRAB:
      time = 10.F / user->getHandActionTimeModifier();
      break;
    default:
      time = 0.F;
  }
  tick = time;
}
