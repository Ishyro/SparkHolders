#include "data/Character.h"
#include "data/items/Item.h"

#include "data/furnitures/ContainerFurniture.h"

void ContainerFurniture::activate(Character * user, bool remote) {
  if(remote || tryActivate(user)) {

  }
}