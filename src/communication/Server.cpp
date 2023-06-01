#include <string>

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/World.h"

#include "data/actions/Action.h"
#include "data/actions/BaseAction.h"
#include "data/actions/EconomicsAction.h"
#include "data/actions/GearAction.h"
#include "data/actions/SkillAction.h"
#include "data/actions/TalkingAction.h"
#include "data/actions/TargetedAction.h"

#include "communication/Socket.h"

#include "communication/Server.h"

#include "util/String.h"

namespace Server {
  Action * receiveAction(Socket s, Character * user, Adventure * adventure) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    std::stringstream * ss = new std::stringstream(msg);
    int type = String::extract_int(ss);
    Action * action;
    switch(type) {
      case IDLE:
      case RESPITE:
      case REST:
      case BREAKPOINT:
        action = new BaseAction(type, adventure, nullptr, user);
        break;
      case MOVE:
      case STRIKE:
      case HEAVY_STRIKE:
      case SHOOT: {
        Target * target = Map::target_from_string(String::extract(ss));
        action = new TargetedAction(type, adventure, nullptr, user);
        ((TargetedAction *) action)->setTarget(target);
        break;
      }
      case RELOAD:
      case SWAP_GEAR:
      case GRAB:
      case USE_ITEM: {
        long item_id = String::extract_long(ss);
        action = new GearAction(type, adventure, nullptr, user, item_id);
        break;
      }
      case USE_SKILL: {
        Target * target = Map::target_from_string(String::extract(ss));
        Skill * skill = (Skill *) adventure->getDatabase()->getSkill(String::extract(ss));
        int overcharge_power = String::extract_int(ss);
        int overcharge_duration = String::extract_int(ss);
        int overcharge_range = String::extract_int(ss);
        action = new SkillAction(type, adventure, nullptr, user);
        ((SkillAction *) action)->setTarget(target);
        ((SkillAction *) action)->setSkill(skill);
        ((SkillAction *) action)->setOverchargePower(overcharge_power);
        ((SkillAction *) action)->setOverchargeRange(overcharge_duration);
        ((SkillAction *) action)->setOverchargeDuration(overcharge_range);
        break;
      }
      case TALKING: {
        action = new BaseAction(IDLE, adventure, nullptr, user);
      }
      case ECONOMICS: {
        action = new BaseAction(IDLE, adventure, nullptr, user);
      }
      default:
        delete ss;
        action = new BaseAction(IDLE, adventure, nullptr, user);
    }
    action->computeTime(adventure);
    return action;
  }

  Character * receiveChoices(Socket s, Adventure * adventure) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    std::stringstream * ss = new std::stringstream(msg);

    std::string  name = String::extract(ss);
    Attributes * attr = nullptr;
    std::string attr_name = String::extract(ss);
    for(Attributes * attr2 : adventure->getStartingAttributes()) {
      if(attr2->name == attr_name) {
        attr = attr2;
        break;
      }
    }
    if(attr == nullptr) {
      return nullptr;
    }
    Way * race = nullptr;
    std::string race_name = String::extract(ss);
    for(Way * way : adventure->getStartingWays()) {
      if(way->name == race_name && way->type == RACE) {
        race = way;
        break;
      }
    }
    if(race == nullptr) {
      return nullptr;
    }
    Way * origin = nullptr;
    std::string origin_name = String::extract(ss);
    for(Way * way : adventure->getStartingWays()) {
      if(way->name == origin_name && way->type == ORIGIN) {
        origin = way;
        break;
      }
    }
    if(origin == nullptr) {
      return nullptr;
    }
    Way * culture = nullptr;
    std::string culture_name = String::extract(ss);
    for(Way * way : adventure->getStartingWays()) {
      if(way->name == culture_name && way->type == CULTURE) {
        culture = way;
        break;
      }
    }
    if(culture == nullptr) {
      return nullptr;
    }
    Way * religion = nullptr;
    std::string religion_name = String::extract(ss);
    for(Way * way : adventure->getStartingWays()) {
      if(way->name == religion_name && way->type == RELIGION) {
        religion = way;
        break;
      }
    }
    if(religion == nullptr) {
      return nullptr;
    }
    Way * profession = nullptr;
    std::string profession_name = String::extract(ss);
    for(Way * way : adventure->getStartingWays()) {
      if(way->name == profession_name && way->type == PROFESSION) {
        profession = way;
        break;
      }
    }
    if(profession == nullptr) {
      return nullptr;
    }
    std::list<std::pair<const std::string, const std::string>> pairs = std::list<std::pair<const std::string, const std::string>>();
    pairs.push_back(std::make_pair(race->name, origin->name));
    pairs.push_back(std::make_pair(race->name, culture->name));
    pairs.push_back(std::make_pair(race->name, religion->name));
    pairs.push_back(std::make_pair(race->name, profession->name));
    pairs.push_back(std::make_pair(origin->name, culture->name));
    pairs.push_back(std::make_pair(origin->name, religion->name));
    pairs.push_back(std::make_pair(origin->name, profession->name));
    pairs.push_back(std::make_pair(culture->name, religion->name));
    pairs.push_back(std::make_pair(culture->name, profession->name));
    pairs.push_back(std::make_pair(religion->name, profession->name));
    for(std::pair<const std::string, const std::string> pair1 : pairs) {
      for(std::pair<const std::string, const std::string> pair2 : adventure->getDatabase()->getWaysIncompatibilities()) {
        if((pair1.first == pair2.first && pair1.second == pair2.second) ||
          (pair1.first == pair2.second && pair1.second == pair2.first)) {
            return nullptr;
        }
      }
    }
    Character * player = adventure->spawnPlayer(name, attr, race, origin, culture, religion, profession);
    player->setNeedToSend(true);
    try {
      s.write(player->full_to_string(adventure));
    } catch (const CloseException &e) {
      throw e;
    }
    delete ss;
    return player;
  }

  void sendState(Socket s, Character * player, Adventure * adventure) {
    try {
      std::stringstream * ss = new std::stringstream();
      if(player->needToSend()) {
        player->setNeedToSend(false);
        String::insert(ss, player->full_to_string(adventure));
      }
      else {
        String::insert(ss, "0");
      }
      String::insert(ss, adventure->state_to_string(player));
      String::insert_bool(ss, player->getNeedToUpdateActions());
      s.write(ss->str());
      delete ss;
    } catch (const CloseException &e) {
      throw e;
    }
  }
  
  void sendAdventure(Socket s, Adventure * adventure) {
    try {
      s.write(adventure->filePath);
    } catch (const CloseException &e) {
      throw e;
    }
  }
}
