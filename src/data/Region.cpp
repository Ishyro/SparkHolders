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
  return getBlock(MapUtil::makeVector3i(coord));
}

Block * Region::getBlock(MapUtil::Vector3i coord) {
  int a = (int) std::floor( (float) (coord.x - id.x) / (float) CHUNK_SIZE) + 1;
  int b = (int) std::floor( (float) (coord.y - id.y) / (float) CHUNK_SIZE) + 1;
  int c = (int) std::floor( (float) (coord.z - id.z) / (float) CHUNK_SIZE) + 1;
  return chunks[a + b * 3 + c * 9]->getBlock(coord);
}


float Region::getMoveCost(Character * c, MapUtil::Vector3 ori, MapUtil::Vector3 dest) {
  Block * block = getBlock(MapUtil::makeVector3i(dest));
  if(block != nullptr && block->type != BLOCK_SLOPE && block->type != BLOCK_STAIRS) {
    return -1.F;
  }
  Block * under = getBlock(MapUtil::makeVector3i(dest.x, dest.y, dest.z - 1));
  if(under == nullptr || under->unwalkable) {
    return -1.F;
  }
  if(ori == dest) {
    return -1.F;
  }
  if(c->isFlying()) {
    return MapUtil::distance(ori, dest) * 10.F / c->getMovementTimeModifier();
  }
  float orientation = MapUtil::getOrientationToTarget(ori.x, ori.y, dest.x, dest.y);
  float ap_cost = 0.F;
  MapUtil::Vector3 current = MapUtil::makeVector3(ori.x, ori.y, ori.z);
  MapUtil::Vector3 next = MapUtil::makeVector3(ori.x, ori.y, ori.z);
  int x_direction = 1;
  int y_direction = 1;
  if(orientation > 180.F) {
    y_direction = -1;
  }
  if(orientation > 90.F && orientation < 270.F) {
    x_direction = -1;
  }
  if(ori.y == dest.y) {
    if(current.x != dest.x && x_direction == -1) {
      next.x = std::floor(next.x);
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance2(current, next);
      }
      current.x = next.x;
    }
    while(std::floor(current.x) != std::floor(dest.x)) {
      next.x = std::floor(current.x + x_direction);
      if(x_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next);
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next);
        }
      }
      current.x = next.x;
    }
    if(x_direction == -1) {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost -= block->ap_cost * MapUtil::distance2(current, dest);
      }
    }
    else {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance2(current, dest);
      }
    }
  }
  else if(ori.x == dest.x) {
    if(current.y != dest.y && y_direction == -1) {
      next.y = std::floor(next.y);
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance2(current, next);
      }
      current.y = next.y;
    }
    while(std::floor(current.y) != std::floor(dest.y)) {
      next.y = std::floor(current.y + y_direction);
      if(y_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next);
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next);
        }
      }
      current.y = next.y;
    }
    if(y_direction == -1) {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost -= block->ap_cost * MapUtil::distance2(current, dest);
      }
    }
    else {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance2(current, dest);
      }
    }
  }
  else {
    float a = (dest.y - ori.y) / (dest.x - ori.x);
    float b = dest.y - a * dest.x;
    if(abs(a) < 1) {
      if(x_direction == -1) {
        next.x = std::floor(current.x);
        next.y = a * next.x + b;
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next);
        }
        current.x = next.x;
        current.y = next.y;
      }
      while(std::floor(current.x) != std::floor(dest.x)) {
        next.x = std::floor(current.x) + x_direction;
        next.y = a * next.x + b;
        if(x_direction == -1) {
          Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance2(current, next);
          }
        }
        else {
          Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance2(current, next);
          }
        }
        current.x = next.x;
        current.y = next.y;
      }
      if(x_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost -= block->ap_cost * MapUtil::distance2(current, dest);
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, dest);
        }
      }
    }
    else {
      if(y_direction == -1) {
        next.y = std::floor(current.y);
        next.x = (next.y - b) / a;
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next);
        }
        current.x = next.x;
        current.y = next.y;
      }
      while(std::floor(current.y) != std::floor(dest.y)) {
        next.y = std::floor(current.y) + y_direction;
        next.x = (next.y - b) / a;
        if(y_direction == -1) {
          Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance2(current, next);
          }
        }
        else {
          Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance2(current, next);
          }
        }
        current.x = next.x;
        current.y = next.y;
      }
      if(y_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost -= block->ap_cost * MapUtil::distance2(current, dest);
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, dest);
        }
      }
    }
  }
  return ap_cost / c->getMovementTimeModifier();
}

