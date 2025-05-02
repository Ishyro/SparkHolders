#include "data/furnitures/Furniture.h"


void Furniture::init(MathUtil::Vector3i coord, MathUtil::Vector3i offset, int64_t rotation) {
  this->coord.z = coord.z + offset.z;
  switch(rotation) {
    case 0:
      this->coord.x = coord.x + offset.x;
      this->coord.y = coord.y + offset.y;
      break;
    case 90:
      this->coord.x = CHUNK_SIZE - 1 - coord.y + offset.x;
      this->coord.y = coord.x + offset.y;
      orientation_z += 90.F;
      break;
    case 180:
      this->coord.x = CHUNK_SIZE - 1 - coord.x + offset.x;
      this->coord.y = CHUNK_SIZE - 1 - coord.y + offset.y;
      orientation_z += 180.F;
      break;
    case 270:
      this->coord.x = coord.y + offset.x;
      this->coord.y = CHUNK_SIZE - 1 - coord.x + offset.y;
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
