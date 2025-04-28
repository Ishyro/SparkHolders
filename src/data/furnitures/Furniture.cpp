#include "data/furnitures/Furniture.h"

#include "data/Map.h"

void Furniture::init(Map * map, MathUtil::Vector3i coord) {
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
      orientation_z += 90.F;
      break;
    case 180:
      this->coord.x = map->sizeX - 1 - coord.x + map->offsetX;
      this->coord.y = map->sizeY - 1 - coord.y + map->offsetY;
      this->coord.z = coord.z + map->offsetZ;
      orientation_z += 180.F;
      break;
    case 270:
      this->coord.x = coord.y + map->offsetX;
      this->coord.y = map->sizeX - 1 - coord.x + map->offsetY;
      this->coord.z = coord.z + map->offsetZ;
      orientation_z += 270.F;
      break;
    default: ;
  }
  orientation_z = orientation_z >= 360.F ? orientation_z - 360.F : orientation_z;
  center = MathUtil::Vector3( (float) this->coord.x + (float) sizeX * .5F, (float) this->coord.y + (float) sizeY * .5F, (float) this->coord.z + (float) sizeZ * .5F);
  hitbox->applyMove(center, MathUtil::getDirectionFromOrientation(MathUtil::Vector3(90.F, 0.F, orientation_z)));
}

MathUtil::Vector3i Furniture::getCoord() { return coord; }
MathUtil::Vector3 Furniture::getCenter() { return center; }
MathUtil::HitboxOBB * Furniture::getHitbox() { return hitbox; }
float Furniture::getOrientationZ() { return orientation_z; }
