#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/Projectile.h"

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
Character * Projectile::getTarget() { return target; }
Character * Projectile::getOwner() { return owner; }

void Projectile::move() {
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
  nextOrientation();
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    current_damages[damage_type] -= (int) ceil( ((float) damages[damage_type]) * waste_per_tile);
    if(current_damages[damage_type] < 0) {
      current_damages[damage_type] = 0;
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
      float ratio = target_y / target_x;
      if(ratio > -2.) {
          way_to_the_target = NORTH_WEST;
      }
      else if(ratio > -0.5) {
          way_to_the_target = WEST;
      }
      else if(ratio > 0.) {
          way_to_the_target = EAST;
      }
      else if(ratio > 0.5) {
          way_to_the_target = NORTH_EAST;
      }
      else if(ratio > 2.) {
          way_to_the_target = NORTH;
      }
      else {
        way_to_the_target = NORTH;
      }
    }
  }
  else if(target_y < 0.) {
    if(target_x == 0.) {
        way_to_the_target = SOUTH;
    } else {
      float ratio = target_y / target_x;
      if(ratio > -2.) {
          way_to_the_target = SOUTH_EAST;
      }
      else if(ratio > -0.5) {
          way_to_the_target = EAST;
      }
      else if(ratio > 0.) {
          way_to_the_target = WEST;
      }
      else if(ratio > 0.5) {
          way_to_the_target = SOUTH_WEST;
      }
      else if(ratio > 2.) {
          way_to_the_target = SOUTH;
      }
      else {
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
  int diff = orientation - way_to_the_target;
  if(diff > 0) {
    orientation = (orientation + 1) % 8;
  }
  if(diff < 0) {
    orientation = (orientation - 1) % 8;
  }
}

bool Projectile::isAtDest() {
  return x == getDestX() && y == getDestY();
}

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
void Projectile::setTarget(Character * target) { this->target = target; }
void Projectile::setOwner(Character * owner) { this->owner = owner; }
void Projectile::setLost(bool state) { lost = state; }

void Projectile::attack_single_target(Character * target, Adventure * adventure) {
  target->receiveAttack(current_damages, orientation);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    current_damages[i] -= (int) ceil( ((float) damages[i]) * waste_per_hit);
  }
  if(skill != nullptr) {
    skill->activate(owner, target, adventure, overcharge);
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
        skill->activate(owner, target, adventure, overcharge);
      }
    }
  }
}

std::string Projectile::to_string(long offsetY, long offsetX) {
  std::string msg = name + ";";
  msg += std::to_string(id) + ";";
  msg += std::to_string(projectile_type) + ";";
  msg += std::to_string(x - offsetX) + ";";
  msg += std::to_string(y - offsetY) + ";";
  msg += std::to_string(orientation) + ";";
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    msg += std::to_string(current_damages[i]) + ";";
  }
  msg += std::to_string(speed) + ";";
  msg += std::to_string(area) + ";";
  msg += std::to_string(waste_per_tile) + ";";
  msg += std::to_string(waste_per_tile_area) + ";";
  msg += std::to_string(waste_per_hit) + ";";
  return msg;
}

std::string Projectile::full_to_string() {
  std::string msg = name + "/";
  msg += std::to_string(projectile_type) + "/";
  msg += std::to_string(homing) + "/";
  msg += std::to_string(current_map_id) + "/";
  msg += std::to_string(x) + "/";
  msg += std::to_string(y) + "/";
  msg += std::to_string(dest_x) + "/";
  msg += std::to_string(dest_y) + "/";
  if(skill != nullptr) {
    //msg += std::to_string(skill) + "/";
  } else {
    msg += "none/";
  }
  if(target != nullptr) {
    msg += std::to_string(target->id) + "/";
  } else {
    msg += "none/";
  }
  if(owner != nullptr) {
    msg += std::to_string(owner->id) + "/";
  } else {
    msg += "none/";
  }
  msg += std::to_string(orientation) + "/";
  msg += std::to_string(speed) + "/";
  msg += std::to_string(area) + "/";
  msg += std::to_string(overcharge) + "/";
  msg += std::to_string(waste_per_tile) + "/";
  msg += std::to_string(waste_per_tile_area) + "/";
  msg += std::to_string(waste_per_hit) + "/";
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    msg += std::to_string(damages[i]) + "/";
  }
  return msg;
}

