#include "data/Region.h"

void Region::addCharacter(Character * character) {
  characters.push_back(character);
  character->setRegion(this);
}

bool Region::removeCharacter(Character * character) {
  characters.remove(character);
  return characters.empty();
}

Block * Region::getBlock(MapUtil::Vector3 coord) {
  MapUtil::Vector3 asked = coord;
  return getBlock(MapUtil::makeVector3i(coord));
}

Block * Region::getBlock(MapUtil::Vector3i coord) {
  int a = (int) std::floor( (float) (coord.x - id.x) / (float) CHUNK_SIZE) + 1;
  int b = (int) std::floor( (float) (coord.y - id.y) / (float) CHUNK_SIZE) + 1;
  int c = (int) std::floor( (float) (coord.z - id.z) / (float) CHUNK_SIZE) + 1;
  int chunk_indice = a + b * 3 + c * 9;
  return chunk_indice >= 0 && chunk_indice < 27 ? chunks[a + b * 3 + c * 9]->getBlock(coord) : nullptr;
}

float Region::getMoveCost(Character * c, MapUtil::Vector3 ori, MapUtil::Vector3 dest) {
  MapUtil::Vector3 the_ori = c->getCoord();
  if(ori == dest) {
    return 0.F;
  }
  if(!tryMove(c, dest)) {
    return -1.F;
  }
  if(c->isFlying()) {
    return MapUtil::distance(ori, dest) * 10.F / c->getMovementTimeModifier();
  }
  else {
    MapUtil::Vector3 next = MapUtil::makeVector3(ori.x, ori.y, ori.z);
    float orientation = MapUtil::getOrientationToTarget(ori.x, ori.y, dest.x, dest.y);
    float orientation_z = std::abs(std::acos((dest.z - ori.z) / MapUtil::distance(ori, dest))); // colatitude is non oriented
    float cos_long = std::cos(orientation * 3.141593F / 180.F);
    float sin_long = std::sin(orientation * 3.141593F / 180.F);
    float cos_colat = std::cos(orientation_z);
    float sin_colat = std::sin(orientation_z);
    //
    float factor_x = sin_colat * cos_long;
    float factor_y = sin_colat * sin_long;
    float factor_z = cos_colat;
    float ap_cost = 0.F;
    int x_direction = 1;
    int y_direction = 1;
    int z_direction = 1;
    if(orientation > 180.F) {
      y_direction = -1;
    }
    if(orientation > 90.F && orientation < 270.F) {
      x_direction = -1;
    }
    if(orientation_z > 3.141593F / 2.F && orientation_z < 1.5F * 3.141593F) {
      z_direction = -1;
    }
    bool done = false;
    while(!done) {
      float range;
      MapUtil::Vector3 current = MapUtil::selectClosestVector(next, dest, x_direction, y_direction, z_direction, factor_x, factor_y, factor_z, range);
      Block * block = getBlock(current);
      if(block == nullptr) {
        block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        // no path
        if(block == nullptr) {
          return -1.F;
        }
      }
      float current_range = std::min(MapUtil::distance(next, dest), range);
      if(current_range != range) {
        done = true;
        float range_with_cost = c->getMovementTimeModifier() * current_range / block->ap_cost;
        next.x += factor_x * range_with_cost;
        next.y += factor_y * range_with_cost;
        next.z += factor_z * range_with_cost;
        next = MapUtil::round(next);
      }
      else {
        next = current;
      }
      ap_cost += current_range * block->ap_cost / c->getMovementTimeModifier();
    }
    return MapUtil::round(ap_cost);
  }
}

bool Region::tryMove(Character * c, MapUtil::Vector3 dest) {
  if(c->isEtheral()) {
    return true;
  }
  Block * block = getBlock(dest);
  if(block != nullptr && block->unwalkable) {
    if(!(block->type == BLOCK_SLOPE || block->type == BLOCK_STAIRS)) {
      return false;
    }
    /*
    if(dest.z == std::floor(dest.z)) {
      return false;
    }
    */
  }
  for(Character * other : characters) {
    if(!other->isMarkedDead() && c != other && !other->isEtheral() && MapUtil::distance(dest, other->getCoord()) <= c->getSize() + other->getSize()) {
      return false;
    }
  }
  for(Furniture * furniture : getFurnitures()) {
    if(intersect(c, dest, furniture)) {
      return false;
    }
  }
  return true;
}

bool Region::intersect(Character * character, MapUtil::Vector3 dest, Furniture * furniture) {
  if(!furniture->getUnwalkable() || dest.z < furniture->getCoord().z || dest.z > furniture->getCoord().z + furniture->sizeZ) {
    return false;
  }
  float testX = dest.x;
  float testY = dest.y;
  // test left edge
  if(dest.x < furniture->getCoord().x) {
    testX = furniture->getCoord().x;
  }
  // test left edge
  else if(dest.x > furniture->getCoord().x + furniture->sizeX) {
    testX = furniture->getCoord().x + furniture->sizeX;
  }
  // test left edge
  if(dest.y < furniture->getCoord().y) {
    testY = furniture->getCoord().y;
  }
  // test left edge
  else if(dest.y > furniture->getCoord().y + furniture->sizeY) {
    testY = furniture->getCoord().y + furniture->sizeY;
  }
  float distX = dest.x - testX;
  float distY = dest.y - testY;
  float distance2 = sqrt( (distX*distX) + (distY*distY) );
  if (distance2 <= character->getSize()) {
    return true;
  }
  return false;
}

