#include <algorithm>
#include <list>
#include <cmath>

#include "data/Map.h"

#include "utils/MapUtil.h"

float MapUtil::round(float var) {
  float value = (int) (var * 1000.F + .5F);
  value /= 1000.F;
  return value > 0.999F && value < 1.F ? 0.999F : value;
}

int MapUtil::distance(int x1, int y1, int x2, int y2) {
  return std::max(std::abs(x2 - x1), std::abs(y2 - y1));
}

float MapUtil::distance(int x1, int y1, float dx1, float dy1, int x2, int y2, float dx2, float dy2) {
  return round(std::sqrt(( (float) x1 + dx1 - (float) x2 - dx2) * ( (float) x1 + dx1 - (float) x2 - dx2) + ( (float) y1 + dy1 - (float) y2 - dy2) * ( (float) y1 + dy1 - (float) y2 - dy2)));
}

float MapUtil::getOrientationToTarget(int x1, int y1, float dx1, float dy1, int x2, int y2, float dx2, float dy2) {
  float startX = (float) x1 + dx1;
  float endX = (float) x2 + dx2;
  float startY = (float) y1 + dy1;
  float endY = (float) y2 + dy2;
  if(startY == endY) {
    if(startX < endX) {
      return 0.F;
    }
    if(startX > endX) {
      return 180.F;
    }
    if(startX == endX) {
      return 360.F;
    }
  }
  if(startX == endX) {
    if(startY < endY) {
      return 90.F;
    }
    if(startY > endY) {
      return 270.F;
    }
  }
  float angle = std::atan2((endY - startY), (endX - startX));
  if(angle < 0) {
    angle += 2 * 3.141593F;
  }
  return angle * 180.F / 3.141593F;
}

std::vector<MapUtil::Pair> MapUtil::reconstruct_path(std::vector<std::vector<MapUtil::Pair>> cameFrom, MapUtil::Pair start, MapUtil::Pair dest) {
  std::list<MapUtil::Pair> work_list = std::list<MapUtil::Pair>();
  if(start == dest) {
    return std::vector<MapUtil::Pair>();
  }
  for(MapUtil::Pair current = dest; current != start; current = cameFrom[current.y][current.x]) {
    work_list.push_front(current);
  }
  std::vector<MapUtil::Pair> result = std::vector<MapUtil::Pair>(work_list.size());
  std::copy(work_list.begin(), work_list.end(), result.begin());
  return result;
}

float MapUtil::reconstruct_orientation(std::vector<std::vector<MapUtil::Pair>> cameFrom, MapUtil::Pair start, MapUtil::Pair dest) {
  if(start == dest) {
    return 360.F;
  }
  MapUtil::Pair previous = dest;
  for(MapUtil::Pair current = dest; current != start; current = cameFrom[current.y][current.x]) {
    previous = current;
  }
  return getOrientationToTarget(start.x, start.y, 0.F, 0.F, previous.x, previous.y, 0.F, 0.F);
}

std::list<MapUtil::Pair> MapUtil::getNeighboursTraversable(Map * map, int startX, int startY, int destX, int destY) {
  std::list<MapUtil::Pair> result = std::list<MapUtil::Pair>();
  MapUtil::Pair next = MapUtil::Pair();
  if(startY > 0) {
    if(!map->getTile(startY - 1, startX)->untraversable) {
      next.x = startX;
      next.y = startY - 1;
      result.push_back(next);
    }
  }
  if(startY < map->sizeY - 1) {
    if(!map->getTile(startY + 1, startX)->untraversable) {
      next.x = startX;
      next.y = startY + 1;
      result.push_back(next);
    }
  }
  if(startX > 0) {
    if(!map->getTile(startY, startX - 1)->untraversable) {
      next.x = startX - 1;
      next.y = startY;
      result.push_back(next);
    }
  }
  if(startX < map->sizeX - 1) {
    if(!map->getTile(startY, startX + 1)->untraversable) {
      next.x = startX + 1;
      next.y = startY;
      result.push_back(next);
    }
    if(startY > 0) {
      if(!map->getTile(startY - 1, startX + 1)->untraversable) {
        next.x = startX + 1;
        next.y = startY - 1;
        result.push_back(next);
      }
    }
    if(startY < map->sizeY - 1) {
      if(!map->getTile(startY + 1, startX + 1)->untraversable) {
        next.x = startX + 1;
        next.y = startY + 1;
        result.push_back(next);
      }
    }
  }
  if(startX > 0) {
    if(startY > 0) {
      if(!map->getTile(startY - 1, startX - 1)->untraversable) {
        next.x = startX - 1;
        next.y = startY - 1;
        result.push_back(next);
      }
    }
    if(startY < map->sizeY - 1) {
      if(!map->getTile(startY + 1, startX - 1)->untraversable) {
        next.x = startX - 1;
        next.y = startY + 1;
        result.push_back(next);
      }
    }
  }
  return result;
}

