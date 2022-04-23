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

namespace Server {
  Action * receiveAction(Socket s, Character * user, Adventure * adventure) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    int keyword = stoi(msg.substr(0, msg.find('@')));
    msg = msg.substr(msg.find('@') + 1, msg.length());
    switch(keyword) {
      case MOVE: {
        int orientation = stoi(msg.substr(0, msg.find('@')));
        msg = msg.substr(msg.find('@') + 1, msg.length());
        return new Action(MOVE, user, orientation, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
      }
      case CHANGE_MAP: {
        MapLink * link = adventure->getWorld()->getMapLink(user->getX(), user->getY(), user->getCurrentMapId());
        if(link != nullptr) {
          return new Action(CHANGE_MAP, user, 0, nullptr, nullptr, nullptr, link, nullptr, nullptr);
        }
        return new Action(REST, user, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
      }
      case GRAB:
        return new Action(GRAB, user, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
      case REST:
        return new Action(REST, user, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
      default:
        return new Action(REST, user, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
  }

  Character * receiveChoices(Socket s, Adventure * adventure) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    std::string name = msg.substr(0, msg.find('@'));
    msg = msg.substr(msg.find('@') + 1, msg.length());
    Attributes * attr = nullptr;
    std::string attr_name = msg.substr(0, msg.find('@'));
    msg = msg.substr(msg.find('@') + 1, msg.length());
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
    std::string race_name = msg.substr(0, msg.find('@'));
    msg = msg.substr(msg.find('@') + 1, msg.length());
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
    std::string origin_name = msg.substr(0, msg.find('@'));
    msg = msg.substr(msg.find('@') + 1, msg.length());
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
    std::string culture_name = msg.substr(0, msg.find('@'));
    msg = msg.substr(msg.find('@') + 1, msg.length());
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
    std::string religion_name = msg.substr(0, msg.find('@'));
    msg = msg.substr(msg.find('@') + 1, msg.length());
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
    std::string profession_name = msg.substr(0, msg.find('@'));
    msg = msg.substr(msg.find('@') + 1, msg.length());
    for(Way * way : adventure->getStartingWays()) {
      if(way->name == profession_name && way->type == PROFESSION) {
        profession = way;
        break;
      }
    }
    if(profession == nullptr) {
      return nullptr;
    }
    return adventure->spawnPlayer(name, attr, race, origin, culture, religion, profession);
  }

  void sendMap(Socket s, Map * map) {
    try {
      s.write(map->to_string());
    } catch (const CloseException &e) {
      throw e;
    }
  }
  void sendStartingPossibilites(Socket s, Adventure * adventure) {
    std::string msg = "";
    std::list<Attributes *> attributes = adventure->getStartingAttributes();
    std::list<Way *> ways = adventure->getStartingWays();
    for(Attributes * attr : attributes) {
      msg += attr->to_string() + "|";
    }
    msg += "@";
    for(Way * way : ways) {
      msg += way->to_string() + "|";
    }
    msg += "@";
    try {
      s.write(msg);
    } catch (const CloseException &e) {
      throw e;
    }
  }
}
