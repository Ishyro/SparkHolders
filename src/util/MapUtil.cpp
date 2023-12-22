#include <algorithm>
#include <list>

#include "data/Map.h"

#include "util/MapUtil.h"

MapUtil::Vector3 MapUtil::makeVector3(float x, float y, float z) {
  MapUtil::Vector3 coord = MapUtil::Vector3();
  coord.x = x;
  coord.y = y;
  coord.z = z;
  return coord;
}

MapUtil::Vector3 MapUtil::makeVector3(MapUtil::Vector3i coord_int) {
  MapUtil::Vector3 coord = MapUtil::Vector3();
  coord.x = coord_int.x;
  coord.y = coord_int.y;
  coord.z = coord_int.z;
  return coord;
}

MapUtil::Vector3i MapUtil::makeVector3i(int x, int y, int z) {
  MapUtil::Vector3i coord = MapUtil::Vector3i();
  coord.x = x;
  coord.y = y;
  coord.z = z;
  return coord;
}

MapUtil::Vector3i MapUtil::makeVector3i(MapUtil::Vector3 coord_float) {
  MapUtil::Vector3i coord = MapUtil::Vector3i();
  coord.x = (int) std::floor(coord_float.x);
  coord.y = (int) std::floor(coord_float.y);
  coord.z = (int) std::floor(coord_float.z);
  return coord;
}

float MapUtil::round(float var) {
  float value = (int) std::floor(var * 1000.F + .5F);
  value /= 1000.F;
  return value > 0.999F && value < 1.F ? 0.999F : value;
}

float MapUtil::distanceSquare(float x1, float y1, float x2, float y2) {
  return std::max(std::abs(x2 - x1), std::abs(y2 - y1));
}

