#include "data/Region.h"

#include <iostream>

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
  int a = (int) std::floor((coord.x - id.x) / CHUNK_SIZE) + 1;
  int b = (int) std::floor((coord.y - id.y) / CHUNK_SIZE) + 1;
  int c = (int) std::floor((coord.z - id.z) / CHUNK_SIZE) + 1;
  return chunks[a + b * 3 + c * 9]->getBlock(coord);
}


float Region::getMoveCost(Character * c, MapUtil::Vector3 ori, MapUtil::Vector3 dest) {
  Block * block = getBlock(MapUtil::makeVector3i(dest));
  if(block != nullptr && block->unwalkable) {
    return -5.F;
  }
  Block * under = getBlock(MapUtil::makeVector3i(dest.x, dest.y - 1, dest.z));
  if(under == nullptr || under->unwalkable) {
    return -10.F;
  }
  if(ori == dest) {
    return -15.F;
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
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance(current, next);
      }
      current.x = next.x;
    }
    while(std::floor(current.x) != std::floor(dest.x)) {
      next.x = std::floor(current.x + x_direction);
      if(x_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y - 1, next.z));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance(current, next);
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance(current, next);
        }
      }
      current.x = next.x;
    }
    if(x_direction == -1) {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
      if(block != nullptr) {
        ap_cost -= block->ap_cost * MapUtil::distance(current, dest);
      }
    }
    else {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance(current, dest);
      }
    }
  }
  else if(ori.x == dest.x) {
    if(current.y != dest.y && y_direction == -1) {
      next.y = std::floor(next.y);
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance(current, next);
      }
      current.y = next.y;
    }
    while(std::floor(current.y) != std::floor(dest.y)) {
      next.y = std::floor(current.y + y_direction);
      if(y_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y - 1, next.z));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance(current, next);
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance(current, next);
        }
      }
      current.y = next.y;
    }
    if(y_direction == -1) {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
      if(block != nullptr) {
        ap_cost -= block->ap_cost * MapUtil::distance(current, dest);
      }
    }
    else {
      Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
      if(block != nullptr) {
        ap_cost += block->ap_cost * MapUtil::distance(current, dest);
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
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y - 1, next.z));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance(current, next);
        }
        current.x = next.x;
        current.y = next.y;
      }
      while(std::floor(current.x) != std::floor(dest.x)) {
        next.x = std::floor(current.x) + x_direction;
        next.y = a * next.x + b;
        if(x_direction == -1) {
          Block * block = getBlock(MapUtil::makeVector3(next.x, next.y - 1, next.z));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance(current, next);
          }
        }
        else {
          Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance(current, next);
          }
        }
        current.x = next.x;
        current.y = next.y;
      }
      if(x_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
        if(block != nullptr) {
          ap_cost -= block->ap_cost * MapUtil::distance(current, dest);
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance(current, dest);
        }
      }
    }
    else {
      if(y_direction == -1) {
        next.y = std::floor(current.y);
        next.x = (next.y - b) / a;
        Block * block = getBlock(MapUtil::makeVector3(next.x, next.y - 1, next.z));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance(current, next);
        }
        current.x = next.x;
        current.y = next.y;
      }
      while(std::floor(current.y) != std::floor(dest.y)) {
        next.y = std::floor(current.y) + y_direction;
        next.x = (next.y - b) / a;
        if(y_direction == -1) {
          Block * block = getBlock(MapUtil::makeVector3(next.x, next.y - 1, next.z));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance(current, next);
          }
        }
        else {
          Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
          if(block != nullptr) {
            ap_cost += block->ap_cost * MapUtil::distance(current, next);
          }
        }
        current.x = next.x;
        current.y = next.y;
      }
      if(y_direction == -1) {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
        if(block != nullptr) {
          ap_cost -= block->ap_cost * MapUtil::distance(current, dest);
        }
      }
      else {
        Block * block = getBlock(MapUtil::makeVector3(current.x, current.y - 1, current.z));
        if(block != nullptr) {
          ap_cost += block->ap_cost * MapUtil::distance(current, dest);
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
    return false;
  }
  for(Character * other : characters) {
    if(!other->isMarkedDead() && c != other && !other->isEtheral() && MapUtil::distance(dest, other->getCoord()) <= c->getSize() + other->getSize()) {
      return false;
    }
  }
  for(Furniture * furniture : getFurnitures()) {
    // TODO
    if(furniture->getUnwalkable() && MapUtil::distance(dest, MapUtil::makeVector3(furniture->getCoord())) <= c->getSize() + furniture->sizeX) {
      return false;
    }
  }
  return true;
}

float Region::move(Character * c, float orientation, MapUtil::Vector3 dest, float ap, World * world) {

}

bool Region::canSee(Character * watcher, Character * target) {
  if(target->isInvisible() || target->isEtheral()) {
    return false;
  }
  MapUtil::Vector3 watcher_head = watcher->getCoord();
  watcher_head.z += watcher->getHeight();
  MapUtil::Vector3 target_head = target->getCoord();
  target_head.z += target->getHeight();
  float distance = MapUtil::distance(watcher_head, target_head);
  MapUtil::Vector3 direction = MapUtil::makeVector3((watcher_head.x - target_head.x) / distance, (watcher_head.y - target_head.y) / distance, (watcher_head.y - target_head.y) / distance);
  for(int step = 1; step < distance; step++) {
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
