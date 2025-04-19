#include "data/Adventure.h"
#include "data/Character.h"
#include "data/ways/Way.h"
#include "data/ways/Race.h"
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

#include "util/MathUtil.h"

#include "ai/AI.h"

Action * AI::getActions(Adventure * adventure, Character * c) {
  return nullptr;
}

float AI::getFollowOrientation(Adventure * adventure, Character * self, int32_t x, int32_t y) {
  //return MathUtil::getOrientationToTarget(self->getX(), self->getY(), (float) x + 0.5F, (float) y + 0.5F);
  return 0.F;
}

float AI::getFleeOrientation(Adventure * adventure, Character * self, int32_t x, int32_t y) {
  float orientation_z = getFollowOrientation(adventure, self, x, y);
  if(orientation_z == 360.F) {
    return orientation_z;
  }
  orientation_z += 180.F;
  return orientation_z >= 360.F ? orientation_z - 360.F : orientation_z;
}

std::vector<MathUtil::Pair> AI::getFollowPath(Adventure * adventure, Character * self, int32_t x, int32_t y) {
  //return MathUtil::getPathToTarget(adventure->getWorld()->getMap(self->getRegion()), self->getX(), self->getY(), x, y, false);
  return std::vector<MathUtil::Pair>();
}

void AI::selectHungriness(Character * self) {
  if(self->getHunger() >= 66.F) {
    hungry = false;
  }
  if(self->getHunger() <= 33.F) {
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

Action * AI::moveTowards(Adventure * adventure, Character * self, int32_t target_x, int32_t target_y) {
  return new BaseAction(ACTION_IDLE, adventure, nullptr, self);
  /*
  int32_t range = (int32_t) MathUtil::distance(self->getX(), self->getY(), target_x, target_y);
  if(range == 0) {
    return new BaseAction(ACTION_IDLE, adventure, nullptr, self);
  }
  std::vector<MathUtil::Pair> path = getFollowPath(adventure, self, target_x, target_y);
  if(path.size() > 2) {
    int32_t tp_range = 0;
    Skill * skill = nullptr;
    for(Skill * s : self->getSkills()) {
      if( (tp_range = s->isTeleportSkill()) != 0 && s->getManaCost(1, 1, 1) < self->getMana()) {
        skill = s;
        break;
      }
    }
    if(skill != nullptr) {
      int32_t tp_index = std::min(tp_range, range) - 1; // path[0] is at range 1
      MathUtil::Pair tp_target = path[tp_index];
      MathUtil::Target * target = new MathUtil::Target();
      target->type = TARGET_COORDINATES;
      target->id = self->getCurrentMap()->id;
      target->x = tp_target.x;
      target->y = tp_target.y;
      target->z = self->getZ();
      return new SkillAction(ACTION_USE_SKILL, adventure, nullptr, self, target, skill, 1, 1, 1);
    }
  }
  if(path.size() > 0) {
    MathUtil::Pair next = path[0];
    MathUtil::Target * target = new MathUtil::Target();
    target->type = TARGET_COORDINATES;
    target->id = self->getCurrentMap()->id;
    target->x = next.x;
    target->y = next.y;
    target->z = self->getZ();
    return new TargetedAction(ACTION_MOVE, adventure, nullptr, self, target);
    //return new Action(MOVE, adventure, nullptr, self, MathUtil::getOrientationToTarget(self->getX(), self->getY(), self->getDX(), self->getDY(), next.x, next.y, 0.F, 0.F), nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
  }
  Action * action = new BaseAction(ACTION_IDLE, adventure, nullptr, self);
  return action;
  */
}

Action * AI::eat(Adventure * adventure, Character * self) {
  return new BaseAction(ACTION_IDLE, adventure, nullptr, self);
  /*
  if(!self->getRace()->need_to_eat) {
    return nullptr;
  }
  if(self->getRace()->can_eat_food) {
    for(ItemSlot * slot : self->getGear()->getBag()->getItems()) {
      if(slot->item->consumable && slot->item->isFood()) {
        return new GearAction(ACTION_USE_ITEM, adventure, nullptr, self, slot, nullptr);
      }
    }
    return trackPrey(adventure, self);
  // herbivorous creature dependent on a skill to eat
  }
  else {
    Map * map = self->getCurrentMap();
    Block * target = nullptr;
    Skill * skill = nullptr;
    for(Skill * s : self->getSkills()) {
      skill = s;
      if( (target = skill->isEatingSkill()) != nullptr) {
        break;
      }
    }
    bool correct_block = false;
    int32_t power = -1; // power = 0 in first loop turn
    int32_t k = 0;
    int32_t i;
    int32_t j;
    // TODO // infinite loop
    while(!correct_block && (power <= map->sizeX || power <= map->sizeY)) {
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
      if(!(j >= map->offsetY && j < map->sizeY + map->offsetY) || !(i >= map->offsetX && i < map->sizeX + map->offsetX) || (correct_block = map->getBlock(i, j) == target )) {
        continue;
      }
    }
    MathUtil::Target * t = new MathUtil::Target();
    t->type = TARGET_COORDINATES;
    t->id = self->getCurrentMap()->id;
    t->x = i;
    t->y = j;
    t->z = self->getZ();
    if(i == (int32_t) std::floor(self->getX()) && j == (int32_t) std::floor(self->getY())) {
      return new SkillAction(ACTION_USE_SKILL, adventure, nullptr, self, t, skill, 1, 1, 1);
    }
    else {
      return new TargetedAction(ACTION_MOVE, adventure, nullptr, self, t);
    }
  }
  */
}

Action * AI::trackPrey(Adventure * adventure, Character * self) {
  return new BaseAction(ACTION_IDLE, adventure, nullptr, self);
  /*
  Map * map = self->getCurrentMap();
  Character * prey = nullptr;
  Loot * corpse = nullptr;
  float max_distance_prey = (float) self->getVisionRange();
  float distance_prey = max_distance_prey;
  for(Character * other : map->getCharacters()) {
    if(other->getTeam() == "prey" || (other->getTeam() != self->getTeam() && self->getSatiety() < 15.F)) {
      float distance = MathUtil::distance(self->getX(), self->getY(), other->getX(), other->getY());
      if(other != self && distance <= max_distance_prey && distance < distance_prey) {
        prey = other;
        distance_prey = distance;
      }
    }
  }
  float distance_corpse = max_distance_prey;
  for(Loot * loot : map->getLoots()) {
    if(loot->type == LOOT_CORPSE) {
      float distance = MathUtil::distance(self->getX(), self->getY(), loot->x, loot->y);
      if(distance <= max_distance_prey && distance < distance_corpse) {
        bool isFood = false;
        for(Item * item : loot->items) {
          if(item->consumable && item->isFood()) {
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
      // TODO
      return new GearAction(ACTION_GRAB, adventure, nullptr, self, nullptr, nullptr);
    }
    else {
      MathUtil::Target * target = new MathUtil::Target();
      target->type = TARGET_COORDINATES;
      target->id = self->getCurrentMap()->id;
      target->x = corpse->x;
      target->y = corpse->y;
      target->z = self->getZ();
      return new TargetedAction(ACTION_MOVE, adventure, nullptr, self, target);
    }
  }
  return new BaseAction(ACTION_IDLE, adventure, nullptr, self);
  */
}

std::list<Character *> AI::getThreats(Adventure * adventure, Map * map, Character * self, int32_t range) {
  std::list<Character *> threats = std::list<Character *>();
  for(Character * other : map->getCharacters()) {
    if(adventure->getDatabase()->getRelation(self->getTeam(), other->getTeam()) == TEAM_ENEMY) {
      threats.push_front(other);
    }
    else if(MathUtil::distance(self->getCoord(), other->getCoord()) <= range
      && adventure->getDatabase()->getRelation(self->getTeam(), other->getTeam()) == TEAM_NEUTRAL) {
      threats.push_front(other);
    }
  }
  return threats;
}

std::map<Character *, Skill *> AI::getBestDamageSkills(std::list<Character *> threats, std::map<Skill *, std::array<float, DAMAGE_TYPE_NUMBER>> skills, Character * self) {
  std::map<Character *, Skill *> bestDamageSkills = std::map<Character *, Skill *>();
  for(Character * threat : threats) {
    float range = MathUtil::distance(self->getCoord(), threat->getCoord());
    Skill * skill = nullptr;
    int32_t maxDamage = 0;
    for(auto pair : skills) {
      if(pair.first != nullptr && pair.first->range >= range) {
        float rawDamage = threat->tryAttack(pair.second);
        if(rawDamage > maxDamage) {
          skill = pair.first;
          maxDamage = rawDamage;
        }
      }
      else if(pair.first == nullptr && self->getGear()->getWeapon_1()->range.y >= range) {
        int32_t rawDamage = threat->tryAttack(pair.second);
        if(rawDamage > maxDamage) {
          skill = pair.first;
          maxDamage = rawDamage;
        }
      }
    }
    if(skill != nullptr || self->getGear()->getWeapon_1()->range.y >= range) {
      bestDamageSkills.insert(std::make_pair(threat, skill));
    }
  }
  return bestDamageSkills;
}

Action * AI::attack(Adventure * adventure, std::list<Character *> threats, Character * self) {
  std::map<Skill *, std::array<float, DAMAGE_TYPE_NUMBER>> skills = self->getDamageSkills();
  std::map<Character *, Skill *> bestDamageSkills = getBestDamageSkills(threats, skills, self);
  float max = 0.F;
  Skill * skill = nullptr;
  Character * target = nullptr;
  for(auto pair : bestDamageSkills) {
    float rawDamage = pair.first->tryAttack(skills.at(pair.second));
    if(rawDamage > max) {
      skill = pair.second;
      target = pair.first;
      max = rawDamage;
    }
  }
  if(target != nullptr) {
    MathUtil::Target * t = new MathUtil::Target();
    t->type = TARGET_CHARACTER;
    t->character = target;
    // float orientation_z = MathUtil::getOrientationToTarget(target->getX(), target->getY(), self->getX(), self->getY());
    if(skill != nullptr) {
      return new SkillAction(ACTION_USE_SKILL, adventure, nullptr, self, t, skill, skill->getManaCost());
    }
    /*
    if(!self->getGear()->getWeapon_1()->use_projectile) {
      return new TargetedAction(ACTION_ATTACK, adventure, nullptr, self, t);
    }
    if(!self->getGear()->getWeapon_1()->use_ammo || self->getGear()->getWeapon_1()->getCurrentCapacity() > 0) {
      return new TargetedAction(ACTION_ATTACK, adventure, nullptr, self, t);
    }
      */
    ItemSlot * slot = nullptr;
    slot = self->canReload(ITEM_SLOT_WEAPON_1);
    if(slot != nullptr) {
      return new GearAction(ACTION_RELOAD, adventure, nullptr, self, slot, nullptr);
    }
    WeaponItem * weapon = nullptr;
    weapon = self->getGear()->getBackupWeapon();
    if(weapon != nullptr) {
      slot->slot = ITEM_SLOT_WEAPON_3;
      return new GearAction(ACTION_SWAP_GEAR, adventure, nullptr, self, slot, nullptr);
    }
  }

  if(self->getGear()->getWeapon_1()->use_ammo && self->getGear()->getWeapon_1()->getCurrentCapacity() == 0) {
    ItemSlot * slot = nullptr;
    slot = self->canReload(ITEM_SLOT_WEAPON_1);
    if(slot != nullptr) {
      return new GearAction(ACTION_RELOAD, adventure, nullptr, self, slot, nullptr);
    }
    WeaponItem * weapon = nullptr;
    weapon = self->getGear()->getBackupWeapon();
    if(weapon != nullptr) {
      slot->slot = ITEM_SLOT_WEAPON_3;
      return new GearAction(ACTION_SWAP_GEAR, adventure, nullptr, self, slot, nullptr);
    }
  }
  max = 0;
  for(Character * threat : threats) {
    int32_t powerScore = threat->getPowerScore();
    if(powerScore > max) {
      target = threat;
      max = powerScore;
    }
  }
  return moveTowards(adventure, self, target->getCoord().x, target->getCoord().y);
}
