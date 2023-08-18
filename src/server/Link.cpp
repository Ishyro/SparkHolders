#include <sstream>

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

#include "util/String.h"

void Link::initialize(Socket s) {
  this->s = s;
}

void Link::listen() {
  std::string msg;
  try {
    msg = s.read();
  } catch (const CloseException &e) {
    markClosed();
  }
  std::stringstream * ss = new std::stringstream(msg);
  int socket_msg_type = String::extract_int(ss);
  switch(socket_msg_type) {
    case SOCKET_MSG_CONNECT:
      username = String::extract(ss);
      password = String::extract(ss);
      if(password == Settings::getMasterPassword()) {
        master = true;
      }
      try {
        Server::sendAdventure(s, adventure, master);
      } catch (const CloseException &e) {
        markClosed();
      }
      break;
    case SOCKET_MSG_RECONNECT:
      // TODO
      break;
    case SOCKET_MSG_READY:
      ready = true;
      break;
    case SOCKET_MSG_CHOICE: {
      Character * character = Server::receiveChoices(ss->str(), adventure);
      characters.insert(std::make_pair(character->id, character));
      try {
        Server::sendCharacter(s, character, adventure);
      } catch (const CloseException &e) {
        markClosed();
      }
      break;
    }
    case SOCKET_MSG_ACTION:
      actions = Server::receiveActions(ss->str(), characters, adventure);
      break;
    case SOCKET_MSG_SWITCH: {
      long id = String::extract_long(ss);
      if(characters.count(id) != 0) {
        std::string other = String::extract(ss);
      }
      break;
    }
    // server shouldn't receive these
    case SOCKET_MSG_STATE:
      break;
    case SOCKET_MSG_CHARACTER:
      break;
    default:
      markClosed();
  }
}

void Link::sendState() {
  if(!isClosed()) {
    try {
      bool need_to_send = false;
      for(auto pair : characters) {
        if(pair.second->needToSend()) {
          Server::sendCharacter(s, pair.second, adventure);
        }
        need_to_send |= pair.second->getNeedToUpdateActions();
      }
      Server::sendState(s, characters, need_to_send, adventure);
    } catch (const CloseException &e) {
      markClosed();
    }
  }
}

void Link::sendStart() {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_READY);
  try {
    s.write(ss->str());
  } catch (const CloseException &e) {
    markClosed();
  }
}

bool Link::isClosed() { return closed; }
bool Link::isReady() { return ready; }
bool Link::isMaster() { return master; }
void Link::markClosed() { closed = true; }
Character * Link::getCharacter(long id) { return characters.at(id); }
void Link::changeSocket(Socket s) { this->s = s; closed = false; }
bool Link::hasActions() { return !actions.empty(); }

std::list<Action *> Link::getActions() {
  std::list<Action *> result = std::list<Action *>(actions);
  actions.clear();
  return result;
}

bool Link::getNeedToUpdateActions() {
  bool result = false;
  for(auto pair : characters) {
    result |= pair.second->getNeedToUpdateActions();
  }
  return result;
}

std::string Link::getUsername() { return username; }
