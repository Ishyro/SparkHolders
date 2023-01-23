
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
  try {
    Server::sendWaysIncompabilities(s, adventure);
    Server::sendStartingPossibilites(s, adventure);
    Server::sendTranslationPaths(s, adventure);
    player = Server::receiveChoices(s, adventure);
  } catch (const CloseException &e) {
    markClosed();
    throw e;
  }
}

void Link::sendMap() {
  if(!isClosed() && ! lastStateSend) {
    Map * map = new Map(adventure->getWorld()->getMap(player->getCurrentMapId()), player, adventure->getDatabase());
    try {
      Server::sendMap(s, map, player, adventure);
    } catch (const CloseException &e) {
      markClosed();
    }
    delete map;
    lastStateSend = true;
  }
}

Action * Link::receiveAction() {
  if(!isClosed() && lastStateSend) {
    try {
      lastStateSend = false;
      return Server::receiveAction(s, player, adventure);
    } catch (const CloseException &e) {
      markClosed();
    }
  }
  return new Action(REST, player, 0, nullptr, nullptr, 0, 0, nullptr, "", 1, 1, 1);
}

bool Link::isClosed() { return closed; }
void Link::markClosed() { closed = true; }
Character * Link::getPlayer() { return player; }
void Link::changeSocket(Socket s) { this->s = s; closed = false; }