float MapUtil::distance(float x1, float y1, float x2, float y2) {
  return round(std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

float MapUtil::distance(MapUtil::Vector3 coord1, MapUtil::Vector3 coord2) {
  return round(std::sqrt((coord1.x - coord2.x) * (coord1.x - coord2.x) + (coord1.y - coord2.y) * (coord1.y - coord2.y) + (coord1.z - coord2.z) * (coord1.z - coord2.z)));
}

float MapUtil::getOrientationToTarget(float x1, float y1, float x2, float y2) {
  if(y1 == y2) {
    if(x1 < x2) {
      return 0.F;
    }
    if(x1 > x2) {
      return 180.F;
    }
    if(x1 == x2) {
      return 360.F;
    }
  }
  if(x1 == x2) {
    if(y1 < y2) {
      return 90.F;
    }
    if(y1 > y2) {
      return 270.F;
    }
  }
  float angle = std::atan2((y2 - y1), (x2 - x1));
  if(angle < 0) {
    angle += 2 * 3.141593F;
  }
  return angle * 180.F / 3.141593F;
}

std::vector<MapUtil::Pair> MapUtil::reconstruct_path(std::vector<std::vector<MapUtil::Pair>> cameFrom, MapUtil::Pair start, MapUtil::Pair dest, int offsetX, int offsetY) {
  std::list<MapUtil::Pair> work_list = std::list<MapUtil::Pair>();
  if(start == dest) {
    return std::vector<MapUtil::Pair>();
  }
  for(MapUtil::Pair current = dest; current != start; current = cameFrom[current.y - offsetY][current.x - offsetX]) {
    work_list.push_front(current);
  }
  std::vector<MapUtil::Pair> result = std::vector<MapUtil::Pair>(work_list.size());
  std::copy(work_list.begin(), work_list.end(), result.begin());
  return result;
}

float MapUtil::reconstruct_orientation(std::vector<std::vector<MapUtil::Pair>> cameFrom, MapUtil::Pair start, MapUtil::Pair dest, int offsetX, int offsetY) {
  if(start == dest) {
    return 360.F;
  }
  MapUtil::Pair previous = dest;
  for(MapUtil::Pair current = dest; current != start; current = cameFrom[current.y - offsetY][current.x - offsetX]) {
    previous = current;
  }
  return getOrientationToTarget(start.x, start.y, previous.x, previous.y);
}

/*
std::list<MapUtil::Pair> MapUtil::getNeighboursTraversable(Map * map, int startX, int startY, int destX, int destY) {
  std::list<MapUtil::Pair> result = std::list<MapUtil::Pair>();
  MapUtil::Pair next = MapUtil::Pair();
  if(startY > map->offsetY) {
    if(!map->getBlock(startX, startY - 1)->unwalkable) {
      next.x = startX;
      next.y = startY - 1;
      result.push_back(next);
    }
    if(startY < map->sizeY + map->offsetY - 1) {
      if(!map->getBlock(startX, startY + 1)->unwalkable) {
        next.x = startX;
        next.y = startY + 1;
        result.push_back(next);
      }
    }
  }
  if(startX > map->offsetX) {
    if(!map->getBlock(startX - 1, startY)->unwalkable) {
      next.x = startX - 1;
      next.y = startY;
      result.push_back(next);
    }
    if(startX < map->sizeX + map->offsetX - 1) {
      if(!map->getBlock(startX + 1, startY)->unwalkable) {
        next.x = startX + 1;
        next.y = startY;
        result.push_back(next);
      }
      if(startY > map->offsetY) {
        if(!map->getBlock(startX + 1, startY - 1)->unwalkable) {
          next.x = startX + 1;
          next.y = startY - 1;
          result.push_back(next);
        }
      }
      if(startY < map->sizeY + map->offsetY - 1) {
        if(!map->getBlock(startX + 1, startY + 1)->unwalkable) {
          next.x = startX + 1;
          next.y = startY + 1;
          result.push_back(next);
        }
      }
    }
  }
  if(startX > map->offsetX) {
    if(startY > map->offsetY) {
      if(!map->getBlock(startX - 1, startY - 1)->unwalkable) {
        next.x = startX - 1;
        next.y = startY - 1;
        result.push_back(next);
      }
      if(startY < map->sizeY + map->offsetY - 1) {
        if(!map->getBlock(startX - 1, startY + 1)->unwalkable) {
          next.x = startX - 1;
          next.y = startY + 1;
          result.push_back(next);
        }
      }
    }
  }
  return result;
}

std::list<MapUtil::Pair> MapUtil::getNeighboursNonSolid(Map * map, int startX, int startY, int destX, int destY) {
  std::list<MapUtil::Pair> result = std::list<MapUtil::Pair>();
  MapUtil::Pair next = MapUtil::Pair();
  if(startY > map->offsetY) {
    if(!map->getBlock(startX, startY - 1)->solid) {
      next.x = startX;
      next.y = startY - 1;
      result.push_back(next);
    }
    if(startY < map->sizeY + map->offsetY - 1) {
      if(!map->getBlock(startX, startY + 1)->solid) {
        next.x = startX;
        next.y = startY + 1;
        result.push_back(next);
      }
    }
  }
  if(startX > map->offsetX) {
    if(!map->getBlock(startX - 1, startY)->solid) {
      next.x = startX - 1;
      next.y = startY;
      result.push_back(next);
    }
    if(startX < map->sizeX + map->offsetX - 1) {
      if(!map->getBlock(startX + 1, startY)->solid) {
        next.x = startX + 1;
        next.y = startY;
        result.push_back(next);
      }
      if(startY > map->offsetY) {
        if(!map->getBlock(startX + 1, startY - 1)->solid) {
          next.x = startX + 1;
          next.y = startY - 1;
          result.push_back(next);
        }
      }
      if(startY < map->sizeY + map->offsetY - 1) {
        if(!map->getBlock(startX + 1, startY + 1)->solid) {
          next.x = startX + 1;
          next.y = startY + 1;
          result.push_back(next);
        }
      }
    }
  }
  if(startX > map->offsetX) {
    if(startY > map->offsetY) {
      if(!map->getBlock(startX - 1, startY - 1)->solid) {
        next.x = startX - 1;
        next.y = startY - 1;
        result.push_back(next);
      }
      if(startY < map->sizeY + map->offsetY - 1) {
        if(!map->getBlock(startX - 1, startY + 1)->solid) {
          next.x = startX - 1;
          next.y = startY + 1;
          result.push_back(next);
        }
      }
    }
  }
  return result;
}
*/

std::vector<MapUtil::Pair> MapUtil::getPathToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying) {
  return std::vector<MapUtil::Pair>();
  /*
  int start_x = startX - map->offsetX;
  int start_y = startY - map->offsetY;
  std::list<MapUtil::Pair> (*getNeighbours)(Map *, int, int, int, int){ &getNeighboursTraversable };
  if(flying) {
    getNeighbours = &getNeighboursNonSolid;
  }
  MapUtil::Pair start = MapUtil::Pair();
  start.x = startX;
  start.y = startY;
  start.score = distance(startX, startY, destX, destY);
  MapUtil::Pair dest = MapUtil::Pair();
  dest.x = destX;
  dest.y = destY;
  dest.score = 0;
  if(start == dest) {
    return std::vector<MapUtil::Pair>();
  }
  std::list<MapUtil::Pair> openSet = std::list<MapUtil::Pair>();
  std::vector<std::vector<MapUtil::Pair>> cameFrom = std::vector<std::vector<MapUtil::Pair>>(map->sizeY);
  std::vector<std::vector<int>> gCost = std::vector<std::vector<int>>(map->sizeY);
  for(int i = 0; i < map->sizeY; i++) {
    cameFrom[i] = std::vector<MapUtil::Pair>(map->sizeX);
    gCost[i] = std::vector<int>(map->sizeX);
  }
  int max = map->sizeX * map->sizeY;
  for(int i = 0; i < map->sizeY; i++) {
    for(int j = 0; j < map->sizeX; j++) {
      gCost[i][j] = max;
    }
  }
  openSet.push_front(start);
  gCost[start_y][start_x] = 0;

  while(!openSet.empty()) {
    MapUtil::Pair current = openSet.front();
    openSet.pop_front();
    if(current.x == destX && current.y == destY) {
      return reconstruct_path(cameFrom, start, dest, map->offsetX, map->offsetY);
    }
    for(MapUtil::Pair pair : getNeighbours(map, current.x, current.y, destX, destY)) {
      int tentative_gScore = gCost[current.y - map->offsetY][current.x - map->offsetX] + 1;
      if(gCost[pair.y - map->offsetY][pair.x - map->offsetX] > tentative_gScore) {
        gCost[pair.y - map->offsetY][pair.x - map->offsetX] = tentative_gScore;
        cameFrom[pair.y - map->offsetY][pair.x - map->offsetX] = current;
        pair.score = tentative_gScore + distance(pair.x, pair.y, destX, destY);
        bool found = false;
        for(MapUtil::Pair pair2 : openSet) {
          if(pair == pair2) {
            found = true;
            break;
          }
        }
        if(!found) {
          std::list<MapUtil::Pair> sorted_element = std::list<MapUtil::Pair>(1);
          sorted_element.push_front(pair);
          openSet.merge(sorted_element);
        }
      }
    }
  }
  return std::vector<MapUtil::Pair>();
  */
}

float MapUtil::getOrientationToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying) {
  return 0.F;
  /*
  std::list<MapUtil::Pair> (*getNeighbours)(Map *, int, int, int, int){ &getNeighboursTraversable };
  if(flying) {
    getNeighbours = &getNeighboursNonSolid;
  }
  MapUtil::Pair start = MapUtil::Pair();
  start.x = startX;
  start.y = startY;
  start.score = distance(startX, startY, destX, destY);
  MapUtil::Pair dest = MapUtil::Pair();
  dest.x = destX;
  dest.y = destY;
  dest.score = 0;
  if(start == dest) {
    return 360.F;
  }
  std::list<MapUtil::Pair> openSet = std::list<MapUtil::Pair>();
  std::vector<std::vector<MapUtil::Pair>> cameFrom = std::vector<std::vector<MapUtil::Pair>>(map->sizeY);
  std::vector<std::vector<int>> gCost = std::vector<std::vector<int>>(map->sizeY);
  for(int i = 0; i < map->sizeY; i++) {
    cameFrom[i] = std::vector<MapUtil::Pair>(map->sizeX);
    gCost[i] = std::vector<int>(map->sizeX);
  }
  int max = map->sizeX * map->sizeY;
  for(int i = 0; i < map->sizeY; i++) {
    for(int j = 0; j < map->sizeX; j++) {
      gCost[i][j] = max;
    }
  }
  openSet.push_front(start);
  gCost[startY][startX] = 0;

  while(!openSet.empty()) {
    MapUtil::Pair current = openSet.front();
    openSet.pop_front();
    if(current.x == destX && current.y == destY) {
      return reconstruct_orientation(cameFrom, start, dest, map->offsetX, map->offsetY);
    }
    for(MapUtil::Pair pair : getNeighbours(map, current.x, current.y, destX, destY)) {
      int tentative_gScore = gCost[current.y][current.x] + 1;
      if(gCost[pair.y][pair.x] > tentative_gScore) {
        gCost[pair.y][pair.x] = tentative_gScore;
        cameFrom[pair.y][pair.x] = current;
        pair.score = tentative_gScore + distance(pair.x, pair.y, destX, destY);
        bool found = false;
        for(MapUtil::Pair pair2 : openSet) {
          if(pair == pair2) {
            found = true;
            break;
          }
        }
        if(!found) {
          std::list<MapUtil::Pair> sorted_element = std::list<MapUtil::Pair>(1);
          sorted_element.push_front(pair);
          openSet.merge(sorted_element);
        }
      }
    }
  }
  return 360.F;
  */
}

