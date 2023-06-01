#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Way.h"
#include "data/World.h"

#include "data/items/Item.h"
#include "data/items/SerialItem.h"
#include "data/items/AmmunitionItem.h"
#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"

#include "data/skills/Skill.h"

#include "data/actions/Action.h"
#include "data/actions/BaseAction.h"
#include "data/actions/EconomicsAction.h"
#include "data/actions/GearAction.h"
#include "data/actions/SkillAction.h"
#include "data/actions/TalkingAction.h"
#include "data/actions/TargetedAction.h"

#include "ai/AI.h"

Action * AI::getActions(Adventure * adventure, Character * c) {
  return nullptr;
}

float AI::getFollowOrientation(Adventure * adventure, Character * self, int x, int y) {
  return MapUtil::getOrientationToTarget(self->getX(), self->getY(), x, y);
}

float AI::getFleeOrientation(Adventure * adventure, Character * self, int x, int y) {
  float orientation = getFollowOrientation(adventure, self, x, y);
  if(orientation == 360.F) {
    return orientation;
  }
  orientation += 180.F;
  return orientation >= 360.F ? orientation - 360.F : orientation;
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
  int range = (int) MapUtil::distance(self->getX(), self->getY(), target_x, target_y);
  if(range == 0) {
    return new BaseAction(IDLE, adventure, nullptr, self);
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
      Target * target = new Target();
      target->type = TILE;
      target->id = self->getCurrentMapId();
      target->x = tp_target.x;
      target->y = tp_target.y;
      return new SkillAction(USE_SKILL, adventure, nullptr, self, target, skill, 1, 1, 1);
    }
  }
  if(path.size() > 0) {
    MapUtil::Pair next = path[0];
    Target * target = new Target();
    target->type = TILE;
    target->id = self->getCurrentMapId();
    target->x = next.x;
    target->y = next.y;
    return new TargetedAction(MOVE, adventure, nullptr, self, target);
    //return new Action(MOVE, adventure, nullptr, self, MapUtil::getOrientationToTarget(self->getX(), self->getY(), self->getDX(), self->getDY(), next.x, next.y, 0.F, 0.F), nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  Action * action = new BaseAction(IDLE, adventure, nullptr, self);
  return action;
}

Action * AI::eat(Adventure * adventure, Character * self) {
  if(!self->getRace()->need_to_eat) {
    return nullptr;
  }
  if(self->getRace()->can_eat_food) {
    for(Item * item : self->getItems()) {
      if(item->type == ITEM_CONSUMABLE && ((SerialItem *) item)->isFood()) {
        return new GearAction(USE_ITEM, adventure, nullptr, self, item->id);
      }
    }
    return trackPrey(adventure, self);
  // herbivorous creature dependent on a skill to eat
  }
  else {
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
    int k = 0;
    int i;
    int j;
    // TODO // infinite loop
    while(!correct_tile && (power <= map->sizeX || power <= map->sizeY)) {
      if(k % 8 == 0) {
        power++;
      }
      i = self->getX();
      j = self->getY();
      switch(k++ % 8 + 1) { // ORIENTATION
        case 0:
          j += power;
          break;
        case 1:
          j += power;
          i += power;
          break;
        case 2:
          i += power;
          break;
        case 3:
          j -= power;
          i += power;
          break;
        case 4:
          j -= power;
          break;
        case 5:
          j -= power;
          i -= power;
          break;
        case 6:
          i -= power;
          break;
        case 7:
          j += power;
          i -= power;
          break;
      }
      if(!(j >= 0 && j < map->sizeY) || !(i >= 0 && i < map->sizeX) || (correct_tile = map->getTile(j, i) == target )) {
        continue;
      }
    }
    Target * t = new Target();
    t->type = TILE;
    t->id = self->getCurrentMapId();
    t->x = i;
    t->y = j;
    if(i == self->getX() && j == self->getY()) {
      return new SkillAction(USE_SKILL, adventure, nullptr, self, t, skill, 1, 1, 1);
    }
    else {
      return new TargetedAction(MOVE, adventure, nullptr, self, t);
    }
  }
}

