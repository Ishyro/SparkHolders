#include "data/Map.h"

#include "data/furnitures/LinkedFurniture.h"

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