std::list<MapUtil::Pair> MapUtil::getNeighboursNonSolid(Map * map, int startX, int startY, int destX, int destY) {
  std::list<MapUtil::Pair> result = std::list<MapUtil::Pair>();
  MapUtil::Pair next = MapUtil::Pair();
  if(startY > 0) {
    if(!map->getTile(startY - 1, startX)->solid) {
      next.x = startX;
      next.y = startY - 1;
      result.push_back(next);
    }
  }
  if(startY < map->sizeY - 1) {
    if(!map->getTile(startY + 1, startX)->solid) {
      next.x = startX;
      next.y = startY + 1;
      result.push_back(next);
    }
  }
  if(startX > 0) {
    if(!map->getTile(startY, startX - 1)->solid) {
      next.x = startX - 1;
      next.y = startY;
      result.push_back(next);
    }
  }
  if(startX < map->sizeX - 1) {
    if(!map->getTile(startY, startX + 1)->solid) {
      next.x = startX + 1;
      next.y = startY;
      result.push_back(next);
    }
    if(startY > 0) {
      if(!map->getTile(startY - 1, startX + 1)->solid) {
        next.x = startX + 1;
        next.y = startY - 1;
        result.push_back(next);
      }
    }
    if(startY < map->sizeY - 1) {
      if(!map->getTile(startY + 1, startX + 1)->solid) {
        next.x = startX + 1;
        next.y = startY + 1;
        result.push_back(next);
      }
    }
  }
  if(startX > 0) {
    if(startY > 0) {
      if(!map->getTile(startY - 1, startX - 1)->solid) {
        next.x = startX - 1;
        next.y = startY - 1;
        result.push_back(next);
      }
    }
    if(startY < map->sizeY - 1) {
      if(!map->getTile(startY + 1, startX - 1)->solid) {
        next.x = startX - 1;
        next.y = startY + 1;
        result.push_back(next);
      }
    }
  }
  return result;
}

std::vector<MapUtil::Pair> MapUtil::getPathToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying) {
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
  gCost[startY][startX] = 0;

  while(!openSet.empty()) {
    MapUtil::Pair current = openSet.front();
    openSet.pop_front();
    if(current.x == destX && current.y == destY) {
      return reconstruct_path(cameFrom, start, dest);
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
  return std::vector<MapUtil::Pair>();
}

float MapUtil::getOrientationToTarget(Map * map, int startX, int startY, int destX, int destY, bool flying) {
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
      return reconstruct_orientation(cameFrom, start, dest);
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

std::list<MapUtil::Pair> MapUtil::getPathFromOrientation(int x, int y, float dx, float dy, float orientation, float range) {
  MapUtil::Pair pair;
  std::list<MapUtil::Pair> result = std::list<MapUtil::Pair>();
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
  pair.dx = x_direction == 1 ? 0.999F : 0.F;
  pair.dy = y_direction == 1 ? 0.999F : 0.F;
  if(orientation == 0.F) {
    result.push_back(pair);
    for(int i = 0; i < std::ceil(range); i++) {
      pair.x++;
      result.push_back(pair);
    }
  }
  else if(orientation == 180.F) {
    result.push_back(pair);
    for(int i = 0; i < std::ceil(range); i++) {
      pair.x--;
      result.push_back(pair);
    }
  }
  else if(orientation == 90.F) {
    result.push_back(pair);
    for(int i = 0; i < std::ceil(range); i++) {
      pair.y++;
      result.push_back(pair);
    }
  }
  else if(orientation == 270.F) {
    result.push_back(pair);
    for(int i = 0; i < std::ceil(range); i++) {
      pair.y--;
      result.push_back(pair);
    }
  }
  else {
    float tan = std::tan(orientation * 3.141593F / 180.F);
    pair.dx = (float) x + dx + (1.F - dy) / tan;
    pair.dx = round(pair.dx - std::floor(pair.dx));
    pair.dy = (float) y + dy + (1.F - dx) * tan;
    pair.dy = round(pair.dy - std::floor(pair.dy));
    float current_range = 0.F;
    float next_x;
    float next_dx;
    float next_y;
    float next_dy;
    while(current_range < range) {
      next_x = pair.x + x_direction;
      next_dx = x_direction == 1 ? 0.F : 0.999F;
      next_dy = (float) pair.y + pair.dy + (1.F - pair.dx) * tan;
      next_y = floor(next_dy);
      next_dy = round(next_dy - (float) next_y);

      if(next_y != pair.y) {
        next_y = pair.y + y_direction;
        next_dy = y_direction == 1 ? 0.F : 0.999F;
        next_dx = (float) pair.x + pair.dx + (1.F - pair.dy) / tan;
        next_x = floor(next_dx);
        next_dx = round(next_dx - (float) next_x);
      }
      current_range += MapUtil::distance(pair.x, pair.y, pair.dx, pair.dy, next_x, next_y, next_dx, next_dy);
      if(pair.x == next_x) {
        pair.dx = next_dx;
      }
      else {
        pair.dx = x_direction == 1 ? 0.999F : 0.F;
      }
      if(pair.y == next_y) {
        pair.dy = next_dy;
      }
      else {
        pair.dy = y_direction == 1 ? 0.999F : 0.F;
      }
      result.push_back(pair);
      pair.dx = next_dx;
      pair.dy = next_dy;
      pair.x = next_x;
      pair.y = next_y;
    }
  }
  return result;
}
