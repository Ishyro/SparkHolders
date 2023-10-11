#include "data/Character.h"

#include "data/furnitures/CraftingFurniture.h"

void CraftingFurniture::activate(Character * user, bool remote) {
  if(remote || tryActivate(user)) {

  }
}