// 0 <= a <= 1
// y <= x
std::list<MapUtil::Pair> MapUtil::getPathFromCartesianEquation(float a, int range) {
  MapUtil::Pair pair;
  std::list<MapUtil::Pair> result = std::list<MapUtil::Pair>();
  for(pair.x = 0; pair.x <= range; pair.x++) {
    pair.y = std::floor(a * (float) pair.x);
    result.push_back(pair);
  }
  return result;
}

std::list<MapUtil::Pair> MapUtil::getPathFromOrientation(float x, float y, float orientation, float size, float range) {
  MapUtil::Pair pair;
  std::list<MapUtil::Pair> result = std::list<MapUtil::Pair>();
  float high_limit = round(0.999F - size);
  float low_limit = round(size);
  pair.x = x;
  pair.y = y;
  int x_direction = 1;
  int y_direction = 1;
  if(orientation > 180.F) {
    y_direction = -1;
  }
  if(orientation > 90.F && orientation < 270.F) {
    x_direction = -1;
  }
  pair.x = std::floor(x) + (x_direction == 1 ? high_limit : low_limit);
  pair.y = std::floor(y) + (y_direction == 1 ? high_limit : low_limit);
  if(orientation == 0.F) {
    pair.y = y;
    result.push_back(pair);
    for(int i = 0; i < std::ceil(range); i++) {
      pair.x = pair.x + 1.F;
      result.push_back(pair);
    }
  }
  else if(orientation == 180.F) {
    pair.y = y;
    result.push_back(pair);
    for(int i = 0; i < std::ceil(range); i++) {
      pair.x = pair.x - 1.F;
      result.push_back(pair);
    }
  }
  else if(orientation == 90.F) {
    pair.x = x;
    result.push_back(pair);
    for(int i = 0; i < std::ceil(range); i++) {
      pair.y = pair.y + 1.F;
      result.push_back(pair);
    }
  }
  else if(orientation == 270.F) {
    pair.x = x;
    result.push_back(pair);
    for(int i = 0; i < std::ceil(range); i++) {
      pair.y = pair.y - 1.F;
      result.push_back(pair);
    }
  }
  else {
    float cos = std::cos(orientation * 3.141593F / 180.F);
    float sin = std::sin(orientation * 3.141593F / 180.F);
    float dx = x - std::floor(x);
    float dy = y - std::floor(y);
    float current_range;
    float range_x = std::abs(-dx / cos);
    float range_y = std::abs(-dy / sin);
    float x_x = x + cos * range_x;
    float x_y = y + sin * range_x;
    float y_x = x + cos * range_y;
    float y_y = y + sin * range_y;
    if(distanceSquare(x, y, x_x, x_y) < distanceSquare(x, y, x_x, x_y)) {
      pair.x = x_x;
      pair.y = x_y;
      current_range = range_x;
    }
    else {
      pair.x = y_x;
      pair.y = y_y;
      current_range = range_y;
    }
    result.push_back(pair);
    while(current_range < range) {
      float current_dx = pair.x - std::floor(pair.x);
      float current_dy = pair.y - std::floor(pair.y);
      range_x = std::abs((x_direction - current_dx) / cos);
      range_y = std::abs((y_direction - current_dy) / sin);
      x_x = pair.x + cos * range_x;
      x_y = pair.y + sin * range_x;
      y_x = pair.x + cos * range_y;
      y_y = pair.y + sin * range_y;
      if(distanceSquare(x, y, x_x, x_y) < distanceSquare(x, y, x_x, x_y)) {
        pair.x = x_x;
        pair.y = x_y;
        current_range += range_x;
      }
      else {
        pair.x = y_x;
        pair.y = y_y;
        current_range += range_y;
      }
      bool change_x = false;
      if(x_direction == 1 && current_dx == low_limit) {
        pair.x = std::floor(pair.x) + high_limit - x_direction;
        change_x = true;
      }
      bool change_y = false;
      if(y_direction == 1 && current_dy == low_limit) {
        pair.y = std::floor(pair.y) + high_limit - y_direction;
        change_y = true;
      }
      result.push_back(pair);
      if(change_x) {
        pair.x += x_direction + low_limit;
      }
      if(change_y) {
        pair.y += y_direction + low_limit;
      }
    }
  }
  return result;
}
