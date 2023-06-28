#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/actions/Action.h"
#include "data/actions/BaseAction.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"
#include "communication/Server.h"

#include "server/Link.h"

void Link::initialize() {
  try {
    Server::sendAdventure(s, adventure);
    player = Server::receiveChoices(s, adventure);
    if(player == nullptr) {
      markClosed();
    }
  } catch (const CloseException &e) {
    markClosed();
    throw e;
  }
}

void Link::sendState() {
  if(!isClosed()) {
    try {
      Server::sendState(s, player, adventure);
    } catch (const CloseException &e) {
      markClosed();
    }
  }
}

Action * Link::receiveAction() {
  if(!isClosed()) {
    try {
      return Server::receiveAction(s, player, adventure);
    } catch (const CloseException &e) {
      markClosed();
    }
  }
  return new BaseAction(ACTION_IDLE, adventure, nullptr, player);
}

bool Link::isClosed() { return closed; }
void Link::markClosed() { closed = true; }
Character * Link::getPlayer() { return player; }
void Link::changeSocket(Socket s) { this->s = s; closed = false; }
