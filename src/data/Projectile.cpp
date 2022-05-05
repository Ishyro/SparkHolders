#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/Projectile.h"

long Projectile::getX() { return x; }
long Projectile::getY() { return y; }
long Projectile::getDestX() {
  if(target_type == SINGLE_CHARACTER) {
    return target->getX();
  }
  else {
    return dest_x;
  }
}
long Projectile::getDestY() {
  if(target_type == SINGLE_CHARACTER) {
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
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] -= (int) floor( ((float) damages[i]) * (1 - waste_per_tile));
  }
}

void Projectile::nextOrientation() {
  if(lost) {
    return;
  }
  float target_x = getDestX() - x;
  float target_y = getDestY() - y;
  int way_to_the_target = NORTH; // default if already same tile
  // projectile will simply move to its target
  if(target_y > 0.) {
    if(target_x == 0. || target_y / target_x > 2.) {
        way_to_the_target = NORTH;
    }
    else if(target_y / target_x > 0.5) {
        way_to_the_target = NORTH_EAST;
    }
    else if(target_y / target_x > 0.) {
        way_to_the_target = EAST;
    }
    else if(target_y / target_x > -0.5) {
        way_to_the_target = WEST;
    }
    else if(target_y / target_x > -2.) {
        way_to_the_target = NORTH_WEST;
    }
    else {
      way_to_the_target = NORTH;
    }
  }
  if(target_y < 0.) {
    if(target_x == 0. || target_y / target_x > 2.) {
        way_to_the_target = SOUTH;
    }
    else if(target_y / target_x > 0.5) {
        way_to_the_target = SOUTH_WEST;
    }
    else if(target_y / target_x > 0.) {
        way_to_the_target = WEST;
    }
    else if(target_y / target_x > -0.5) {
        way_to_the_target = EAST;
    }
    else if(target_y / target_x > -2.) {
        way_to_the_target = SOUTH_EAST;
    }
    else {
      way_to_the_target = SOUTH;
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
  int diff = (orientation - way_to_the_target) % 8;
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
    power += damages[i];
  }
  return power;
}

bool Projectile::noDamage() { return getRawDamage() == 0; }

void Projectile::setX(long x) { this->x = x; }
void Projectile::setY(long y) { this->y = y; }
void Projectile::setDestX(long dest_x) { this->dest_x = dest_x; }
void Projectile::setDestY(long dest_y) { this->dest_y = dest_y; }
void Projectile::setTarget(Character * target) { this->target = target; }
void Projectile::setOwner(Character * owner) { this->owner = owner; }
void Projectile::setLost(bool state) { lost = state; }

void Projectile::attack_single_target(Character * target, Adventure * adventure) {
  target->receiveAttack(damages, orientation);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] -= (int) floor( ((float) damages[i]) * (1 - waste_per_hit));
  }
  skill->activate(owner, target, adventure, overcharge);
}

void Projectile::attack_multiple_targets(std::list<Character *> characters, Adventure * adventure) {
  for(Character * target : characters) {
    // TOCHECK should be ok
    int distance = std::max(abs(x - target->getX()), abs(y - target->getY()));
    if(distance <= area) {
      int reducedDamages[DAMAGE_TYPE_NUMBER];
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        reducedDamages[i] = (int) floor( ((float) damages[i]) * pow(1 - waste_per_tile_area, distance));
      }
      target->receiveAttack(reducedDamages, NO_ORIENTATION);
      skill->activate(owner, target, adventure, overcharge);
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
    msg += std::to_string(damages[i]) + ";";
  }
  msg += std::to_string(speed) + ";";
  msg += std::to_string(area) + ";";
  msg += std::to_string(waste_per_tile) + ";";
  msg += std::to_string(waste_per_tile_area) + ";";
  msg += std::to_string(waste_per_hit) + ";";
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
  display->x = stol(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->y = stol(msg.substr(0, msg.find(';')));
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
  display->waste_per_tile = stof(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->waste_per_tile_area = stof(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  display->waste_per_hit = stof(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());

  return display;
}

std::string Projectile::ammo_to_string(Ammunition * ammo) {
  std::string msg = ammo->projectile->name + "|";
  msg += std::to_string(ammo->number) + "|";
  msg += std::to_string(ammo->gold_value) + "|";
  msg += std::to_string(ammo->ammo_type) + "|";
  return msg;
}

Ammunition * Projectile::ammo_from_string(std::string to_read) {
  std::string msg = to_read;
  Ammunition * ammo = new Ammunition();
  //std::string msg = ammo->projectile->name + "|";
  ammo->number = stoi(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  ammo->gold_value = stoi(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  ammo->ammo_type = stoi(msg.substr(0, msg.find('|')));
  msg = msg.substr(msg.find('|') + 1, msg.length());
  return ammo;
}
