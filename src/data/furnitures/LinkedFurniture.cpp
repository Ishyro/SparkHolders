#include "data/Map.h"

#include "data/furnitures/LinkedFurniture.h"

bool LinkedFurniture::getUnwalkable() { return unwalkable; }
bool LinkedFurniture::getOpaque() { return opaque; }
bool LinkedFurniture::getSolid() { return solid; }
int LinkedFurniture::getLight() { return light; }

void LinkedFurniture::init(Map * map, ActivableFurniture * linked_furniture) {
    this->linked_furniture = (ActivableFurniture *) map->getFurniture(linked_furniture->getX(), linked_furniture->getY());
}

Furniture * LinkedFurniture::getLinkedFurniture() { return linked_furniture; }

void LinkedFurniture::setLinkedFurniture(ActivableFurniture * linked_furniture) { this->linked_furniture = linked_furniture; }

void LinkedFurniture::activate(Character * user, bool remote) {
    if(remote || tryActivate(user)) {
        linked_furniture->activate(user, true);
    }
}
