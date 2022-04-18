#include <sstream>

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Action.h"
#include "data/Map.h"
#include "data/Way.h"

#include "communication/Socket.h"

#include "communication/Client.h"

namespace Client {

  void receiveStartingPossibilites(Socket s, std::list<Attributes *> * attributes, std::list<Way *> * ways,  Adventure * adventure) {
    std::string msg = s.read();
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

  MapDisplay * receiveMap(Socket s, Adventure * adventure) {
    std::string msg = s.read();
    if (msg != "") {
      return Map::from_string(msg);
    } else {
      return nullptr;
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
    s.write(msg);
  }

  void sendChoices(Socket s, std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession, Adventure * adventure) {
    s.write(name + "@" + attibutes + "@" + race + "@" + origin + "@" + culture + "@" + religion + "@" + profession + "@");
  }
}
