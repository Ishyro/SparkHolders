#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"

#include "ai/AI.h"

Action * AI::getAction(Adventure * adventure, Character * c) {
  return nullptr;
}

int AI::getFleeOrientation(Character * self, int x, int y) {
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

int AI::getFollowOrientation(Character * self, int x, int y) {
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

void AI::selectHungriness(Character * self) {
  if(self->getSatiety() >= 66.F) {
    hungry = false;
  }
  if(self->getSatiety() <= 33.F) {
    hungry = true;
  }
}

void AI::selectTiredness(Character * self) {
  if(self->getStamina() >= 66.F) {
    sleepy = false;
  }
  if(self->getStamina() <= 33.F) {
    sleepy = true;
  }
}

Action * AI::eat(Adventure * adventure, Character * self) {
  if(!self->need_to_eat) {
    return nullptr;
  }
  if(self->can_eat_food) {
    for(Item * item : self->getItems()) {
      if(item->isFood()) {
        return new Action(USE_ITEM, self, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, item->name, 1, 1, 1);
      }
    }
    return trackPrey(adventure, self);
  // herbivorous creature dependent on a skill to eat
  } else {
    Map * map = adventure->getWorld()->getMap(self->getCurrentMapId());
    Tile * target = nullptr;
    Skill * skill = nullptr;
    for(Skill * s : self->getSkills()) {
      skill = s;
      if( (target = skill->isEatingSkill()) != nullptr) {
        break;
      }
    }
    bool correct_tile = false;
    int power = -1; // power = 0 in first loop turn
    int k = NORTH;
    int i;
    int j;
    while(!correct_tile) {
      if(k % 8 == 0) {
        power++;
      }
      i = self->getX();
      j = self->getY();
      switch(k++ % 8 + 1) { // ORIENTATION
        case NORTH:
          j += power;
          break;
        case NORTH_EAST:
          j += power;
          i += power;
          break;
        case EAST:
          i += power;
          break;
        case SOUTH_EAST:
          j -= power;
          i += power;
          break;
        case SOUTH:
          j -= power;
          break;
        case SOUTH_WEST:
          j -= power;
          i -= power;
          break;
        case WEST:
          i -= power;
          break;
        case NORTH_WEST:
          j += power;
          i -= power;
          break;
      }
      if(!(j >= 0 && j < map->sizeY) || !(i >= 0 && i < map->sizeX) || (correct_tile = map->getTile(j, i) == target )) {
        continue;
      }
    }
    if(i == self->getX() && j == self->getY()) {
      return new Action(USE_SKILL, self, self->getOrientation(), skill, nullptr, i, j, nullptr, "", 1, 1, 1);
    } else {
      return new Action(MOVE, self, getFollowOrientation(self, i, j), nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    }
  }
}

Action * AI::trackPrey(Adventure * adventure, Character * self) {
  return nullptr;
}
