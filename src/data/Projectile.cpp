#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/Projectile.h"

#include "utils/String.h"

int Projectile::getX() { return x; }
int Projectile::getY() { return y; }
int Projectile::getDestX() {
  if(target != nullptr) {
    return target->getX();
  }
  else {
    return dest_x;
  }
}
int Projectile::getDestY() {
  if(target != nullptr) {
    return target->getY();
  }
  else {
    return dest_y;
  }
}

int Projectile::getOrientation() { return orientation; }
int Projectile::getDamageFromType(int damage_type) { return damages[damage_type]; }
int Projectile::getSpeed() { return speed; }
int Projectile::getArea() { return area; }
int Projectile::getFalloffRange() { return falloff_range; }
float Projectile::getWastePerTile() { return waste_per_tile; }
float Projectile::getWastePerTile_area() { return waste_per_tile_area; }
float Projectile::getWastePerHit() { return waste_per_hit; }
Character * Projectile::getTarget() { return target; }
Character * Projectile::getOwner() { return owner; }

void Projectile::move() {
  nextOrientation();
  switch(orientation) {
    case NORTH:
      y++;
      break;
    case NORTH_EAST:
      y++;
      x++;
      break;
    case EAST:
      x++;
      break;
    case SOUTH_EAST:
      y--;
      x++;
      break;
    case SOUTH:
      y--;
      break;
    case SOUTH_WEST:
      y--;
      x--;
      break;
    case WEST:
      x--;
      break;
    case NORTH_WEST:
      y++;
      x--;
      break;
  }
  if(current_travel++ >= falloff_range) {
    for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
      current_damages[damage_type] -= (int) ceil( ((float) damages[damage_type]) * waste_per_tile);
      if(current_damages[damage_type] < 0) {
        current_damages[damage_type] = 0;
      }
    }
  }
}

void Projectile::nextOrientation() {
  if(lost) {
    return;
  }
  float target_x = getDestX() - x;
  float target_y = getDestY() - y;
  int way_to_the_target = orientation; // default if already same tile
  // projectile will simply move to its target
  if(target_y > 0.) {
    if(target_x == 0.) {
        way_to_the_target = NORTH;
    } else {
      way_to_the_target = NORTH;
      float ratio = target_y / target_x;
      if(ratio > -2.) {
          way_to_the_target = NORTH_WEST;
      }
      if(ratio > -0.5) {
          way_to_the_target = WEST;
      }
      if(ratio > 0.) {
          way_to_the_target = EAST;
      }
      if(ratio > 0.5) {
          way_to_the_target = NORTH_EAST;
      }
      if(ratio > 2.) {
          way_to_the_target = NORTH;
      }
    }
  }
  else if(target_y < 0.) {
    way_to_the_target = SOUTH;
    if(target_x == 0.) {
        way_to_the_target = SOUTH;
    } else {
      float ratio = target_y / target_x;
      if(ratio > -2.) {
          way_to_the_target = SOUTH_EAST;
      }
      if(ratio > -0.5) {
          way_to_the_target = EAST;
      }
      if(ratio > 0.) {
          way_to_the_target = WEST;
      }
      if(ratio > 0.5) {
          way_to_the_target = SOUTH_WEST;
      }
      if(ratio > 2.) {
          way_to_the_target = SOUTH;
      }
    }
  }
  // target_y == 0
  else {
    if(target_x > 0.) {
      way_to_the_target = EAST;
    }
    else if(target_x < 0.) {
      way_to_the_target = WEST;
    }
  }
  int diff = way_to_the_target - orientation;
  if(diff > 0 || diff <= -4) {
    orientation = (orientation + 1) % 8;
  } else if(diff < 0 || diff >= 4) {
    orientation = (8 + orientation - 1) % 8;
  }
}

bool Projectile::isAtDest() {
  return x == getDestX() && y == getDestY();
}
float getWastePerTile();
float getWastePerTile_area();
float getWastePerHit();
int Projectile::getRawDamage() {
  int power = 0;
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += current_damages[i];
  }
  return power;
}

bool Projectile::noDamage() { return getRawDamage() <= 0; }

void Projectile::setX(int x) { this->x = x; }
void Projectile::setY(int y) { this->y = y; }
void Projectile::setDestX(int dest_x) { this->dest_x = dest_x; }
void Projectile::setDestY(int dest_y) { this->dest_y = dest_y; }
void Projectile::setSpeed(int speed) { this->speed = speed; }
void Projectile::setArea(int area) { this->area = area; }
void Projectile::setFalloffRange(int falloff_range) { this->falloff_range = falloff_range; }
void Projectile::setWastePerTile(float waste_per_tile) { this->waste_per_tile = waste_per_tile; }
void Projectile::setWastePerTile_area(float waste_per_tile_area) { this->waste_per_tile_area = waste_per_tile_area; }
void Projectile::setWastePerHit(float waste_per_hit) { this->waste_per_hit = waste_per_hit; }
void Projectile::setTarget(Character * target) { this->target = target; }
void Projectile::setOwner(Character * owner) { this->owner = owner; }
void Projectile::setLost(bool state) { lost = state; }

void Projectile::attack_single_target(Character * target, Adventure * adventure) {
  target->receiveAttack(current_damages, orientation);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    current_damages[i] -= (int) ceil( ((float) damages[i]) * waste_per_hit);
  }
  if(skill != nullptr) {
    skill->activate(owner, target, adventure, overcharge_power, overcharge_duration, overcharge_range);
  }
}

