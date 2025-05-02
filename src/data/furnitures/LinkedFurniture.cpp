#include "data/furnitures/LinkedFurniture.h"

bool LinkedFurniture::getUnwalkable() { return unwalkable; }
bool LinkedFurniture::getOpaque() { return opaque; }
bool LinkedFurniture::getSolid() { return solid; }
int32_t LinkedFurniture::getLight() { return light; }

void LinkedFurniture::init(ActivableFurniture * linked_furniture, MathUtil::Vector3i coord, MathUtil::Vector3i offset, int64_t rotation) {
    // TODO
    //this->linked_furniture = (ActivableFurniture *) map->getFurniture(linked_furniture->getCoord().x, linked_furniture->getCoord().y);
}

Furniture * LinkedFurniture::getLinkedFurniture() { return linked_furniture; }

void LinkedFurniture::setLinkedFurniture(ActivableFurniture * linked_furniture) { this->linked_furniture = linked_furniture; }

void LinkedFurniture::activate(Character * user, bool remote) {
    if(remote || tryActivate(user)) {
        linked_furniture->activate(user, true);
    }
}
