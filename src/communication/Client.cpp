#include "data/Adventure.h"
#include "data/Action.h"
#include "data/Map.h"

#include "communication/Socket.h"

#include "communication/Client.h"

namespace Client {
  MapDisplay * receive(Socket s, Adventure * adventure) {
    std::string msg = s.read();
    if (msg != "") {
      return Map::from_string(msg);
    } else {
      return nullptr;
    }
  }

  void send(Socket s, Action * action, Adventure * adventure) {

  }
}
