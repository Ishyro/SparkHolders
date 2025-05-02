#include "data/Adventure.h"
#include "data/Database.h"
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
      user->reload(slot1, ITEM_SLOT_WEAPON_1);
      break;
    case ACTION_SWAP_GEAR:
      if((slot1->id == user->id && slot2->id == user->id) || (slot1->id == user->getGear()->getBag()->id && slot2->id == user->getGear()->getBag()->id)) {
        if((slot1->slot == ITEM_SLOT_WEAPON_1 || slot1->slot == ITEM_SLOT_WEAPON_2 || slot1->slot == ITEM_SLOT_WEAPON_3) &&
          (slot2->slot == ITEM_SLOT_WEAPON_1 || slot2->slot == ITEM_SLOT_WEAPON_2 || slot2->slot == ITEM_SLOT_WEAPON_3)) {
          user->getGear()->swapWeapon(slot1->slot, slot2->slot);
        }
        else {
          user->getGear()->swapItem(slot1, slot2);
        }
        user->setNeedToSend(true);
      }
      break;
    case ACTION_GRAB:
      //adventure->getWorld()->getMap(user->getCurrentMap()->id)->takeLoot(user, item_id);
      break;
    case ACTION_USE_ITEM:
      user->useItem(slot1->x, slot1->y, slot1->slot);
      break;
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

void GearAction::computeTime(Adventure * adventure) {
  switch(type) {
    case ACTION_RELOAD:
      time = user->getReloadTime(ITEM_SLOT_WEAPON_1);
      break;
    case ACTION_SWAP_GEAR:
      if((slot1->slot == ITEM_SLOT_WEAPON_1 || slot1->slot == ITEM_SLOT_WEAPON_2 || slot1->slot == ITEM_SLOT_WEAPON_3) &&
        (slot2->slot == ITEM_SLOT_WEAPON_1 || slot2->slot == ITEM_SLOT_WEAPON_2 || slot2->slot == ITEM_SLOT_WEAPON_3)) {
        time = user->getSwapTime(slot1->slot, slot2->slot);
      }
      else {
        time = 5;
      }
      break;
    case ACTION_USE_ITEM:
      time = user->getUseTime(slot1->item); //(float) ( (Item *) adventure->getDatabase()->getItem(piece->name))->use_time / user->getHandActionTimeModifier();
      break;
    case ACTION_GRAB:
      time = 10.F / user->getHandActionTimeModifier();
      break;
    default:
      time = 0.F;
  }
  tick = time;
}
