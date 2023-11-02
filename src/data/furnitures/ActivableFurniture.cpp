#include "data/Character.h"
#include "data/items/Item.h"

#include "data/furnitures/ActivableFurniture.h"

bool ActivableFurniture::tryActivate(Character * user) {
  if(isLocked) {
    bool hasKey = false;
    for(ItemSlot * slot: user->getGear()->getItems()) {
      if(slot->item->name == key_name) {
        hasKey = true;
        break;
      }
    }
    if(!hasKey) {
      return false;
    }
  }
  return true;
}