Action * AI::trackPrey(Adventure * adventure, Character * self) {
  Map * map = adventure->getWorld()->getMap(self->getCurrentMapId());
  Character * prey = nullptr;
  Loot * corpse = nullptr;
  // a predator can sense prey even if they are far.
  float max_distance_prey = (float) self->getVisionRange() * 2.F;
  float distance_prey = max_distance_prey;
  for(Character * other : map->getCharacters()) {
    if(other->getTeam() == "prey" || (other->getTeam() != self->getTeam() && self->getSatiety() < 15.F)) {
      float distance = MapUtil::distance(self->getX(), self->getY(), other->getX(), other->getY());
      if(other != self && distance <= max_distance_prey && distance < distance_prey) {
        prey = other;
        distance_prey = distance;
      }
    }
  }
  float distance_corpse = max_distance_prey;
  for(Loot * loot : map->getLoots()) {
    if(loot->type == CORPSE) {
      float distance = MapUtil::distance(self->getX(), self->getY(), loot->x, loot->y);
      if(distance <= max_distance_prey && distance < distance_corpse) {
        bool isFood = false;
        for(Item * item : loot->items) {
          if(item->type == ITEM_CONSUMABLE && ((SerialItem *) item)->isFood()) {
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
      return new GearAction(GRAB, adventure, nullptr, self, FOOD);
    }
    else {
      Target * target = new Target();
      target->type = TILE;
      target->id = self->getCurrentMapId();
      target->x = corpse->x;
      target->y = corpse->y;
      return new TargetedAction(MOVE, adventure, nullptr, self, target);
    }
  }
  return new BaseAction(IDLE, adventure, nullptr, self);
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
    float range = MapUtil::distance(self->getX(), self->getY(), threat->getX(), threat->getY());
    Skill * skill = nullptr;
    int maxDamage = 0;
    for(auto pair : skills) {
      if(pair.first != nullptr && pair.first->range >= range) {
        int rawDamage = threat->tryAttack(pair.second, STRIKE);
        if(rawDamage > maxDamage) {
          skill = pair.first;
          maxDamage = rawDamage;
        }
      }
      else if(pair.first == nullptr && self->getGear()->getWeapon()->range >= range) {
        int rawDamage = threat->tryAttack(pair.second, STRIKE);
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
    int rawDamage = pair.first->tryAttack(skills.at(pair.second), STRIKE);
    if(rawDamage > max) {
      skill = pair.second;
      target = pair.first;
      max = rawDamage;
    }
  }
  if(target != nullptr) {
    Target * t = new Target();
    t->type = CHARACTER;
    t->id = target->id;
    // float orientation = MapUtil::getOrientationToTarget(target->getX(), target->getY(), self->getX(), self->getY());
    if(skill != nullptr) {
      return new SkillAction(USE_SKILL, adventure, nullptr, self, t, skill, 1, 1, 1);
    }
    if(!self->getGear()->getWeapon()->use_projectile) {
      return new TargetedAction(STRIKE, adventure, nullptr, self, t);
    }
    if(!self->getGear()->getWeapon()->use_ammo || self->getGear()->getWeapon()->getCurrentCapacity() > 0) {
      return new TargetedAction(SHOOT, adventure, nullptr, self, t);
    }
    AmmunitionItem * ammo = nullptr;
    ammo = self->canReload();
    if(ammo != nullptr) {
      return new GearAction(RELOAD, adventure, nullptr, self, ammo->id);
    }
    WeaponItem * weapon = nullptr;
    weapon = self->swapMelee();
    if(weapon != nullptr) {
      return new GearAction(RELOAD, adventure, nullptr, self, weapon->id);
    }
  }

  if(self->getGear()->getWeapon()->use_ammo && self->getGear()->getWeapon()->getCurrentCapacity() == 0) {
    AmmunitionItem * ammo = nullptr;
    ammo = self->canReload();
    if(ammo != nullptr) {
      return new GearAction(RELOAD, adventure, nullptr, self, ammo->id);
    }
    WeaponItem * weapon = nullptr;
    weapon = self->swapMelee();
    if(weapon != nullptr) {
      return new GearAction(SWAP_GEAR, adventure, nullptr, self, weapon->id);
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
