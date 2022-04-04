#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/Projectile.h"

int Projectile::getX() { return x; }
int Projectile::getY() { return y; }
int Projectile::getDestX() {
  if(target_type == SINGLE_CHARACTER) {
    return target->getX();
  }
  else {
    return dest_x;
  }
}
int Projectile::getDestY() {
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
    damages[i] -= (int) floor( ((float) damages[i]) * waste_per_tile);
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

bool Projectile::noDamage() {
  return getRawDamage() == 0;
}

void Projectile::setLost(bool state) { lost = state; }

void Projectile::attack_single_target(Character * target, Adventure * adventure) {
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    target->receiveAttack(damages[i], i, orientation);
    damages[i] -= (int) floor( ((float) damages[i]) * waste_per_hit);
  }
  skill->activate(owner, target, adventure, overcharge);
}

void Projectile::attack_multiple_targets(std::list<Character *> characters, Adventure * adventure) {
  for(auto target : characters) {
    // TOCHECK should be ok
    int distance = std::max(abs(x - target->getX()), abs(y - target->getY()));
    if(distance <= area) {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        target->receiveAttack((int) floor( ((float) damages[i]) * distance * waste_per_tile_area), i, NO_ORIENTATION);
      }
      skill->activate(owner, target, adventure, overcharge);
    }
  }
}
