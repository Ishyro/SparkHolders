#include <algorithm>
#include <list>

#include "data/Adventure.h"
#include "data/Map.h"
#include "data/Settings.h"

#include "util/String.h"
#include "util/MathUtil.h"

MathUtil::Vector3 MathUtil::makeVector3(float x, float y, float z) {
  MathUtil::Vector3 coord = MathUtil::Vector3();
  coord.x = x;
  coord.y = y;
  coord.z = z;
  return coord;
}

MathUtil::Vector3 MathUtil::makeVector3(MathUtil::Vector3i coord_int) {
  MathUtil::Vector3 coord = MathUtil::Vector3();
  coord.x = coord_int.x;
  coord.y = coord_int.y;
  coord.z = coord_int.z;
  return coord;
}

MathUtil::Vector3i MathUtil::makeVector3i(float x, float y, float z) {
  MathUtil::Vector3i coord = MathUtil::Vector3i();
  coord.x = (int64_t) std::floor(x);
  coord.y = (int64_t) std::floor(y);
  coord.z = (int64_t) std::floor(z);
  return coord;
}

MathUtil::Vector3i MathUtil::makeVector3i(MathUtil::Vector3 coord_float) {
  MathUtil::Vector3i coord = MathUtil::Vector3i();
  coord.x = (int64_t) std::floor(coord_float.x);
  coord.y = (int64_t) std::floor(coord_float.y);
  coord.z = (int64_t) std::floor(coord_float.z);
  return coord;
}

MathUtil::Coords MathUtil::getCoords(Vector3 pos) {
  MathUtil::Coords coords = MathUtil::Coords();
  coords.longitude = makeVector3i(0, 0, (int64_t) ((std::floor(pos.x) - (float) Settings::getLongitudeOrigin()) / Settings::getSecondToMeter()));
  coords.latitude = makeVector3i(0, 0, (int64_t) ((std::floor(pos.y) - (float) Settings::getLatitudeOrigin()) / Settings::getSecondToMeter()));
  
  coords.longitude.y = coords.longitude.z / 60;
  coords.longitude.z -= coords.longitude.y * 60;
  coords.longitude.x = coords.longitude.y / 60;
  coords.longitude.y -= coords.longitude.x * 60;

  coords.latitude.y = coords.latitude.z / 60;
  coords.latitude.z -= coords.latitude.y * 60;
  coords.latitude.x = coords.latitude.y / 60;
  coords.latitude.y -= coords.latitude.x * 60;

  return coords;
}

float MathUtil::getLight(MathUtil::Coords coords, Time time) {
  float angle;
  float sun_angle = 0.F;
  int64_t light_power;
  float latitude = (float) coords.latitude.x + (float) coords.latitude.y / 60.F + (float) coords.latitude.z / 3600.F;
  float longitude = (float) coords.longitude.x + (float) coords.longitude.y / 60.F + (float) coords.longitude.z / 3600.F;
  int64_t hour = time.hour * Settings::getHourDuration() * Settings::getMinuteDuration() + time.minutes * Settings::getMinuteDuration() + time.seconds;
  int64_t max_hour = Settings::getDayDurationInRound() * Settings::getMinuteDuration();
  int64_t zenith = max_hour / 2;
  int64_t startNight = max_hour - Settings::getNightDuration() * Settings::getHourDuration() * Settings::getMinuteDuration() / 2.F;
  int64_t startDusk = startNight - Settings::getDuskDuration() * Settings::getHourDuration() * Settings::getMinuteDuration();
  int64_t startDawn = Settings::getNightDuration() * Settings::getHourDuration() * Settings::getMinuteDuration() / 2.F;
  int64_t startDay = startDawn + Settings::getDawnDuration() * Settings::getHourDuration() * Settings::getMinuteDuration();
  int64_t local_hour = hour;
  if(!Settings::getTidalLocked()) {
    // negative angle when the sun is rising, ie at east from ORIGIN
    float sun_factor = (float) hour / (float) max_hour - (float) zenith;
    sun_angle = 360.F * sun_factor;
    if(std::abs(latitude) > std::abs(longitude - sun_angle)) {
      angle = latitude;
    }
    else {
      angle = longitude - sun_angle;
    }
  }
  else {
    if(std::abs(latitude) > std::abs(longitude)) {
      angle = latitude;
    }
    else {
      angle = longitude;
    }
  }
  local_hour = (int64_t) (max_hour + hour + std::sin((longitude - sun_angle) * 3.141593F / 180.F) * zenith) % max_hour;
  int32_t day_indice = time.day - (time.week - 1) * Settings::getWeekDuration();
  // day
  if(local_hour >= startDay && local_hour <= startDusk) {
    light_power = Settings::getZenithLightPower(day_indice);
  }
  // night
  else if(local_hour >= startNight || local_hour <= startDawn) {
    light_power = Settings::getNightLightPower(day_indice);
  }
  // dusk
  else if(local_hour >= startDusk) {
    light_power = Settings::getNightLightPower(day_indice) + (Settings::getZenithLightPower(day_indice) - Settings::getNightLightPower(day_indice)) * (1.F - ((float) local_hour - (float) startDusk) / (float) ((Settings::getDuskDuration() * Settings::getHourDuration() * Settings::getMinuteDuration())));
  }
  // dawn
  else {
    light_power = Settings::getNightLightPower(day_indice) + (Settings::getZenithLightPower(day_indice) - Settings::getNightLightPower(day_indice)) * ((float) local_hour - (float)  startDawn) / (float) ( (Settings::getDawnDuration() * Settings::getHourDuration() * Settings::getMinuteDuration()));
  }
  return std::cos(angle * 3.141593F / 180.F) * light_power;
}

