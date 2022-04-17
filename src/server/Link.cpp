
#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"
#include "communication/Server.h"

#include "server/Link.h"

void Link::playerChoices() {
  Server::sendStartingPossibilites(s, adventure);
  player = Server::receiveChoices(s, adventure);
}

void Link::sendMap() {
  Map * map = new Map(adventure->getWorld()->getMap(player->getCurrentMapId()), player, adventure->getDatabase());
  Server::sendMap(s, map);
  delete map;
}

Action * Link::receiveAction() { return Server::receiveAction(s, player, adventure); }
