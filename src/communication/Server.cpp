#include <string>

#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/Block.h"
#include "data/World.h"

#include "data/actions/Action.h"
#include "data/actions/BaseAction.h"
#include "data/actions/EconomicsAction.h"
#include "data/actions/GearAction.h"
#include "data/actions/OrientedAction.h"
#include "data/actions/SkillAction.h"
#include "data/actions/TalkingAction.h"
#include "data/actions/TargetedAction.h"

#include "data/ways/Way.h"
#include "data/ways/Attributes.h"
#include "data/ways/Race.h"

#include "communication/Socket.h"

#include "communication/Server.h"

#include "util/String.h"

namespace Server {

  Action * receiveAction(std::string msg, Character * character, Adventure * adventure) {
    Action * result;
    std::stringstream * ss = new std::stringstream(msg);
    // ignore socket_msg_type
    String::extract(ss);
    result = readAction(String::extract(ss), character, adventure);
    delete ss;
    return result;
  }

  Action * readAction(std::string msg, Character * user, Adventure * adventure) {
    std::stringstream * ss = new std::stringstream(msg);
    int32_t type = String::extract_int(ss);
    Action * action;
    switch(type) {
      case ACTION_IDLE:
      case ACTION_RESPITE:
      case ACTION_REST:
      case ACTION_RUN:
      case ACTION_JUMP:
      case ACTION_BREAKPOINT:
      case ACTION_CHANNEL:
        action = new BaseAction(type, adventure, nullptr, user);
        break;
      case ACTION_MOVE: {
        float orientation = String::extract_float(ss);
        action = new OrientedAction(type, adventure, nullptr, user, orientation);
        break;
      }
      case ACTION_STRIKE:
      case ACTION_ACTIVATION: {
        Target * target = MathUtil::target_from_string(String::extract(ss), adventure);
        action = new TargetedAction(type, adventure, nullptr, user, target);
        break;
      }
      case ACTION_RELOAD:
      case ACTION_GRAB:
      case ACTION_USE_ITEM: {
        ItemSlot * slot = new ItemSlot();
        slot->id = String::extract_long(ss);
        slot->x = String::extract_int(ss);
        slot->y = String::extract_int(ss);
        slot->slot = String::extract_int(ss);
        action = new GearAction(type, adventure, nullptr, user, slot, nullptr);
        break;
      }
      case ACTION_SWAP_GEAR: {
        ItemSlot * slot1 = new ItemSlot();
        slot1->id = String::extract_long(ss);
        slot1->x = String::extract_int(ss);
        slot1->y = String::extract_int(ss);
        slot1->slot = String::extract_int(ss);
        ItemSlot * slot2 = new ItemSlot();
        slot2->id = String::extract_long(ss);
        slot2->x = String::extract_int(ss);
        slot2->y = String::extract_int(ss);
        slot2->slot = String::extract_int(ss);
        action = new GearAction(type, adventure, nullptr, user, slot1, slot2);
        break;
      }
      case ACTION_USE_SKILL: {
        Target * target = MathUtil::target_from_string(String::extract(ss), adventure);
        Skill * skill = (Skill *) adventure->getDatabase()->getSkill(String::extract(ss));
        int32_t mana_cost = String::extract_int(ss);
        action = new SkillAction(type, adventure, nullptr, user, target, skill, mana_cost);
        break;
      }
      case ACTION_TALKING: {
        action = new BaseAction(ACTION_IDLE, adventure, nullptr, user);
      }
      case ACTION_ECONOMICS: {
        action = new BaseAction(ACTION_IDLE, adventure, nullptr, user);
      }
      default:
        delete ss;
        action = new BaseAction(ACTION_IDLE, adventure, nullptr, user);
    }
    return action;
  }

  Character * receiveChoices(std::string msg, Adventure * adventure) {
    std::stringstream * ss = new std::stringstream(msg);
    // ignore socket_msg_type
    String::extract(ss);
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
    Race * race = nullptr;
    std::string race_name = String::extract(ss);
    for(Way * way : adventure->getStartingWays()) {
      if(way->name == race_name && way->type == WAY_RACE) {
        race = (Race *) way;
        break;
      }
    }
    if(race == nullptr) {
      return nullptr;
    }
    Way * origin = nullptr;
    std::string origin_name = String::extract(ss);
    for(Way * way : adventure->getStartingWays()) {
      if(way->name == origin_name && way->type == WAY_ORIGIN) {
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
      if(way->name == culture_name && way->type == WAY_CULTURE) {
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
      if(way->name == religion_name && way->type == WAY_RELIGION) {
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
      if(way->name == profession_name && way->type == WAY_PROFESSION) {
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
    Character * character = adventure->spawnPlayer(name, attr, race, origin, culture, religion, profession);
    delete ss;
    return character;
  }

  void sendState(Socket s, Character * character, Adventure * adventure) {
    try {
      std::stringstream * ss = new std::stringstream();
      String::insert_int(ss, SOCKET_MSG_STATE);
      String::insert(ss, adventure->state_to_string(character));
      String::insert_bool(ss, character->getAction() == nullptr);
      s.write(ss->str());
      delete ss;
    } catch (const CloseException &e) {
      throw e;
    }
  }

  void sendAdventure(Socket s, Adventure * adventure, bool master) {
    try {
      std::stringstream * ss = new std::stringstream();
      String::insert_int(ss, SOCKET_MSG_ADVENTURE);
      String::insert(ss, adventure->filePath);
      String::insert_long(ss, Settings::getTickRate().count());
      String::insert_bool(ss, master);
      s.write(ss->str());
      delete ss;
    } catch (const CloseException &e) {
      throw e;
    }
  }

  void sendCharacter(Socket s, Character * character, Adventure * adventure) {
    try {
      std::stringstream * ss = new std::stringstream();
      String::insert_int(ss, SOCKET_MSG_CHARACTER);
      String::insert(ss, character->full_to_string(adventure));
      character->setNeedToSend(false);
      s.write(ss->str());
      delete ss;
    } catch (const CloseException &e) {
      throw e;
    }
  }

  void sendAction(Socket s) {
    try {
      std::stringstream * ss = new std::stringstream();
      String::insert_int(ss, SOCKET_MSG_ACTION);
      s.write(ss->str());
      delete ss;
    } catch (const CloseException &e) {
      throw e;
    }
  }
}