float MathUtil::round(float var) {
  float value = (int32_t) std::floor(var * 1000.F + .5F);
  value /= 1000.F;
  return value > 0.999F && value < 1.F ? 0.999F : value;
}

MathUtil::Vector3 MathUtil::round(MathUtil::Vector3 var) {
  return MathUtil::makeVector3(MathUtil::round(var.x), MathUtil::round(var.y), MathUtil::round(var.z));
}

float MathUtil::distanceSquare(float x1, float y1, float x2, float y2) {
  return std::max(std::abs(x2 - x1), std::abs(y2 - y1));
}

float MathUtil::distance(float x1, float y1, float x2, float y2) {
  return round(std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

float MathUtil::distance(MathUtil::Vector3 coord1, MathUtil::Vector3 coord2) {
  return round(std::sqrt((coord1.x - coord2.x) * (coord1.x - coord2.x) + (coord1.y - coord2.y) * (coord1.y - coord2.y) + (coord1.z - coord2.z) * (coord1.z - coord2.z)));
}

double MathUtil::distanceSquare(MathUtil::Vector3 coord1, MathUtil::Vector3 coord2) {
  return ( (double) coord1.x - (double) coord2.x) * ( (double)coord1.x - (double) coord2.x) + ( (double) coord1.y - (double) coord2.y) * ( (double) coord1.y - (double) coord2.y) + ( (double) coord1.z - (double) coord2.z) * ( (double) coord1.z - (double) coord2.z);
}

float MathUtil::distance2(MathUtil::Vector3 coord1, MathUtil::Vector3 coord2) {
  return round(std::sqrt((coord1.x - coord2.x) * (coord1.x - coord2.x) + (coord1.y - coord2.y) * (coord1.y - coord2.y)));
}

float MathUtil::getOrientationToTarget(float x1, float y1, float x2, float y2) {
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

MathUtil::Vector3 MathUtil::selectClosestVector(MathUtil::Vector3 next, MathUtil::Vector3 dest, int32_t x_direction, int32_t y_direction, int32_t z_direction, float factor_x, float factor_y, float factor_z, float & range) {
  float range_x = INFINITY;
  if(factor_x != 0.F) {
    if(x_direction == 1) {
      range_x = std::abs((1.F - (next.x - std::floor(next.x))) / factor_x);
    }
    else {
      range_x = std::abs((next.x - std::floor(next.x)) / factor_x);
      if(range_x == 0.F) {
        range_x = std::abs(1.F / factor_x);
      }
    }
  }
  float range_y = INFINITY;
  if(factor_y != 0.F) {
    if(y_direction == 1) {
      range_y = std::abs((1.F - (next.y - std::floor(next.y))) / factor_y);
    }
    else {
      range_y = std::abs((next.y - std::floor(next.y)) / factor_y);
      if(range_y == 0.F) {
        range_y = std::abs(1.F / factor_y);
      }
    }
  }
  float range_z = INFINITY;
  if(factor_z != 0.F) {
    if(z_direction == 1) {
      range_z = std::abs((1.F - (next.z - std::floor(next.z))) / factor_z);
    }
    else {
      range_z = std::abs((next.z - std::floor(next.z)) / factor_z);
      if(range_z == 0.F) {
        range_z = std::abs(1.F / factor_z);
      }
    }
  }
  MathUtil::Vector3 try_x = MathUtil::makeVector3(
    try_x.x = next.x + factor_x * range_x,
    try_x.y = next.y + factor_y * range_x,
    try_x.z = next.z + factor_z * range_x
  );
  MathUtil::Vector3 try_y = MathUtil::makeVector3(
    try_y.x = next.x + factor_x * range_y,
    try_y.y = next.y + factor_y * range_y,
    try_y.z = next.z + factor_z * range_y
  );
  MathUtil::Vector3 try_z = MathUtil::makeVector3(
    try_z.x = next.x + factor_x * range_z,
    try_z.y = next.y + factor_y * range_z,
    try_z.z = next.z + factor_z * range_z
  );
  double dist_x = MathUtil::distanceSquare(try_x, dest);
  double dist_y = MathUtil::distanceSquare(try_y, dest);
  double dist_z = MathUtil::distanceSquare(try_z, dest);
  if(dist_x <= dist_y && dist_x <= dist_z) {
    range = range_x;
    return MathUtil::round(try_x);
  }
  else if(dist_y <= dist_x && dist_y <= dist_z) {
    range = range_y;
    return MathUtil::round(try_y);
  }
  else {
    range = range_z;
    return MathUtil::round(try_z);
  }
}

std::vector<MathUtil::Pair> MathUtil::reconstruct_path(std::vector<std::vector<MathUtil::Pair>> cameFrom, MathUtil::Pair start, MathUtil::Pair dest, int32_t offsetX, int32_t offsetY) {
  std::list<MathUtil::Pair> work_list = std::list<MathUtil::Pair>();
  if(start == dest) {
    return std::vector<MathUtil::Pair>();
  }
  for(MathUtil::Pair current = dest; current != start; current = cameFrom[current.y - offsetY][current.x - offsetX]) {
    work_list.push_front(current);
  }
  std::vector<MathUtil::Pair> result = std::vector<MathUtil::Pair>(work_list.size());
  std::copy(work_list.begin(), work_list.end(), result.begin());
  return result;
}

float MathUtil::reconstruct_orientation(std::vector<std::vector<MathUtil::Pair>> cameFrom, MathUtil::Pair start, MathUtil::Pair dest, int32_t offsetX, int32_t offsetY) {
  if(start == dest) {
    return 360.F;
  }
  MathUtil::Pair previous = dest;
  for(MathUtil::Pair current = dest; current != start; current = cameFrom[current.y - offsetY][current.x - offsetX]) {
    previous = current;
  }
  return getOrientationToTarget(start.x, start.y, previous.x, previous.y);
}

/*
std::list<MathUtil::Pair> MathUtil::getNeighboursTraversable(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY) {
  std::list<MathUtil::Pair> result = std::list<MathUtil::Pair>();
  MathUtil::Pair next = MathUtil::Pair();
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

std::list<MathUtil::Pair> MathUtil::getNeighboursNonSolid(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY) {
  std::list<MathUtil::Pair> result = std::list<MathUtil::Pair>();
  MathUtil::Pair next = MathUtil::Pair();
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

std::vector<MathUtil::Pair> MathUtil::getPathToTarget(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY, bool flying) {
  return std::vector<MathUtil::Pair>();
  /*
  int32_t start_x = startX - map->offsetX;
  int32_t start_y = startY - map->offsetY;
  std::list<MathUtil::Pair> (*getNeighbours)(Map *, int32_t, int32_t, int32_t, int32_t){ &getNeighboursTraversable };
  if(flying) {
    getNeighbours = &getNeighboursNonSolid;
  }
  MathUtil::Pair start = MathUtil::Pair();
  start.x = startX;
  start.y = startY;
  start.score = distance(startX, startY, destX, destY);
  MathUtil::Pair dest = MathUtil::Pair();
  dest.x = destX;
  dest.y = destY;
  dest.score = 0;
  if(start == dest) {
    return std::vector<MathUtil::Pair>();
  }
  std::list<MathUtil::Pair> openSet = std::list<MathUtil::Pair>();
  std::vector<std::vector<MathUtil::Pair>> cameFrom = std::vector<std::vector<MathUtil::Pair>>(map->sizeY);
  std::vector<std::vector<int32_t>> gCost = std::vector<std::vector<int32_t>>(map->sizeY);
  for(int32_t i = 0; i < map->sizeY; i++) {
    cameFrom[i] = std::vector<MathUtil::Pair>(map->sizeX);
    gCost[i] = std::vector<int32_t>(map->sizeX);
  }
  int32_t max = map->sizeX * map->sizeY;
  for(int32_t i = 0; i < map->sizeY; i++) {
    for(int32_t j = 0; j < map->sizeX; j++) {
      gCost[i][j] = max;
    }
  }
  openSet.push_front(start);
  gCost[start_y][start_x] = 0;

  while(!openSet.empty()) {
    MathUtil::Pair current = openSet.front();
    openSet.pop_front();
    if(current.x == destX && current.y == destY) {
      return reconstruct_path(cameFrom, start, dest, map->offsetX, map->offsetY);
    }
    for(MathUtil::Pair pair : getNeighbours(map, current.x, current.y, destX, destY)) {
      int32_t tentative_gScore = gCost[current.y - map->offsetY][current.x - map->offsetX] + 1;
      if(gCost[pair.y - map->offsetY][pair.x - map->offsetX] > tentative_gScore) {
        gCost[pair.y - map->offsetY][pair.x - map->offsetX] = tentative_gScore;
        cameFrom[pair.y - map->offsetY][pair.x - map->offsetX] = current;
        pair.score = tentative_gScore + distance(pair.x, pair.y, destX, destY);
        bool found = false;
        for(MathUtil::Pair pair2 : openSet) {
          if(pair == pair2) {
            found = true;
            break;
          }
        }
        if(!found) {
          std::list<MathUtil::Pair> sorted_element = std::list<MathUtil::Pair>(1);
          sorted_element.push_front(pair);
          openSet.merge(sorted_element);
        }
      }
    }
  }
  return std::vector<MathUtil::Pair>();
  */
}

float MathUtil::getOrientationToTarget(Map * map, int32_t startX, int32_t startY, int32_t destX, int32_t destY, bool flying) {
  return 0.F;
  /*
  std::list<MathUtil::Pair> (*getNeighbours)(Map *, int32_t, int32_t, int32_t, int32_t){ &getNeighboursTraversable };
  if(flying) {
    getNeighbours = &getNeighboursNonSolid;
  }
  MathUtil::Pair start = MathUtil::Pair();
  start.x = startX;
  start.y = startY;
  start.score = distance(startX, startY, destX, destY);
  MathUtil::Pair dest = MathUtil::Pair();
  dest.x = destX;
  dest.y = destY;
  dest.score = 0;
  if(start == dest) {
    return 360.F;
  }
  std::list<MathUtil::Pair> openSet = std::list<MathUtil::Pair>();
  std::vector<std::vector<MathUtil::Pair>> cameFrom = std::vector<std::vector<MathUtil::Pair>>(map->sizeY);
  std::vector<std::vector<int32_t>> gCost = std::vector<std::vector<int32_t>>(map->sizeY);
  for(int32_t i = 0; i < map->sizeY; i++) {
    cameFrom[i] = std::vector<MathUtil::Pair>(map->sizeX);
    gCost[i] = std::vector<int32_t>(map->sizeX);
  }
  int32_t max = map->sizeX * map->sizeY;
  for(int32_t i = 0; i < map->sizeY; i++) {
    for(int32_t j = 0; j < map->sizeX; j++) {
      gCost[i][j] = max;
    }
  }
  openSet.push_front(start);
  gCost[startY][startX] = 0;

  while(!openSet.empty()) {
    MathUtil::Pair current = openSet.front();
    openSet.pop_front();
    if(current.x == destX && current.y == destY) {
      return reconstruct_orientation(cameFrom, start, dest, map->offsetX, map->offsetY);
    }
    for(MathUtil::Pair pair : getNeighbours(map, current.x, current.y, destX, destY)) {
      int32_t tentative_gScore = gCost[current.y][current.x] + 1;
      if(gCost[pair.y][pair.x] > tentative_gScore) {
        gCost[pair.y][pair.x] = tentative_gScore;
        cameFrom[pair.y][pair.x] = current;
        pair.score = tentative_gScore + distance(pair.x, pair.y, destX, destY);
        bool found = false;
        for(MathUtil::Pair pair2 : openSet) {
          if(pair == pair2) {
            found = true;
            break;
          }
        }
        if(!found) {
          std::list<MathUtil::Pair> sorted_element = std::list<MathUtil::Pair>(1);
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
std::list<MathUtil::Pair> MathUtil::getPathFromCartesianEquation(float a, int32_t range) {
  MathUtil::Pair pair;
  std::list<MathUtil::Pair> result = std::list<MathUtil::Pair>();
  for(pair.x = 0; pair.x <= range; pair.x++) {
    pair.y = std::floor(a * (float) pair.x);
    result.push_back(pair);
  }
  return result;
}

std::list<MathUtil::Pair> MathUtil::getPathFromOrientation(float x, float y, float orientation, float size, float range) {
  MathUtil::Pair pair;
  std::list<MathUtil::Pair> result = std::list<MathUtil::Pair>();
  float high_limit = round(0.999F - size);
  float low_limit = round(size);
  pair.x = x;
  pair.y = y;
  int32_t x_direction = 1;
  int32_t y_direction = 1;
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
    for(int32_t i = 0; i < std::ceil(range); i++) {
      pair.x = pair.x + 1.F;
      result.push_back(pair);
    }
  }
  else if(orientation == 180.F) {
    pair.y = y;
    result.push_back(pair);
    for(int32_t i = 0; i < std::ceil(range); i++) {
      pair.x = pair.x - 1.F;
      result.push_back(pair);
    }
  }
  else if(orientation == 90.F) {
    pair.x = x;
    result.push_back(pair);
    for(int32_t i = 0; i < std::ceil(range); i++) {
      pair.y = pair.y + 1.F;
      result.push_back(pair);
    }
  }
  else if(orientation == 270.F) {
    pair.x = x;
    result.push_back(pair);
    for(int32_t i = 0; i < std::ceil(range); i++) {
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

std::string MathUtil::target_to_string(Target * target) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, target->type);
  if(target->type != TARGET_NONE && target->type != TARGET_SELF) {
    if(target->type == TARGET_CHARACTER) {
      String::insert_long(ss, target->id);
    }
    else {
      String::insert_float(ss, target->coord.x);
      String::insert_float(ss, target->coord.y);
      String::insert_float(ss, target->coord.z);
    }
  }
  if(target->next != nullptr) {
    String::insert(ss, target_to_string(target->next));
  }
  else if(target->type != TARGET_NONE) {
    String::insert(ss, "END");
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

Target * MathUtil::target_from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  Target * target = new Target();
  target->type = String::extract_int(ss);
  if(target->type != TARGET_NONE && target->type != TARGET_SELF) {
    if(target->type == TARGET_CHARACTER) {
      target->id = String::extract_long(ss);
    }
    else {
      float x = String::extract_float(ss);
      float y = String::extract_float(ss);
      float z = String::extract_float(ss);
      target->coord = MathUtil::makeVector3(x, y, z);
    }
  }
  if(target->type != TARGET_NONE) {
    std::string next = String::extract(ss);
    if(next != "END") {
      target->next = target_from_string(next);
    }
    else {
      target->next = nullptr;
    }
  }
  delete ss;
  return target;
}
