
#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"
#include "communication/Server.h"

#include "server/Link.h"

#include <iostream>

void Link::playerChoices() {
  Server::sendStartingPossibilites(s, adventure);
  try {
    player = Server::receiveChoices(s, adventure);
  } catch (CloseException &e) {
    throw e;
  }
}

void Link::sendMap() {
  Map * map = new Map(adventure->getWorld()->getMap(player->getCurrentMapId()), player, adventure->getDatabase());
  Server::sendMap(s, map);
  delete map;
}

Action * Link::receiveAction() {
  if(!isClosed()) {
    try {
      return Server::receiveAction(s, player, adventure);
    } catch (const CloseException &e) {
      markClosed();
    }
  }
  return new Action(REST, player, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

bool Link::isClosed() { return closed; }
void Link::markClosed() { closed = true; }
Character * Link::getPlayer() { return player; }
