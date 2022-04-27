#include <sstream>

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Action.h"
#include "data/Map.h"
#include "data/Way.h"

#include "communication/Socket.h"

#include "communication/Client.h"

namespace Client {

  void receiveStartingPossibilites(Socket s, std::vector<Attributes *> * attributes, std::vector<Way *> * ways,  Adventure * adventure) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    if(msg.find("RECONNECT") != std::string::npos) {
      throw CloseException();
    }
    std::istringstream attrs(msg.substr(0, msg.find('@')));
    std::string attr;
    while(getline(attrs, attr, '|') && attr != "") {
      attributes->push_back(Attributes::from_string(attr));
    }
    msg = msg.substr(msg.find('@') + 1, msg.length());
    std::istringstream ways2(msg.substr(0, msg.find('@')));
    std::string way;
    while(getline(ways2, way, '|') && way != "") {
      ways->push_back(Way::from_string(way));
    }
    msg = msg.substr(msg.find('@') + 1, msg.length());
  }

  std::list<std::string> receiveTraductionPaths(Socket s) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    std::list<std::string> result = std::list<std::string>();
    std::istringstream paths(msg);
    std::string path;
    while(getline(paths, path, '@') && path != "") {
      result.push_back(path);
    }
    return result;
  }

  void receiveWaysIncompabilities(Socket s, std::list<std::pair<const std::string, const std::string>> * waysIncompatibilities) {
    std::string msg;
    try {
      msg = s.read();
    } catch (const CloseException &e) {
      throw e;
    }
    std::istringstream ways(msg);
    std::string _2ways;
    while(getline(ways, _2ways, '@') && _2ways != "") {
      std::string way1 = _2ways.substr(0, _2ways.find('|'));
      _2ways = _2ways.substr(_2ways.find('|') + 1, _2ways.length());
      std::string way2 = _2ways.substr(0, _2ways.find('|'));
      _2ways = _2ways.substr(_2ways.find('|') + 1, _2ways.length());
      waysIncompatibilities->push_back(std::make_pair(way1, way2));
    }
  }

  MapDisplay * receiveMap(Socket s, Adventure * adventure) {
    try {
      return Map::from_string(s.read());
    } catch (const CloseException &e) {
      throw e;
    }
  }

  void sendAction(Socket s, int type, int orientation, ProjectileDisplay * projectile, Skill * skill, CharacterDisplay * target, Item * item, Weapon * weapon, Adventure * adventure) {
    std::string msg = std::to_string(type) + "@";
    switch(type) {
      case MOVE:
        msg += std::to_string(orientation) + "@";
        break;
      case CHANGE_MAP:
        break;
      case GRAB:
        break;
      case REST:
        break;
      default:
        ;
    }
    try {
      s.write(msg);
    } catch (const CloseException &e) {
      throw e;
    }
  }

  void sendChoices(Socket s, std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession, Adventure * adventure) {
    try {
      s.write(name + "@" + attibutes + "@" + race + "@" + origin + "@" + culture + "@" + religion + "@" + profession + "@");
    } catch (const CloseException &e) {
      throw e;
    }
  }
}
