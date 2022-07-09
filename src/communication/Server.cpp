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
        int orientation = String::extract_int(ss);
        return new Action(MOVE, user, orientation, nullptr, nullptr, 0, 0, nullptr, "", 1);
      }
      case REST:
        return new Action(REST, user, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, "", 1);
      case SHOOT: {
        int orientation = String::extract_int(ss);
        const Character * target = adventure->getCharacter(String::extract_int(ss));
        int target_x = String::extract_int(ss);
        int target_y = String::extract_int(ss);
        return new Action(SHOOT, user, orientation, nullptr, target, target_x, target_y, nullptr, "", 1);
      }
      case FORCE_STRIKE: {
        int orientation = String::extract_int(ss);
        const Character * target = adventure->getCharacter(String::extract_int(ss));
        int target_x = String::extract_int(ss);
        int target_y = String::extract_int(ss);
        return new Action(FORCE_STRIKE, user, orientation, nullptr, target, target_x, target_y, nullptr, "", 1);
      }
      case RELOAD: {
        std::string object = String::extract(ss);
        return new Action(RELOAD, user, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, object, 1);
      }
      case SWAP_GEAR: {
        std::string object = String::extract(ss);
        return new Action(SWAP_GEAR, user, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, object, 1);
      }
      case CHANGE_MAP: {
        MapLink * link = adventure->getWorld()->getMapLink(user->getY(), user->getX(), user->getCurrentMapId());
        if(link != nullptr) {
          return new Action(CHANGE_MAP, user, NO_ORIENTATION, nullptr, nullptr, 0, 0, link, "", 1);
        }
        return new Action(REST, user, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, "", 1);
      }
      case GRAB:
        return new Action(GRAB, user, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, "", 1);
      case USE_SKILL: {
        std::string object = String::extract(ss);
        int orientation = String::extract_int(ss);
        const Character * target = adventure->getCharacter(String::extract_int(ss));
        int target_x = String::extract_int(ss);
        int target_y = String::extract_int(ss);
        int overcharge = String::extract_int(ss);
        return new Action(USE_SKILL, user, orientation, nullptr, target, target_x, target_y, nullptr, object, overcharge);
      }
      case USE_ITEM:
      case ECONOMICS:
      default:
        return new Action(REST, user, NO_ORIENTATION, nullptr, nullptr, 0, 0, nullptr, "", 1);
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
      s.write(map->to_string(player, adventure));
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

  void sendTraductionPaths(Socket s, Adventure * adventure) {
    std::stringstream * ss_trads = new std::stringstream();
    for(std::string path : adventure->getDatabase()->getTraductionPaths()) {
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
