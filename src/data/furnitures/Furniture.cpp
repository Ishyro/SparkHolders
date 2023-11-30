#include "data/furnitures/Furniture.h"

#include "data/Map.h"

void Furniture::init(Map * map, int x, int y) {
  this->z = map->offsetZ; 
  switch(map->getRotation()) {
    case 0:
      this->x = x + map->offsetX;
      this->y = y + map->offsetY;
      break;
    case 90:
      this->x = map->sizeY - 1 - y + map->offsetX;
      this->y = x + map->offsetY;
      orientation += 90.F;
      break;
    case 180:
      this->x = map->sizeX - 1 - x + map->offsetX;
      this->y = map->sizeY - 1 - y + map->offsetY;
      orientation += 180.F;
      break;
    case 270:
      this->x = y + map->offsetX;
      this->y = map->sizeX - 1 - x + map->offsetY;
      orientation += 270.F;
      break;
    default: ;
  }
  orientation = orientation >= 360.F ? orientation - 360.F : orientation;
}

int Furniture::getX() { return x; }
int Furniture::getY() { return y; }
int Furniture::getZ() { return z; }
float Furniture::getOrientation() { return orientation; }
