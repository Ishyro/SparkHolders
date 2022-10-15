#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"


#include "ai/AI.h"

Action * AI::getAction(Adventure * adventure, Character * c) {
  return nullptr;
}

int AI::getFleeOrientation(Adventure * adventure, Character * self, int x, int y) {
  return (MapUtil::getDirectOrientationToTarget(x - self->getX(), y - self->getY()) + 4) % 8;
}

int AI::getFollowOrientation(Adventure * adventure, Character * self, int x, int y) {
  return MapUtil::getOrientationToTarget(adventure->getWorld()->getMap(self->getCurrentMapId()), self->getX(), self->getY(), x, y, false);
}

std::vector<MapUtil::Pair> AI::getFollowPath(Adventure * adventure, Character * self, int x, int y) {
  return MapUtil::getPathToTarget(adventure->getWorld()->getMap(self->getCurrentMapId()), self->getX(), self->getY(), x, y, false);
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

Action * AI::moveTowards(Adventure * adventure, Character * self, int target_x, int target_y) {
  int range = MapUtil::distance(self->getX(), self->getY(), target_x, target_y);
  if(range == 0) {
    return new Action(REST, self, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  std::vector<MapUtil::Pair> path = getFollowPath(adventure, self, target_x, target_y);
  if(path.size() > 2) {
    int tp_range = 0;
    Skill * skill = nullptr;
    for(Skill * s : self->getSkills()) {
      if( (tp_range = s->isTeleportSkill()) != 0 && s->getManaCost(1, 1, 1) < self->getMana()) {
        skill = s;
        break;
      }
    }
    if(skill != nullptr) {
      int tp_index = std::min(tp_range, range) - 1; // path[0] is at range 1
      MapUtil::Pair tp_target = path[tp_index];
      return new Action(USE_SKILL, self, self->getOrientation(), skill, nullptr, tp_target.x, tp_target.y, nullptr, "", 1, 1, 1);
    }
  }
  if(path.size() > 0) {
    MapUtil::Pair next = path[0];
    return new Action(MOVE, self, MapUtil::getOrientationToTarget(self->getX(), self->getY(), next.x, next.y), nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  return new Action(REST, self, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
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
  Loot * corpse = nullptr;
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
  int distance_corpse = max_distance_prey;
  for(Loot * loot : map->getLoots()) {
    if(loot->type == CORPSE) {
      int distance = MapUtil::distance(self->getX(), self->getY(), loot->x, loot->y);
      if(distance <= max_distance_prey && distance < distance_corpse) {
        bool isFood = false;
        for(Item * item : loot->items) {
          if(item->isFood()) {
            isFood = true;
            break;
          }
        }
        if(isFood) {
          corpse = loot;
          distance_corpse = distance;
        }
      }
    }
  }
  if(distance_prey < distance_corpse) {
    if(prey != nullptr) {
      return moveTowards(adventure, self, prey->getX(), prey->getY());
    }
  }
  else {
    if(distance_corpse == 0) {
      return new Action(GRAB, self, FOOD, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    }
    else {
      return new Action(MOVE, self, getFollowOrientation(adventure, self, corpse->x, corpse->y), nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    }
  }
  return new Action(REST, self, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
}

std::list<Character *> AI::getThreats(Adventure * adventure, Map * map, Character * self, int range) {
  std::list<Character *> threats = std::list<Character *>();
  for(Character * other : map->getCharacters()) {
    if(adventure->getDatabase()->getRelation(self->getTeam(), other->getTeam()) == ENEMY) {
      threats.push_front(other);
    }
    else if(MapUtil::distance(self->getX(), self->getY(), other->getX(), other->getY()) <= range
      && adventure->getDatabase()->getRelation(self->getTeam(), other->getTeam()) == NEUTRAL) {
      threats.push_front(other);
    }
  }
  return threats;
}

std::map<Character *, Skill *> AI::getBestDamageSkills(std::list<Character *> threats, std::map<Skill *, std::array<int, DAMAGE_TYPE_NUMBER>> skills, Character * self) {
  std::map<Character *, Skill *> bestDamageSkills = std::map<Character *, Skill *>();
  for(Character * threat : threats) {
    int range = MapUtil::distance(self->getX(), self->getY(), threat->getX(), threat->getY());
    Skill * skill = nullptr;
    int maxDamage = 0;
    for(auto pair : skills) {
      if(pair.first != nullptr && pair.first->range >= range) {
        int rawDamage = threat->tryAttack(pair.second);
        if(rawDamage > maxDamage) {
          skill = pair.first;
          maxDamage = rawDamage;
        }
      }
      else if(pair.first == nullptr && self->getGear()->getWeapon()->range >= range) {
        int rawDamage = threat->tryAttack(pair.second);
        if(rawDamage > maxDamage) {
          skill = pair.first;
          maxDamage = rawDamage;
        }
      }
    }
    if(skill != nullptr || self->getGear()->getWeapon()->range >= range) {
      bestDamageSkills.insert(std::make_pair(threat, skill));
    }
  }
  return bestDamageSkills;
}

Action * AI::attack(Adventure * adventure, std::list<Character *> threats, Character * self) {
  std::map<Skill *, std::array<int, DAMAGE_TYPE_NUMBER>> skills = self->getDamageSkills();
  std::map<Character *, Skill *> bestDamageSkills = getBestDamageSkills(threats, skills, self);
  int max = 0;
  Skill * skill = nullptr;
  Character * target = nullptr;
  for(auto pair : bestDamageSkills) {
    int rawDamage = pair.first->tryAttack(skills.at(pair.second));
    if(rawDamage > max) {
      skill = pair.second;
      target = pair.first;
      max = rawDamage;
    }
  }
  if(target != nullptr) {
    int orientation = MapUtil::getDirectOrientationToTarget(target->getX() - self->getX(), target->getY() - self->getY());
    if(skill != nullptr) {
      return new Action(USE_SKILL, self, orientation, skill, target, target->getX(), target->getY(), nullptr, "", 1, 1, 1);
    }
    if(self->getGear()->getWeapon()->melee) {
      return new Action(FORCE_STRIKE, self, orientation, nullptr, target, target->getX(), target->getY(), nullptr, "", 1, 1, 1);
    }
    if(!self->getGear()->getWeapon()->use_ammo || self->getGear()->getWeapon()->getCurrentCapacity() > 0) {
      return new Action(SHOOT, self, orientation, nullptr, target, target->getX(), target->getY(), nullptr, "", 1, 1, 1);
    }
    Ammunition * ammo = nullptr;
    ammo = self->canReload();
    if(ammo != nullptr) {
      return new Action(RELOAD, self, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, ammo->projectile->name, 1, 1, 1);
    }
    Weapon * weapon = nullptr;
    weapon = self->swapMelee();
    if(weapon != nullptr) {
      return new Action(SWAP_GEAR, self, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, weapon->name, 1, 1, 1);
    }
  }
  if(self->getGear()->getWeapon()->use_ammo && self->getGear()->getWeapon()->getCurrentCapacity() == 0) {
    Ammunition * ammo = nullptr;
    ammo = self->canReload();
    if(ammo != nullptr) {
      return new Action(RELOAD, self, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, ammo->projectile->name, 1, 1, 1);
    }
    Weapon * weapon = nullptr;
    weapon = self->swapMelee();
    if(weapon != nullptr) {
      return new Action(SWAP_GEAR, self, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, weapon->name, 1, 1, 1);
    }
  }
  max = 0;
  for(Character * threat : threats) {
    int powerScore = threat->getPowerScore();
    if(powerScore > max) {
      target = threat;
      max = powerScore;
    }
  }
  return moveTowards(adventure, self, target->getX(), target->getY());
}
