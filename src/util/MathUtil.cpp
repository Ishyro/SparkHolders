#include <algorithm>
#include <list>

#include "data/Adventure.h"
#include "data/Settings.h"

#include "util/String.h"
#include "util/MathUtil.h"

MathUtil::Coords MathUtil::getCoords(MathUtil::Vector3i pos) {
  return getCoords(MathUtil::Vector3(pos.x, pos.y, pos.z));
}

MathUtil::Coords MathUtil::getCoords(MathUtil::Vector3 pos) {
  MathUtil::Coords coords = MathUtil::Coords();
  coords.longitude = MathUtil::Vector3i( (int64_t) 0, (int64_t) 0, (int64_t) ((std::floor(pos.x) - (float) Settings::getLongitudeOrigin()) / Settings::getSecondToMeter()));
  coords.latitude = MathUtil::Vector3i( (int64_t) 0, (int64_t) 0, (int64_t) ((std::floor(pos.y) - (float) Settings::getLatitudeOrigin()) / Settings::getSecondToMeter()));
  
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

int32_t MathUtil::getLight(MathUtil::Coords coords, Time time) {
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
  bool permaDay = false;
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
  // TODO: the shadow is smaller when latitude is higher / lower than 0, because moons are spheres and not cubes
  else {
    Vector3i shadow_angle = Settings::getShadowAngle();
    float shadow_latitude = (float) shadow_angle.x + (float) shadow_angle.y / 60.F + (float) shadow_angle.z / 3600.F;
    if(std::abs(latitude) > std::abs(longitude)) {
      angle = latitude;
    }
    else {
      angle = longitude;
    }
    if(std::abs(latitude) > shadow_latitude) {
      permaDay = true;
    }
  }
  local_hour = (int64_t) (max_hour + hour + std::sin((longitude - sun_angle) * M_PI / 180.F) * zenith) % max_hour;
  int32_t day_indice = time.day - (time.week - 1) * Settings::getWeekDuration();
  // day
  if((local_hour >= startDay && local_hour <= startDusk) || permaDay) {
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
  return (int32_t) std::ceil(std::cos(angle * M_PI / 180.F) * light_power);
}

float MathUtil::round(float var) {
  float value = (int32_t) std::floor(var * 1000.F + .5F);
  value /= 1000.F;
  return value > 0.999F && value < 1.F ? 0.999F : value;
}

MathUtil::Vector3 MathUtil::round(MathUtil::Vector3 var) {
  return MathUtil::Vector3(MathUtil::round(var.x), MathUtil::round(var.y), MathUtil::round(var.z));
}

int64_t MathUtil::mod(int64_t k, int64_t n) {
  int64_t m = k % n;
  return m < 0 ? m + n : m;
}

MathUtil::Vector3i MathUtil::mod(MathUtil::Vector3i var, int64_t n) {
  return MathUtil::Vector3i(MathUtil::mod(var.x, n), MathUtil::mod(var.y, n), MathUtil::mod(var.z, n));
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
    angle += 2 * M_PI;
  }
  return angle * 180.F / M_PI;
}

float MathUtil::getOrientationToTarget(MathUtil::Vector3 coord) {
  if(coord.y == 0.F) {
    if(coord.x > 0.F) {
      return 0.F;
    }
    if(coord.x < 0.F) {
      return 180.F;
    }
    if(coord.x == 0.F) {
      return 360.F;
    }
  }
  if(coord.x == 0.F) {
    if(coord.y > 0) {
      return 90.F;
    }
    if(coord.y < 0) {
      return 270.F;
    }
  }
  float angle = std::atan2(coord.y, coord.x);
  if(angle < 0) {
    angle += 2 * M_PI;
  }
  return angle * 180.F / M_PI;
}

MathUtil::Vector3 MathUtil::getOrientationFromDirection(MathUtil::Vector3 direction) {
  MathUtil::Vector3 angle = MathUtil::Vector3(0.F, 0.F, 0.F);
  if(direction.y == 0.F) {
    if(direction.x > 0.F) {
      angle.z = 0.F;
    }
    if(direction.x < 0.F) {
      angle.z = 180.F;
    }
    if(direction.x == 0.F) {
      angle.z = 360.F;
    }
  }
  else if(direction.x == 0.F) {
    if(direction.y > 0) {
      angle.z = 90.F;
    }
    if(direction.y < 0) {
      angle.z = 270.F;
    }
  }
  else {
    angle.z = std::atan2(direction.y, direction.x);
    if(angle.z < 0) {
      angle.z += 2 * M_PI;
    }
  }

  if(direction.z == 0.F) {
    if(direction.y > 0.F) {
      angle.x = 0.F;
    }
    if(direction.y < 0.F) {
      angle.x = 180.F;
    }
    if(direction.y == 0.F) {
      angle.x = 360.F;
    }
  }
  else if(direction.y == 0.F) {
    if(direction.z > 0) {
      angle.x = 90.F;
    }
    if(direction.z < 0) {
      angle.x = 270.F;
    }
  }
  else {
    angle.x = std::atan2(direction.z, direction.y);
    if(angle.x < 0) {
      angle.x += 2 * M_PI;
    }
  }
  angle = angle * (180.F / M_PI);
  angle.x += 90.F;
  return angle;
}

MathUtil::Vector3 MathUtil::getDirectionFromOrientation(MathUtil::Vector3 orientation) {
  // no orientation.y
  float cos_long = std::cos(orientation.z * M_PI / 180.F);
  float sin_long = std::sin(orientation.z * M_PI / 180.F);
  float cos_colat = std::cos(orientation.x * M_PI / 180.F);
  float sin_colat = std::sin(orientation.x * M_PI / 180.F);
  //
  MathUtil::Vector3 direction = Vector3(sin_colat * cos_long, sin_colat * sin_long, cos_colat);
  direction.normalize();
  return direction;
}


MathUtil::Vector3 MathUtil::selectClosestVector(MathUtil::Vector3 next, MathUtil::Vector3 dest, int32_t x_direction, int32_t y_direction, int32_t z_direction, float factor_x, float factor_y, float factor_z, float & range) {
  float range_x = 0.F;
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
  if(range_x > 1.5F) {
    range_x = 0.F;
  }
  float range_y = 0.F;
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
  if(range_y > 1.5F) {
    range_y = 0.F;
  }
  float range_z = 0.F;
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
  if(range_z > 1.5F) {
    range_z = 0.F;
  }
  MathUtil::Vector3 try_x = MathUtil::Vector3(
    try_x.x = next.x + factor_x * range_x,
    try_x.y = next.y + factor_y * range_x,
    try_x.z = next.z + factor_z * range_x
  );
  MathUtil::Vector3 try_y = MathUtil::Vector3(
    try_y.x = next.x + factor_x * range_y,
    try_y.y = next.y + factor_y * range_y,
    try_y.z = next.z + factor_z * range_y
  );
  MathUtil::Vector3 try_z = MathUtil::Vector3(
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

std::list<MathUtil::Pair> MathUtil::getPathFromOrientation(float x, float y, float orientation_z, float size, float range) {
  MathUtil::Pair pair;
  std::list<MathUtil::Pair> result = std::list<MathUtil::Pair>();
  float high_limit = round(0.999F - size);
  float low_limit = round(size);
  pair.x = x;
  pair.y = y;
  int32_t x_direction = 1;
  int32_t y_direction = 1;
  if(orientation_z > 180.F) {
    y_direction = -1;
  }
  if(orientation_z > 90.F && orientation_z < 270.F) {
    x_direction = -1;
  }
  pair.x = std::floor(x) + (x_direction == 1 ? high_limit : low_limit);
  pair.y = std::floor(y) + (y_direction == 1 ? high_limit : low_limit);
  if(orientation_z == 0.F) {
    pair.y = y;
    result.push_back(pair);
    for(int32_t i = 0; i < std::ceil(range); i++) {
      pair.x = pair.x + 1.F;
      result.push_back(pair);
    }
  }
  else if(orientation_z == 180.F) {
    pair.y = y;
    result.push_back(pair);
    for(int32_t i = 0; i < std::ceil(range); i++) {
      pair.x = pair.x - 1.F;
      result.push_back(pair);
    }
  }
  else if(orientation_z == 90.F) {
    pair.x = x;
    result.push_back(pair);
    for(int32_t i = 0; i < std::ceil(range); i++) {
      pair.y = pair.y + 1.F;
      result.push_back(pair);
    }
  }
  else if(orientation_z == 270.F) {
    pair.x = x;
    result.push_back(pair);
    for(int32_t i = 0; i < std::ceil(range); i++) {
      pair.y = pair.y - 1.F;
      result.push_back(pair);
    }
  }
  else {
    float cos = std::cos(orientation_z * M_PI / 180.F);
    float sin = std::sin(orientation_z * M_PI / 180.F);
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

std::string MathUtil::target_to_string(MathUtil::Target * target) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, target->type);
  if(target->type != TARGET_NONE && target->type != TARGET_SELF) {
    if(target->type == TARGET_CHARACTER || target->type == TARGET_FURNITURE) {
      String::insert_long(ss, target->id);
    }
    else if(target->type == TARGET_COORDINATES || target->type == TARGET_FRONT) {
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

MathUtil::Target * MathUtil::target_from_string(std::string to_read, Adventure * adventure) {
  std::stringstream * ss = new std::stringstream(to_read);
  int32_t type = String::extract_int(ss);
  Character * character = nullptr;
  Furniture * furniture = nullptr;
  MathUtil::Vector3 coord = MathUtil::Vector3(0, 0, 0);
  if(type != TARGET_NONE && type != TARGET_SELF) {
    if(type == TARGET_CHARACTER) {
      character = adventure->getCharacter(String::extract_long(ss));
    }
    else if(type == TARGET_FURNITURE) {
      MathUtil::Vector3i furniture_coord = MathUtil::Vector3i();
      furniture_coord.x = String::extract_long(ss);
      furniture_coord.y = String::extract_long(ss);
      furniture_coord.z = String::extract_long(ss);
      furniture = adventure->getFurniture(furniture_coord);
    }
    else if(type == TARGET_COORDINATES || type == TARGET_FRONT) {
      coord.x = String::extract_float(ss);
      coord.y = String::extract_float(ss);
      coord.z = String::extract_float(ss);
    }
  }
  MathUtil::Target * next = nullptr;
  if(type != TARGET_NONE) {
    std::string next_str = String::extract(ss);
    if(next_str != "END") {
      next = target_from_string(next_str, adventure);
    }
    else {
      next = nullptr;
    }
  }
  delete ss;
  return new MathUtil::Target(
    type,
    character,
    furniture,
    coord,
    next
  );
}

bool MathUtil::collide(MathUtil::Hitbox * box1, MathUtil::Hitbox * box2) {
  if(box1->type == box2->type) {
    if(box1->type == HITBOX_BALL) {
      return MathUtil::collideBalls( (MathUtil::HitboxBall *) box1, (MathUtil::HitboxBall *) box2);
    }
    else if (box1->type == HITBOX_AABB) {
      return MathUtil::collideAABB( (MathUtil::HitboxAABB *) box1, (MathUtil::HitboxAABB *) box2);
    }
    else {
      return MathUtil::collideOBB( (MathUtil::HitboxOBB *) box1, (MathUtil::HitboxOBB *) box2);
    }
  }
  else {
    if(box1->type == HITBOX_BALL) {
      if(box2->type == HITBOX_AABB) {
        return MathUtil::collideBallAABB( (MathUtil::HitboxBall *) box1, (MathUtil::HitboxAABB *) box2);
      }
      else {
        return MathUtil::collideBallOBB( (MathUtil::HitboxBall *) box1, (MathUtil::HitboxOBB *) box2);
      }
    }
    else if(box1->type == HITBOX_AABB) {
      if(box2->type == HITBOX_BALL) {
        return MathUtil::collideBallAABB( (MathUtil::HitboxBall *) box2, (MathUtil::HitboxAABB *) box1);
      }
      else {
        return MathUtil::collideAABBOBB( (MathUtil::HitboxAABB *) box1, (MathUtil::HitboxOBB *) box2);
      }
    }
    else {
      if(box2->type == HITBOX_BALL) {
        return MathUtil::collideBallOBB( (MathUtil::HitboxBall *) box2, (MathUtil::HitboxOBB *) box1);
      }
      else {
        return MathUtil::collideAABBOBB( (MathUtil::HitboxAABB *) box2, (MathUtil::HitboxOBB *) box1);
      }
    }
  }
}

bool MathUtil::collideBalls(MathUtil::HitboxBall * box1, MathUtil::HitboxBall * box2) {
  return MathUtil::distanceSquare(box1->origin, box2->origin) <= (box1->radius + box2->radius) * (box1->radius + box2->radius);
}

bool MathUtil::collideAABB(MathUtil::HitboxAABB * box1, MathUtil::HitboxAABB * box2) {
  if((box1->origin.x + box1->size.x < box2->origin.x || box1->origin.x > + box2->size.x)
    || (box1->origin.y + box1->size.y < box2->origin.y || box1->origin.y > + box2->size.y) 
    || (box1->origin.z + box1->size.z < box2->origin.z || box1->origin.z > + box2->size.z)) {
    return false;
  }
  return true;
}

bool MathUtil::collideOBB(MathUtil::HitboxOBB * box1, MathUtil::HitboxOBB * box2) {
  std::vector<MathUtil::Vector3> corners1 = box1->getCorners();
  std::vector<MathUtil::Vector3> corners2 = box2->getCorners();

  std::vector<MathUtil::Vector3> axes_to_test = {
    box1->x_axis, box1->y_axis, box1->z_axis,
    box2->x_axis, box2->y_axis, box2->z_axis,
    box1->x_axis.cross(box2->x_axis),
    box1->x_axis.cross(box2->y_axis),
    box1->x_axis.cross(box2->z_axis),
    box1->y_axis.cross(box2->x_axis),
    box1->y_axis.cross(box2->y_axis),
    box1->y_axis.cross(box2->z_axis),
    box1->z_axis.cross(box2->x_axis),
    box1->z_axis.cross(box2->y_axis),
    box1->z_axis.cross(box2->z_axis)
  };

  // test projections
  for(const auto& axis : axes_to_test) {
    float min1 = std::numeric_limits<float>::max(), max1 = -std::numeric_limits<float>::max();
    float min2 = std::numeric_limits<float>::max(), max2 = -std::numeric_limits<float>::max();

    for (const auto& corner : corners1) {
      float projection = corner.dot(axis);
      min1 = std::min(min1, projection);
      max1 = std::max(max1, projection);
    }

    for (const auto& corner : corners2) {
      float projection = corner.dot(axis);
      min2 = std::min(min2, projection);
      max2 = std::max(max2, projection);
    }

    if (max1 < min2 || max2 < min1) {
        return false;  // separation
    }
  }
  // no separation
  return true;
}

bool MathUtil::collideBallAABB(MathUtil::HitboxBall * box1, MathUtil::HitboxAABB * box2) {
  float px = std::fmax(box2->origin.x, std::fmin(box1->origin.x, box2->origin.x + box2->size.x));
  float py = std::fmax(box2->origin.y, std::fmin(box1->origin.y, box2->origin.y + box2->size.y));
  float pz = std::fmax(box2->origin.z, std::fmin(box1->origin.z, box2->origin.z + box2->size.z));
  float distance = (px - box1->origin.x) * (px - box1->origin.x) +
                    (py - box1->origin.y) * (py - box1->origin.y) +
                    (pz - box1->origin.z) * (pz - box1->origin.z);
  return distance <= box1->radius * box1->radius;
}

bool MathUtil::collideBallOBB(MathUtil::HitboxBall * box1, MathUtil::HitboxOBB * box2) {
  MathUtil::Vector3 diff = box1->origin - box2->origin;
  float distance;

  distance = std::abs(diff.dot(box2->x_axis));
  if (distance > (box2->size.x / 2) + box1->radius) return false;

  distance = std::abs(diff.dot(box2->y_axis));
  if (distance > (box2->size.y / 2) + box1->radius) return false;

  distance = std::abs(diff.dot(box2->z_axis));
  if (distance > (box2->size.z / 2) + box1->radius) return false;

  // no separation
  return true;
}

std::vector<MathUtil::Vector3> transformAABBToOBBSystem(MathUtil::HitboxAABB * box1, MathUtil::HitboxOBB * box2) {
  MathUtil::Vector3 halfExtents = MathUtil::Vector3(box1->size.x, box1->size.y, box1->size.z) * 0.5;
  MathUtil::Vector3 centerOffset = box2->origin - (box1->origin + halfExtents);

  std::vector<MathUtil::Vector3> aabbCorners = {
    MathUtil::Vector3(centerOffset.x, centerOffset.y, centerOffset.z),
    MathUtil::Vector3(centerOffset.x, centerOffset.y, centerOffset.z + box1->size.z),
    MathUtil::Vector3(centerOffset.x, centerOffset.y + box1->size.y, centerOffset.z),
    MathUtil::Vector3(centerOffset.x, centerOffset.y + box1->size.y, centerOffset.z + box1->size.z),
    MathUtil::Vector3(centerOffset.x + box1->size.x, centerOffset.y, centerOffset.z),
    MathUtil::Vector3(centerOffset.x + box1->size.x, centerOffset.y, centerOffset.z + box1->size.z),
    MathUtil::Vector3(centerOffset.x + box1->size.x, centerOffset.y + box1->size.y, centerOffset.z),
    MathUtil::Vector3(centerOffset.x + box1->size.x, centerOffset.y + box1->size.y, centerOffset.z + box1->size.z)
  };

  std::vector<MathUtil::Vector3> transformedCorners;
  for (const auto& corner : aabbCorners) {
    MathUtil::Vector3 cornerRelative = corner - box2->origin;  // move AABB to OOB center
      transformedCorners.push_back(MathUtil::Vector3(
          cornerRelative.dot(box2->x_axis), 
          cornerRelative.dot(box2->y_axis), 
          cornerRelative.dot(box2->z_axis)
      ));
  }
  return transformedCorners;
}

// TODO
// NOT WORKING
bool MathUtil::collideAABBOBB(MathUtil::HitboxAABB * box1, MathUtil::HitboxOBB * box2) {
  std::vector<MathUtil::Vector3> transformedAABBCorners = transformAABBToOBBSystem(box1, box2);
  std::vector<MathUtil::Vector3> obbCorners = box2->getCorners();

  std::vector<MathUtil::Vector3> axes_to_test = {
    box2->x_axis, box2->y_axis, box2->z_axis
  };

  // test projections
  for(const auto& axis : axes_to_test) {
    float min1 = std::numeric_limits<float>::max(), max1 = -std::numeric_limits<float>::max();
    float min2 = std::numeric_limits<float>::max(), max2 = -std::numeric_limits<float>::max();

    for (const auto& corner : transformedAABBCorners) {
      float projection = corner.dot(axis);
      min1 = std::min(min1, projection);
      max1 = std::max(max1, projection);
    }
    for (const auto& corner : obbCorners) {
      float projection = corner.dot(axis);
      min2 = std::min(min2, projection);
      max2 = std::max(max2, projection);
    }
    if (max1 < min2 || max2 < min1) {
      return false; // separation
    }
  }

  // no separation
  return true;
}