void Projectile::attack_multiple_targets(std::list<Character *> characters, Adventure * adventure) {
  for(Character * target : characters) {
    int distance = std::max(abs(x - target->getX()), abs(y - target->getY()));
    if(distance <= area) {
      int reducedDamages[DAMAGE_TYPE_NUMBER];
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        reducedDamages[i] = (int) ceil( ((float) current_damages[i]) * pow(1 - waste_per_tile_area, distance));
      }
      target->receiveAttack(reducedDamages, NO_ORIENTATION);
      if(skill != nullptr) {
        skill->activate(owner, target, adventure, overcharge_power, overcharge_duration, overcharge_range);
      }
    }
  }
}

std::string Projectile::to_string(int offsetY, int offsetX) {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, projectile_type);
  String::insert_int(ss, x - offsetX);
  String::insert_int(ss, y - offsetY);
  String::insert_int(ss, orientation);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_int(ss, current_damages[i]);
  }
  String::insert_int(ss, speed);
  String::insert_int(ss, area);
  String::insert_float(ss, waste_per_tile);
  String::insert_float(ss, waste_per_tile_area);
  String::insert_float(ss, waste_per_hit);
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Projectile::full_to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, projectile_type);
  String::insert_bool(ss, homing);
  String::insert_int(ss, current_map_id);
  String::insert_int(ss, x);
  String::insert_int(ss, y);
  String::insert_int(ss, dest_x);
  String::insert_int(ss, dest_y);
  if(skill != nullptr) {
    String::insert(ss, skill->to_string());
  } else {
    String::insert(ss, "none");
  }
  String::insert_int(ss, orientation);
  String::insert_int(ss, speed);
  String::insert_int(ss, area);
  String::insert_int(ss, overcharge_power);
  String::insert_int(ss, overcharge_duration);
  String::insert_int(ss, overcharge_range);
  String::insert_int(ss, falloff_range);
  String::insert_float(ss, waste_per_tile);
  String::insert_float(ss, waste_per_tile_area);
  String::insert_float(ss, waste_per_hit);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_int(ss, damages[i]);
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Projectile::ammo_to_string(Ammunition * ammo) {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, ammo->projectile->full_to_string());
  String::insert_int(ss, ammo->number);
  String::insert_int(ss, ammo->gold_value);
  String::insert_int(ss, ammo->ammo_type);
  std::string result = ss->str();
  delete ss;
  return result;
}

ProjectileDisplay * Projectile::from_string(std::string to_read) {
  ProjectileDisplay * display = new ProjectileDisplay();
  std::stringstream * ss = new std::stringstream(to_read);
  display->name = String::extract(ss);
  display->id = String::extract_long(ss);
  display->projectile_type = String::extract_int(ss);
  display->x = String::extract_int(ss);
  display->y = String::extract_int(ss);
  display->orientation = String::extract_int(ss);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damages[i] = String::extract_int(ss);
  }
  display->speed = String::extract_int(ss);
  display->area = String::extract_int(ss);
  display->waste_per_tile = String::extract_float(ss);
  display->waste_per_tile_area = String::extract_float(ss);
  display->waste_per_hit = String::extract_float(ss);
  delete ss;
  return display;
}

Projectile * Projectile::full_from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int projectile_type = String::extract_int(ss);
  bool homing = String::extract_bool(ss);
  int current_map_id = String::extract_int(ss);
  int x = String::extract_int(ss);
  int y = String::extract_int(ss);
  int dest_x = String::extract_int(ss);
  int dest_y = String::extract_int(ss);
  std::string skill_str = String::extract(ss);
  Skill * skill = nullptr;
  if(skill_str != "none") {
    skill = Skill::from_string(skill_str);
  }
  Character * target = nullptr;
  Character * owner = nullptr;
  int orientation = String::extract_int(ss);
  int speed = String::extract_int(ss);
  int area = String::extract_int(ss);
  int overcharge_power = String::extract_int(ss);
  int overcharge_duration = String::extract_int(ss);
  int overcharge_range = String::extract_int(ss);
  int falloff_range = String::extract_int(ss);
  float waste_per_tile = String::extract_float(ss);
  float waste_per_tile_area = String::extract_float(ss);
  float waste_per_hit = String::extract_float(ss);
  int damages[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] = String::extract_int(ss);
  }
  delete ss;
  return new Projectile(
    name,
    projectile_type,
    homing,
    current_map_id,
    x,
    y,
    dest_x,
    dest_y,
    skill,
    target,
    owner,
    orientation,
    speed,
    area,
    overcharge_power,
    overcharge_duration,
    overcharge_range,
    falloff_range,
    waste_per_tile,
    waste_per_tile_area,
    waste_per_hit,
    damages
  );
}

Ammunition * Projectile::ammo_from_string(std::string to_read) {
  Ammunition * ammo = new Ammunition();
  std::stringstream * ss = new std::stringstream(to_read);
  ammo->projectile = Projectile::full_from_string(String::extract(ss));
  ammo->number = String::extract_int(ss);
  ammo->gold_value = String::extract_int(ss);
  ammo->ammo_type = String::extract_int(ss);
  delete ss;
  return ammo;
}
