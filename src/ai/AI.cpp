#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"

#include "ai/AI.h"

Action * AI::getAction(Adventure * adventure, Character * c) {
  return nullptr;
}

int AI::getFleeOrientation(Character * self, long x, long y) {
  float target_x = x - self->getX();
  float target_y = y - self->getY();
  int orientation = NORTH; // default if already same tile
  if(target_y < 0.) {
    if(target_x == 0. || target_y / target_x > 2.) {
        orientation = NORTH;
    }
    else if(target_y / target_x > 0.5) {
        orientation = NORTH_EAST;
    }
    else if(target_y / target_x > 0.) {
        orientation = EAST;
    }
    else if(target_y / target_x > -0.5) {
        orientation = WEST;
    }
    else if(target_y / target_x > -2.) {
        orientation = NORTH_WEST;
    }
    else {
      orientation = NORTH;
    }
  }
  if(target_y > 0.) {
    if(target_x == 0. || target_y / target_x > 2.) {
        orientation = SOUTH;
    }
    else if(target_y / target_x > 0.5) {
        orientation = SOUTH_WEST;
    }
    else if(target_y / target_x > 0.) {
        orientation = WEST;
    }
    else if(target_y / target_x > -0.5) {
        orientation = EAST;
    }
    else if(target_y / target_x > -2.) {
        orientation = SOUTH_EAST;
    }
    else {
      orientation = SOUTH;
    }
  }
  // target_y == 0
  else {
    if(target_x < 0.) {
      orientation = EAST;
    }
    else if(target_x > 0.) {
      orientation = WEST;
    }
  }
  return orientation;
}

int AI::getFollowOrientation(Character * self, long x, long y) {
  float target_x = x - self->getX();
  float target_y = y - self->getY();
  int orientation = NORTH; // default if already same tile
  if(target_y > 0.) {
    if(target_x == 0. || target_y / target_x > 2.) {
        orientation = NORTH;
    }
    else if(target_y / target_x > 0.5) {
        orientation = NORTH_EAST;
    }
    else if(target_y / target_x > 0.) {
        orientation = EAST;
    }
    else if(target_y / target_x > -0.5) {
        orientation = WEST;
    }
    else if(target_y / target_x > -2.) {
        orientation = NORTH_WEST;
    }
    else {
      orientation = NORTH;
    }
  }
  if(target_y < 0.) {
    if(target_x == 0. || target_y / target_x > 2.) {
        orientation = SOUTH;
    }
    else if(target_y / target_x > 0.5) {
        orientation = SOUTH_WEST;
    }
    else if(target_y / target_x > 0.) {
        orientation = WEST;
    }
    else if(target_y / target_x > -0.5) {
        orientation = EAST;
    }
    else if(target_y / target_x > -2.) {
        orientation = SOUTH_EAST;
    }
    else {
      orientation = SOUTH;
    }
  }
  // target_y == 0
  else {
    if(target_x > 0.) {
      orientation = EAST;
    }
    else if(target_x < 0.) {
      orientation = WEST;
    }
  }
  return orientation;
}
