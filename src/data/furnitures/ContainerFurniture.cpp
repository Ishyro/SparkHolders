#include "data/Character.h"
#include "data/items/Item.h"

#include "data/furnitures/ContainerFurniture.h"

bool ContainerFurniture::getUnwalkable() { return unwalkable; }
bool ContainerFurniture::getOpaque() { return opaque; }
bool ContainerFurniture::getSolid() { return solid; }
int ContainerFurniture::getLight() { return light; }

void ContainerFurniture::activate(Character * user, bool remote) {
  if(remote || tryActivate(user)) {

  }
}