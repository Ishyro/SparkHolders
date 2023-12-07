#include "data/furnitures/Furniture.h"

#include "data/Map.h"

void Furniture::init(Map * map, MapUtil::Vector3i coord) {
  this->coord.z = map->offsetZ; 
  switch(map->getRotation()) {
    case 0:
      this->coord.x = coord.x + map->offsetX;
      this->coord.y = coord.y + map->offsetY;
      this->coord.z = coord.z + map->offsetZ;
      break;
    case 90:
      this->coord.x = map->sizeY - 1 - coord.y + map->offsetX;
      this->coord.y = coord.x + map->offsetY;
      this->coord.z = coord.z + map->offsetZ;
      orientation += 90.F;
      break;
    case 180:
      this->coord.x = map->sizeX - 1 - coord.x + map->offsetX;
      this->coord.y = map->sizeY - 1 - coord.y + map->offsetY;
      this->coord.z = coord.z + map->offsetZ;
      orientation += 180.F;
      break;
    case 270:
      this->coord.x = coord.y + map->offsetX;
      this->coord.y = map->sizeX - 1 - coord.x + map->offsetY;
      this->coord.z = coord.z + map->offsetZ;
      orientation += 270.F;
      break;
    default: ;
  }
  orientation = orientation >= 360.F ? orientation - 360.F : orientation;
}

MapUtil::Vector3i Furniture::getCoord() { return coord; }
float Furniture::getOrientation() { return orientation; }