bool Region::tryMove(Character * c, MapUtil::Vector3 dest) {
  if(c->isEtheral()) {
    return true;
  }
  Block * block = getBlock(MapUtil::makeVector3i(dest));
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
  Block * block = getBlock(MapUtil::makeVector3i(dest));
  if(block != nullptr && block->type != BLOCK_SLOPE && block->type != BLOCK_STAIRS) {
    return false;
  }
  Block * under = getBlock(MapUtil::makeVector3i(dest.x, dest.y, dest.z - 1));
  if(under == nullptr || under->unwalkable) {
    return false;
  }
  MapUtil::Vector3 ori = c->getCoord();
  if(ori == dest) {
    return false;
  }

  if(c->isFlying()) {
    return MapUtil::distance(ori, dest) * 10.F / c->getMovementTimeModifier();
  }

  float ap_cost = 0.F;
  MapUtil::Vector3 current = MapUtil::makeVector3(ori.x, ori.y, ori.z);
  MapUtil::Vector3 next = MapUtil::makeVector3(ori.x, ori.y, ori.z);
  int x_direction = 1;
  int y_direction = 1;
  if(orientation > 180.F) {
    y_direction = -1;
  }
  if(orientation > 90.F && orientation < 270.F) {
    x_direction = -1;
  }
  if(ori.y == dest.y) {
    if(current.x != dest.x && x_direction == -1) {
      next.x = std::floor(next.x);
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
      }
      current.x = next.x;
    }
    while(std::floor(current.x) != std::floor(dest.x) && ap_cost < ap) {
      next.x = std::floor(current.x + x_direction);
      if(x_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
        }
      }
      current.x = next.x;
    }
    if(ap_cost < ap) {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost += x_direction* block->ap_cost * MapUtil::distance2(current, dest) / c->getMovementTimeModifier();
        current.x = dest.x;
      }
    }
    if(ap_cost > ap) {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        current.x = current.x - (float) x_direction * (ap_cost - ap) / block->ap_cost * c->getMovementTimeModifier();
        ap_cost = ap;
      }
    }
  }
  else if(ori.x == dest.x) {
    if(current.y != dest.y && y_direction == -1) {
      next.y = std::floor(next.y);
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
      }
      current.y = next.y;
    }
    while(std::floor(current.y) != std::floor(dest.y) && ap_cost < ap) {
      next.y = std::floor(current.y + y_direction);
      if(y_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
        }
      }
      current.y = next.y;
    }
    if(ap_cost < ap) {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        ap_cost += y_direction * block->ap_cost * MapUtil::distance2(current, dest) / c->getMovementTimeModifier();
        current.y = dest.y;
      }
    }
    if(ap_cost > ap) {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
      if(block != nullptr) {
        current.y = current.y - (float) y_direction * (ap_cost - ap) / block->ap_cost * c->getMovementTimeModifier();
        ap_cost = ap;
      }
    }
  }
  else {
    float a = (dest.y - ori.y) / (dest.x - ori.x);
    float b = dest.y - a * dest.x;
    if(abs(a) < 1) {
      if(x_direction == -1) {
        next.x = std::floor(current.x);
        next.y = a * next.x + b;
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
        }
        current.x = next.x;
        current.y = next.y;
      }
      while(std::floor(current.x) != std::floor(dest.x) && ap_cost < ap) {
        next.x = std::floor(current.x) + x_direction;
        next.y = a * next.x + b;
        if(x_direction == -1) {
          Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
          }
        }
        else {
          Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
          }
        }
        current.x = next.x;
        current.y = next.y;
      }
      if(ap_cost < ap) {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost += x_direction * block->ap_cost * MapUtil::distance2(current, dest) / c->getMovementTimeModifier();
          current.x = dest.x;
          current.y = dest.y;
        }
      }
      if(ap_cost > ap) {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          current.x = current.x - ((ap_cost - ap) / block->ap_cost) * c->getMovementTimeModifier() * std::cos(orientation * 3.141593F / 180.F);
          current.y = current.y - ((ap_cost - ap) / block->ap_cost) * c->getMovementTimeModifier() * std::sin(orientation * 3.141593F / 180.F);
          ap_cost = ap;
        }
      }
    }
    else {
      if(y_direction == -1) {
        next.y = std::floor(current.y);
        next.x = (next.y - b) / a;
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
        }
        current.x = next.x;
        current.y = next.y;
      }
      while(std::floor(current.y) != std::floor(dest.y) && ap_cost < ap) {
        next.y = std::floor(current.y) + y_direction;
        next.x = (next.y - b) / a;
        if(y_direction == -1) {
          Block * block = getBlock(MapUtil::makeVector3(next.x, next.y, next.z - 1));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
          }
        }
        else {
          Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance2(current, next) / c->getMovementTimeModifier();
          }
        }
        current.x = next.x;
        current.y = next.y;
      }
      if(ap_cost < ap) {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          ap_cost += y_direction * block->ap_cost * MapUtil::distance2(current, dest) / c->getMovementTimeModifier();
          current.x = dest.x;
          current.y = dest.y;
        }
      }
      if(ap_cost > ap) {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
        if(block != nullptr) {
          current.x = current.x - ((ap_cost - ap) / block->ap_cost) * c->getMovementTimeModifier() * std::cos(orientation * 3.141593F / 180.F);
          current.y = current.y - ((ap_cost - ap) / block->ap_cost) * c->getMovementTimeModifier() * std::sin(orientation * 3.141593F / 180.F);
          ap_cost = ap;
        }
      }
    }
  }
  current = MapUtil::makeVector3(MapUtil::round(current.x), MapUtil::round(current.y), MapUtil::round(current.z)); 
  // check if we went past the target coords
  if(MapUtil::distance2(c->getCoord(), current) > MapUtil::distance2(c->getCoord(), dest)) {
    current = dest;
  }
  block = getBlock(current);
  // we went inside a block after leaving a slope / stairs
  if(block != nullptr && block->type == BLOCK_SOLID) {
    current.z = std::floor(current.z) + 1; 
    block = getBlock(current);
  }
  under = getBlock(MapUtil::makeVector3(current.x, current.y, current.z - 1));
  float z_block;
  std::cout << "asked: " << dest.x << " " << dest.y << " " << dest.z << std::endl;
  std::cout << "coord: " << current.x << " " << current.y << " " << current.z << std::endl;
  if(block != nullptr && (block->type == BLOCK_SLOPE || block->type == BLOCK_STAIRS)) {
    z_block = std::floor(current.z);
    current.z = dest.z;
    current = getCoordsOnSlope(c, current, block->orientation, z_block); 
  }
  else if(under != nullptr && (under->type == BLOCK_SLOPE || under->type == BLOCK_STAIRS)) {
    z_block = std::floor(current.z - 1);
    current.z = dest.z;
    current = getCoordsOnSlope(c, current, under->orientation, z_block); 
  }
  if(tryMove(c, current)) {
    c->move(current, orientation, world);
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
  std::cout << "coord_fixed: " << result.x << " " << result.y << " " << result.z << std::endl;
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
