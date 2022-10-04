#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"


#include "ai/AI.h"

Action * AI::getAction(Adventure * adventure, Character * c) {
  return nullptr;
}

int AI::getFleeOrientation(Adventure * adventure, Character * self, int x, int y) {
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

int AI::getFollowOrientation(Adventure * adventure, Character * self, int x, int y) {
  return MapUtil::getOrientationToTarget(adventure->getWorld()->getMap(self->getCurrentMapId()), self->getX(), self->getY(), x, y);
}

std::vector<MapUtil::Pair> AI::getFollowPath(Adventure * adventure, Character * self, int x, int y) {
  return MapUtil::getPathToTarget(adventure->getWorld()->getMap(self->getCurrentMapId()), self->getX(), self->getY(), x, y);
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
      return new Action(MOVE, self, getFollowOrientation(adventure, self, i, j), nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    }
  }
}

Action * AI::trackPrey(Adventure * adventure, Character * self) {
  Map * map = adventure->getWorld()->getMap(self->getCurrentMapId());
  Character * prey = nullptr;
  // a predator can sense prey even if they are far.
  int max_distance_prey = self->getVisionRange() * 2;
  int distance_prey = max_distance_prey;
  for(Character * other : map->getCharacters()) {
    if(other->getTeam() == "prey" || (other->getTeam() != self->getTeam() && self->getSatiety() < 15.F)) {
      int distance = MapUtil::distance(self->getX(), self->getY(), other->getX(), other->getY());
      if(other != self && distance <= max_distance_prey && distance < distance_prey) {
        prey = other;
        distance_prey = distance;
      }
    }
  }
  if(prey != nullptr) {
    std::vector<MapUtil::Pair> path = getFollowPath(adventure, self, prey->getX(), prey->getY());
    if(path.size() > 2) {
      int tp_range = 0;
      Skill * skill = nullptr;
      for(Skill * s : self->getSkills()) {
        skill = s;
        if( (tp_range = skill->isTeleportSkill()) != 0) {
          break;
        }
      }
      if(tp_range != 0) {
        int tp_index = std::min(tp_range, distance_prey);
        MapUtil::Pair tp_target = path[tp_index];
        return new Action(USE_SKILL, self, self->getOrientation(), skill, nullptr, tp_target.x, tp_target.y, nullptr, "", 1, 1, 1);
      }
    }
    if(path.size() > 0) {
      MapUtil::Pair next = path[0];
      return new Action(MOVE, self, MapUtil::orientationToTarget(self->getX(), self->getY(), next.x, next.y), nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    }
  }
  return new Action(REST, self, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
}

std::list<Character *> AI::getThreats(Adventure * adventure, Character * self) {
  Map * map = adventure->getWorld()->getMap(self->getCurrentMapId());
  std::list<Character *> threats = std::list<Character *>();
  for(Character * other : map->getCharacters()) {
    if(other != self && map->canSee(self, other) && adventure->getDatabase()->getRelation(self->getTeam(), other->getTeam()) == ENEMY) {
      threats.push_front(other);
    }
  }
  return threats;
}
