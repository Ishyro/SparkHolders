#include <string>

#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Action.h"
#include "data/Map.h"
#include "data/Tile.h"
#include "data/World.h"

#include "communication/Socket.h"

#include "communication/Server.h"

namespace Server {
  Action * receive(Socket s, Adventure * adventure) {
    std::string msg = s.read();
    int keyword = stoi(msg.substr(0, msg.find('@')));
    msg = msg.substr(msg.find('@') + 1, msg.length());
    switch(keyword) {
      case MOVE: {
        int orientation = stoi(msg.substr(0, msg.find('@')));
        msg = msg.substr(msg.find('@') + 1, msg.length());
        return new Action(MOVE, s.getUser(), orientation, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        break;
      }
      case CHANGE_MAP: {
        MapLink * link = adventure->getWorld()->getMapLink(s.getUser()->getX(), s.getUser()->getY(), s.getUser()->getCurrentMapId());
        if(link != nullptr);
        return new Action(CHANGE_MAP, s.getUser(), 0, nullptr, nullptr, nullptr, link, nullptr, nullptr);
        break;
      }
      case GRAB:
        return new Action(GRAB, s.getUser(), 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        break;
      case REST:
      default:
        return new Action(REST, s.getUser(), 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }
    return nullptr;
  }

  void send(Socket s, Map * map, Adventure * adventure) {
    s.write(map->to_string());
  }
}
