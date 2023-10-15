#include "data/Character.h"

#include "data/furnitures/CraftingFurniture.h"

bool CraftingFurniture::getUnwalkable() { return unwalkable; }
bool CraftingFurniture::getOpaque() { return opaque; }
bool CraftingFurniture::getSolid() { return solid; }
int CraftingFurniture::getLight() { return light; }

void CraftingFurniture::activate(Character * user, bool remote) {
  if(remote || tryActivate(user)) {

  }
}