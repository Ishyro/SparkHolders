#include <string>

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Action.h"
#include "data/Map.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/World.h"

#include "communication/Socket.h"

#include "communication/Server.h"

#include "utils/String.h"

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
    switch(type) {
      case MOVE: {
        float orientation = String::extract_float(ss);
        delete ss;
        return new Action(MOVE, adventure, nullptr, user, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
      }
      case REST:
        delete ss;
        return new Action(REST, adventure, nullptr, user, 0.F, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
      case SHOOT: {
        float orientation = String::extract_float(ss);
        const Character * target = adventure->getCharacter(String::extract_int(ss));
        int target_x = String::extract_int(ss);
        int target_y = String::extract_int(ss);
        delete ss;
        return new Action(SHOOT, adventure, nullptr, user, orientation, nullptr, target, target_x, target_y, nullptr, "", 1, 1, 1);
      }
      case STRIKE: {
        float orientation = String::extract_float(ss);
        const Character * target = adventure->getCharacter(String::extract_int(ss));
        int target_x = String::extract_int(ss);
        int target_y = String::extract_int(ss);
        delete ss;
        return new Action(STRIKE, adventure, nullptr, user, orientation, nullptr, target, target_x, target_y, nullptr, "", 1, 1, 1);
      }
      case RELOAD: {
        std::string object = String::extract(ss);
        delete ss;
        return new Action(RELOAD, adventure, nullptr, user, 0.F, nullptr, nullptr, 0, 0, nullptr, object, 1, 1, 1);
      }
      case SWAP_GEAR: {
        std::string object = String::extract(ss);
        delete ss;
        return new Action(SWAP_GEAR, adventure, nullptr, user, 0.F, nullptr, nullptr, 0, 0, nullptr, object, 1, 1, 1);
      }
      case GRAB:
        delete ss;
        return new Action(GRAB, adventure, nullptr, user, 0.F, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
      case USE_SKILL: {
        std::string object = String::extract(ss);
        float orientation = String::extract_float(ss);
        const Character * target = adventure->getCharacter(String::extract_int(ss));
        const Skill * skill = adventure->getDatabase()->getSkill(object);
        int target_x = String::extract_int(ss);
        int target_y = String::extract_int(ss);
        int overcharge_power = String::extract_int(ss);
        int overcharge_duration = String::extract_int(ss);
        int overcharge_range = String::extract_int(ss);
        delete ss;
        Action * action = new Action(USE_SKILL, adventure, nullptr, user, orientation, skill, target, target_x, target_y, nullptr, object, overcharge_power, overcharge_duration, overcharge_range);
        if(!skill->is_instant) {
          return action;
        }
        else {
          action->execute(adventure);
          Map * map = new Map(adventure->getWorld()->getMap(user->getCurrentMapId()), user, adventure->getDatabase());
          try {
            sendMap(s, map, user, adventure);
          } catch (const CloseException &e) {
            throw e;
          }
          delete map;
          return receiveAction(s, user, adventure);
        }
      }
      case USE_ITEM: {
        std::string object = String::extract(ss);
        delete ss;
        return new Action(USE_ITEM, adventure, nullptr, user, 0.F, nullptr, nullptr, 0, 0, nullptr, object, 1, 1, 1);
      }
      case TALKING: {
        std::string object = String::extract(ss);
        const Character * target = adventure->getCharacter(String::extract_int(ss));
        delete ss;
        return new Action(TALKING, adventure, nullptr, user, 0.F, nullptr, target, 0, 0, nullptr, object, 1, 1, 1);
      }
      case ECONOMICS: {
        std::string object = String::extract(ss);
        int object_type = String::extract_int(ss);
        const Character * target = adventure->getCharacter(String::extract_int(ss));
        delete ss;
        return new Action(ECONOMICS, adventure, nullptr, user, object_type, nullptr, target, 0, 0, nullptr, object, 1, 1, 1);
      }
      default:
        delete ss;
        return new Action(REST, adventure, nullptr, user, 0.F, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
    }
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

  void sendMap(Socket s, Map * map, Character * player, Adventure * adventure) {
    try {
      std::stringstream * ss = new std::stringstream();
      if(player->needToSend()) {
        player->setNeedToSend(false);
        *ss << std::to_string(player->id);
        String::insert(ss, player->full_to_string(adventure));
      }
      String::insert(ss, map->to_string(player, adventure));
      String::insert_bool(ss, player->getNeedToUpdateActions());
      s.write(ss->str());
      delete ss;
    } catch (const CloseException &e) {
      throw e;
    }
  }

  void sendStartingPossibilites(Socket s, Adventure * adventure) {
    std::list<Attributes *> attributes = adventure->getStartingAttributes();
    std::list<Way *> ways = adventure->getStartingWays();
    std::stringstream * ss = new std::stringstream();
    std::stringstream * ss_attributes = new std::stringstream();
    for(Attributes * attr : attributes) {
      String::insert(ss_attributes, attr->to_string());
    }
    String::insert(ss, ss_attributes->str());
    delete ss_attributes;
    std::stringstream * ss_ways = new std::stringstream();
    for(Way * way : ways) {
      String::insert(ss_ways, way->to_string());
    }
    String::insert(ss, ss_ways->str());
    delete ss_ways;
    try {
      s.write(ss->str());
    } catch (const CloseException &e) {
      throw e;
    }
    delete ss;
  }

  void sendWaysIncompabilities(Socket s, Adventure * adventure) {
    std::stringstream * ss_ways = new std::stringstream();
    for(std::pair<const std::string, const std::string> pair : adventure->getDatabase()->getWaysIncompatibilities()) {
      String::insert(ss_ways, pair.first);
      String::insert(ss_ways, pair.second);
    }
    try {
      s.write(ss_ways->str());
    } catch (const CloseException &e) {
      throw e;
    }
    delete ss_ways;
  }

  void sendTranslationPaths(Socket s, Adventure * adventure) {
    std::stringstream * ss_trads = new std::stringstream();
    for(std::string path : adventure->getDatabase()->getTranslationPaths()) {
      String::insert(ss_trads, path);
    }
    try {
      s.write(ss_trads->str());
    } catch (const CloseException &e) {
      throw e;
    }
    delete ss_trads;
  }
}
