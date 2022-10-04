#include <algorithm>
#include <list>

#include "data/Map.h"

#include "utils/MapUtil.h"

int MapUtil::distance(int x1, int y1, int x2, int y2) {
  return std::max(abs(x1 - x2), abs(y1 - y2));
}

// no sqrt because it will be used only as comparaison
int MapUtil::distanceSqrt(int x1, int y1, int x2, int y2) {
  return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

int MapUtil::orientationToTarget(int x1, int y1, int x2, int y2) {
  if(x1 == x2 && y1 < y2)
    return NORTH;
  if(x1 < x2 && y1 < y2)
    return NORTH_EAST;
  if(x1 < x2 && y1 == y2)
    return EAST;
  if(x1 < x2 && y1 > y2)
    return SOUTH_EAST;
  if(x1 == x2 && y1 > y2)
    return SOUTH;
  if(x1 > x2 && y1 > y2)
    return SOUTH_WEST;
  if(x1 > x2 && y1 == y2)
    return WEST;
  if(x1 > x2 && y1 < y2)
    return NORTH_WEST;
  return NO_ORIENTATION;
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

int MapUtil::reconstruct_orientation(std::vector<std::vector<MapUtil::Pair>> cameFrom, MapUtil::Pair start, MapUtil::Pair dest) {
  if(start == dest) {
    return NO_ORIENTATION;
  }
  int result = 0;
  MapUtil::Pair previous = dest;
  for(MapUtil::Pair current = dest; current != start; current = cameFrom[current.y][current.x]) {
    previous = current;
  }
  return orientationToTarget(start.x, start.y, previous.x, previous.y);
}

std::list<MapUtil::Pair> MapUtil::getNeighbours(Map * map, int startX, int startY, int destX, int destY) {
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

std::list<MapUtil::Pair> MapUtil::getNeighboursAllTiles(Map * map, int startX, int startY, int destX, int destY) {
  std::list<MapUtil::Pair> result = std::list<MapUtil::Pair>();
  MapUtil::Pair next = MapUtil::Pair();
  if(startY > 0) {
    next.x = startX;
    next.y = startY - 1;
    result.push_back(next);
  }
  if(startY < map->sizeY - 1) {
    next.x = startX;
    next.y = startY + 1;
    result.push_back(next);
  }
  if(startX > 0) {
    next.x = startX - 1;
    next.y = startY;
    result.push_back(next);
  }
  if(startX < map->sizeX - 1) {
    next.x = startX + 1;
    next.y = startY;
    result.push_back(next);
    if(startY > 0) {
      next.x = startX + 1;
      next.y = startY - 1;
      result.push_back(next);
    }
    if(startY < map->sizeY - 1) {
      next.x = startX + 1;
      next.y = startY + 1;
      result.push_back(next);
    }
  }
  if(startX > 0) {
    if(startY > 0) {
      next.x = startX - 1;
      next.y = startY - 1;
      result.push_back(next);
    }
    if(startY < map->sizeY - 1) {
      next.x = startX - 1;
      next.y = startY + 1;
      result.push_back(next);
    }
  }
  return result;
}

std::vector<MapUtil::Pair> MapUtil::getPathToTarget(Map * map, int startX, int startY, int destX, int destY) {
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

int MapUtil::getOrientationToTarget(Map * map, int startX, int startY, int destX, int destY) {
  MapUtil::Pair start = MapUtil::Pair();
  start.x = startX;
  start.y = startY;
  start.score = distance(startX, startY, destX, destY);
  MapUtil::Pair dest = MapUtil::Pair();
  dest.x = destX;
  dest.y = destY;
  dest.score = 0;
  if(start == dest) {
    return NO_ORIENTATION;
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
  return NO_ORIENTATION;
}

std::vector<MapUtil::Pair> MapUtil::getStraightPathToTarget(Map * map, int startX, int startY, int destX, int destY) {
  MapUtil::Pair start = MapUtil::Pair();
  start.x = startX;
  start.y = startY;
  start.score = distanceSqrt(startX, startY, destX, destY);
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
    for(MapUtil::Pair pair : getNeighboursAllTiles(map, current.x, current.y, destX, destY)) {
      int tentative_gScore = gCost[current.y][current.x] + 1;
      if(gCost[pair.y][pair.x] > tentative_gScore) {
        gCost[pair.y][pair.x] = tentative_gScore;
        cameFrom[pair.y][pair.x] = current;
        pair.score = tentative_gScore + distanceSqrt(pair.x, pair.y, destX, destY);
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
