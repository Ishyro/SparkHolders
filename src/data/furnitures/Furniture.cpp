#include "data/furnitures/Furniture.h"

#include "data/Map.h"

void Furniture::init(Map * map, int x, int y) {
    this->x = x + map->offsetX;
    this->y = y + map->offsetY;
    this->z = map->offsetZ; 
}

int Furniture::getX() { return x; }
int Furniture::getY() { return y; }
int Furniture::getZ() { return z; }
float Furniture::getOrientation() { return orientation; }
bool Furniture::getUntraversable() { return untraversable; }
bool Furniture::getOpaque() { return opaque; }
bool Furniture::getSolid() { return solid; }
int Furniture::getLight() { return light; }