bool Region::move(Character * c, float orientation, MapUtil::Vector3 dest, float ap, World * world) {
  MapUtil::Vector3 ori = c->getCoord();
  if(ori == dest) {
    return false;
  }
  MapUtil::Vector3 next = MapUtil::makeVector3(ori.x, ori.y, ori.z);
  float orientation_z = std::abs(std::acos((dest.z - ori.z) / MapUtil::distance(ori, dest))); // colatitude is non oriented
  float cos_long = std::cos(orientation * 3.141593F / 180.F);
  float sin_long = std::sin(orientation * 3.141593F / 180.F);
  float cos_colat = std::cos(orientation_z);
  float sin_colat = std::sin(orientation_z);
  //
  float factor_x = sin_colat * cos_long;
  float factor_y = sin_colat * sin_long;
  float factor_z = cos_colat;
  if(c->isFlying()) {
    float range_with_cost = c->getMovementTimeModifier() * ap / 10.F;
    next.x += factor_x * range_with_cost;
    next.y += factor_y * range_with_cost;
    next.z += factor_z * range_with_cost;
    next = MapUtil::round(next);
  }
  else {
    int x_direction = 1;
    int y_direction = 1;
    int z_direction = 1;
    if(orientation > 180.F) {
      y_direction = -1;
    }
    if(orientation > 90.F && orientation < 270.F) {
      x_direction = -1;
    }
    if(orientation_z > 3.141593F / 2.F && orientation_z < 1.5F * 3.141593F) {
      z_direction = -1;
    }
    float ap_cost = 0.F;
    bool done = false; 
    while(!done) {
      float range;
      MapUtil::Vector3 current = MapUtil::selectClosestVector(next, dest, x_direction, y_direction, z_direction, factor_x, factor_y, factor_z, range);
      Block * block = getBlock(current);
      if(block == nullptr) {
        block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        // no path
        if(block == nullptr) {
          return false;
        }
      }
      float border_ap = range * block->ap_cost / c->getMovementTimeModifier();
      float current_ap = std::min(ap - ap_cost, border_ap);
      if(current_ap != border_ap) {
        done = true;
        float range_with_cost = c->getMovementTimeModifier() * current_ap / block->ap_cost;
        next.x += factor_x * range_with_cost;
        next.y += factor_y * range_with_cost;
        next.z += factor_z * range_with_cost;
        next = MapUtil::round(next);
      }
      else {
        next = current;
      }
      ap_cost += current_ap;
    }
  }
  // check if we went too far
  if(MapUtil::distance(ori, dest) < MapUtil::distance(ori, next)) {
    next = MapUtil::round(dest);
  }
  if(tryMove(c, next)) {
    c->move(next, orientation, world);
    return true;
  }
  else {
    return false;
  }
}

MapUtil::Vector3 Region::getCoordsOnSlope(Character * character, MapUtil::Vector3 dest, int orientation, float z) {
  MapUtil::Vector3 result = MapUtil::makeVector3(dest.x, dest.y, character->getCoord().z);
  switch(orientation) {
    case 0:
      result.z = MapUtil::round(z + (result.x - std::floor(result.x)));
      break;
    case 90:
      result.z = MapUtil::round(z + (result.y - std::floor(result.y)));
      break;
    case 180:
      result.z = MapUtil::round(z + 1 - (result.x - std::floor(result.x)));
      break;
    case 270:
      result.z = MapUtil::round(z + 1 - (result.y - std::floor(result.y)));
      break;
  }
  return result;
}

bool Region::canSee(Character * watcher, Character * target) {
  if(target->isInvisible() || target->isEtheral()) {
    return false;
  }
  MapUtil::Vector3 watcher_head = watcher->getCoord();
  watcher_head.z += watcher->getHeight();
  MapUtil::Vector3 target_head = target->getCoord();
  target_head.z += target->getHeight();
  float distance2 = MapUtil::distance2(watcher_head, target_head);
  MapUtil::Vector3 direction = MapUtil::makeVector3((watcher_head.x - target_head.x) / distance2, (watcher_head.y - target_head.y) / distance2, (watcher_head.y - target_head.y) / distance2);
  for(int step = 1; step < distance2; step++) {
    MapUtil::Vector3 coord = MapUtil::makeVector3(watcher_head.x + step * direction.x, watcher_head.y + step * direction.y, watcher_head.z + step * direction.z);
    Block * block = getBlock(coord);
    if(block != nullptr && block->opaque) {
      return false;
    }
  }
  return true;
}

std::list<Character *> Region::getCharacters() { return characters; }

std::list<Character *> Region::getCharacters(Character * character) {
  std::list<Character *> result = std::list<Character *>();
  for(Character * c : characters) {
    if(canSee(character, c)) {
      result.push_back(c);
    }
  }
  return result;
}

std::list<Furniture *> Region::getFurnitures() {
  std::list<Furniture *> result = std::list<Furniture *>();
  for(BlocksChunk * chunk : chunks) {
    for(Furniture * furniture : chunk->getFurnitures()) {
      result.push_front(furniture);
    }
  }
  return result;
}

std::list<Furniture *> Region::getFurnitures(Character * character) {
  std::list<Furniture *> result = std::list<Furniture *>();
  for(BlocksChunk * chunk : chunks) {
    for(Furniture * furniture : chunk->getFurnitures()) {
      result.push_front(furniture);
    }
  }
  return result;
}

std::map<const MapUtil::Vector3i, Block *> Region::getBlocks() {
  std::map<const MapUtil::Vector3i, Block *> result = std::map<const MapUtil::Vector3i, Block *>();
  for(BlocksChunk * chunk : chunks) {
    for(auto pair : chunk->getBlocks()) {
      result.insert(pair);
    }
  }
  return result;
}

Furniture * Region::getFurniture(MapUtil::Vector3i coord) {
  for(BlocksChunk * chunk : chunks) {
    for(Furniture * furniture : chunk->getFurnitures()) {
      if(furniture->getCoord() == coord) {
        return furniture;
      }
    }
  }
  return nullptr;
}