std::string Projectile::ammo_to_string(Ammunition * ammo) {
  std::string msg = ammo->projectile->full_to_string() + ";";
  msg += std::to_string(ammo->number) + ";";
  msg += std::to_string(ammo->gold_value) + ";";
  msg += std::to_string(ammo->ammo_type) + ";";
  return msg;
}

ProjectileDisplay * Projectile::from_string(std::string to_read) {
  std::string msg = to_read;
  ProjectileDisplay * display = new ProjectileDisplay();
  display->name = msg.substr(0, msg.find(';'));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->id = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->projectile_type = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->x = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->y = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->orientation = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damages[i] = stoi(msg.substr(0, msg.find(';')));
    msg = msg.substr(msg.find(';') + 1, msg.length());
  }
  display->speed = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->area = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  std::string float_value = msg.substr(0, msg.find(';'));
  display->waste_per_tile = stof(float_value);
  if(display->waste_per_tile == 0) {
    std::replace(float_value.begin(), float_value.end(), '.', ',');
    display->waste_per_tile = stof(float_value);
  }
  msg = msg.substr(msg.find(';') + 1, msg.length());
  float_value = msg.substr(0, msg.find(';'));
  display->waste_per_tile_area = stof(float_value);
  if(display->waste_per_tile_area == 0) {
    std::replace(float_value.begin(), float_value.end(), '.', ',');
    display->waste_per_tile_area = stof(float_value);
  }
  msg = msg.substr(msg.find(';') + 1, msg.length());
  float_value = msg.substr(0, msg.find(';'));
  display->waste_per_hit = stof(float_value);
  if(display->waste_per_hit == 0) {
    std::replace(float_value.begin(), float_value.end(), '.', ',');
    display->waste_per_hit = stof(float_value);
  }
  msg = msg.substr(msg.find(';') + 1, msg.length());

  return display;
}


Projectile * Projectile::full_from_string(std::string to_read) {
  std::string msg = to_read;
  const std::string name = msg.substr(0, msg.find('/'));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int projectile_type = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  std::string homing_str = msg.substr(0, msg.find('/'));
  bool homing = (homing_str == "1");
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int current_map_id = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int x = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int y = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int dest_x = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int dest_y = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  std::string skill_str = msg.substr(0, msg.find('/'));
  Skill * skill = nullptr;
  if(skill_str != "none") {

  }
  msg = msg.substr(msg.find('/') + 1, msg.length());

  std::string target_str = msg.substr(0, msg.find('/'));
  Character * target = nullptr;
  if(target_str != "none") {

  }
  msg = msg.substr(msg.find('/') + 1, msg.length());
  std::string owner_str = msg.substr(0, msg.find('/'));
  Character * owner = nullptr;
  if(owner_str != "none") {

  }
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int orientation = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int speed = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int area = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  int overcharge = stoi(msg.substr(0, msg.find('/')));
  msg = msg.substr(msg.find('/') + 1, msg.length());
  std::string float_value = msg.substr(0, msg.find(';'));
  float waste_per_tile = stof(float_value);
  if(waste_per_tile == 0) {
    std::replace(float_value.begin(), float_value.end(), '.', ',');
    waste_per_tile = stof(float_value);
  }
  msg = msg.substr(msg.find(';') + 1, msg.length());
  float_value = msg.substr(0, msg.find(';'));
  float waste_per_tile_area = stof(float_value);
  if(waste_per_tile_area == 0) {
    std::replace(float_value.begin(), float_value.end(), '.', ',');
    waste_per_tile_area = stof(float_value);
  }
  msg = msg.substr(msg.find(';') + 1, msg.length());
  float_value = msg.substr(0, msg.find(';'));
  float waste_per_hit = stof(float_value);
  if(waste_per_hit == 0) {
    std::replace(float_value.begin(), float_value.end(), '.', ',');
    waste_per_hit = stof(float_value);
  }
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int damages[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] = stoi(msg.substr(0, msg.find('/')));
    msg = msg.substr(msg.find('/') + 1, msg.length());
  }
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
    overcharge,
    waste_per_tile,
    waste_per_tile_area,
    waste_per_hit,
    damages
  );
}

Ammunition * Projectile::ammo_from_string(std::string to_read) {
  std::string msg = to_read;
  Ammunition * ammo = new Ammunition();
  ammo->projectile = Projectile::full_from_string(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  ammo->number = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  ammo->gold_value = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  ammo->ammo_type = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  return ammo;
}
