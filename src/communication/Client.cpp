#include <sstream>

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Action.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/Way.h"

#include "communication/Socket.h"
#include "communication/Client.h"

#include "utils/String.h"

namespace Client {

  void receiveStartingPossibilites(Socket s, std::vector<Attributes *> * attributes, std::vector<Way *> * ways) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    if(msg.find("RECONNECT") != std::string::npos) {
      throw CloseException();
    }
    std::stringstream * ss = new std::stringstream(msg);
    std::stringstream * ss_attributes = new std::stringstream(String::extract(ss));
    while(ss_attributes->rdbuf()->in_avail() != 0) {
      attributes->push_back(Attributes::from_string(String::extract(ss_attributes)));
    }
    delete ss_attributes;
    std::stringstream * ss_ways = new std::stringstream(String::extract(ss));
    while(ss_ways->rdbuf()->in_avail() != 0) {
      ways->push_back(Way::from_string(String::extract(ss_ways)));
    }
    delete ss_ways;
    delete ss;
  }

  std::list<std::string> receiveTraductionPaths(Socket s) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    std::list<std::string> result = std::list<std::string>();
    std::stringstream * ss_trads = new std::stringstream(msg);
    while(ss_trads->rdbuf()->in_avail() != 0) {
      result.push_back(String::extract(ss_trads));
    }
    delete ss_trads;
    return result;
  }

  void receiveWaysIncompabilities(Socket s, std::list<std::pair<const std::string, const std::string>> * waysIncompatibilities) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    std::stringstream * ss_ways = new std::stringstream(msg);
    while(ss_ways->rdbuf()->in_avail() != 0) {
      std::string way1 = String::extract(ss_ways);
      std::string way2 = String::extract(ss_ways);
      waysIncompatibilities->push_back(std::make_pair(way1, way2));
    }
    delete ss_ways;
  }

  MapDisplay * receiveMap(Socket s, Character ** player, long * id) {
    try {
      std::string msg = s.read();
      std::stringstream * ss = new std::stringstream(msg);
      MapDisplay * result = nullptr;
      if(msg.at(0) != '{') {
        *ss >> *id;
        (*player)->deepDelete();
        delete *player;
        *player = Character::full_from_string(String::extract(ss));
        result = Map::from_string(String::extract(ss));
      } else {
        result = Map::from_string(msg);
      }
      delete ss;
      return result;
    } catch (const CloseException &e) {
      throw e;
    }
  }

  void sendAction(Socket s, int type, int orientation, Skill * skill, int target_id, int target_x, int target_y, std::string object, int overcharge_power, int overcharge_duration, int overcharge_range) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, type);
    switch(type) {
      case MOVE:
        String::insert_int(ss, orientation);
        break;
      case REST:
        break;
      case SHOOT:
        String::insert_int(ss, orientation);
        String::insert_int(ss, target_id);
        String::insert_int(ss, target_x);
        String::insert_int(ss, target_y);
        break;
      case FORCE_STRIKE:
        String::insert_int(ss, orientation);
        String::insert_int(ss, target_id);
        String::insert_int(ss, target_x);
        String::insert_int(ss, target_y);
        break;
      case RELOAD:
        String::insert(ss, object);
        break;
      case SWAP_GEAR:
        String::insert(ss, object);
        break;
      case CHANGE_MAP:
        break;
      case GRAB:
        break;
      case USE_SKILL:
        String::insert(ss, object);
        String::insert_int(ss, orientation);
        String::insert_int(ss, target_id);
        String::insert_int(ss, target_x);
        String::insert_int(ss, target_y);
        String::insert_int(ss, overcharge_power);
        String::insert_int(ss, overcharge_duration);
        String::insert_int(ss, overcharge_range);
        break;
      case USE_ITEM:
        String::insert(ss, object);
        break;
      case TALKING:
        String::insert(ss, object);
        String::insert_int(ss, target_id);
        break;
      case ECONOMICS:
        String::insert(ss, object);
        String::insert_int(ss, orientation);
        String::insert_int(ss, target_id);
        break;
      default:
        ;
    }
    try {
      s.write(ss->str());
    } catch (const CloseException &e) {
      throw e;
    }
    delete ss;
  }

  Character * sendChoices(Socket s, std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
    std::stringstream * ss = new std::stringstream();
    String::insert(ss, name);
    String::insert(ss, attributes);
    String::insert(ss, race);
    String::insert(ss, origin);
    String::insert(ss, culture);
    String::insert(ss, religion);
    String::insert(ss, profession);
    try {
      s.write(ss->str());
      delete ss;
      return Character::full_from_string(s.read());
    } catch (const CloseException &e) {
      throw e;
    }
  }
}
