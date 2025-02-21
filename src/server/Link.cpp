#include <sstream>
#include <mutex>

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
  int32_t socket_msg_type = String::extract_int(ss);
  switch(socket_msg_type) {
    case SOCKET_MSG_CONNECT:
      password = String::extract(ss);
      if(password == Settings::getMasterPassword()) {
        master = true;
      }
      try {
        Server::sendAdventure(s, adventure, master);
      }
      catch (const CloseException &e) {
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
      if(character != nullptr) {
        delete character;
      }
      character = Server::receiveChoices(ss->str(), adventure);
      try {
        Server::sendCharacter(s, character, adventure);
      } catch (const CloseException &e) {
        markClosed();
      }
      break;
    }
    case SOCKET_MSG_ACTION: {
      Action * action = Server::receiveAction(ss->str(), character, adventure);
      action->computeTime(adventure);
      const std::lock_guard<std::mutex> guard(mutex);
      // select between standard and leg action
      if(action->type == ACTION_MOVE || action->type == ACTION_JUMP || action->type == ACTION_RUN) {
        Action * tmp = character->getLegAction();
        if(tmp == nullptr) {
          character->setLegAction(action);
        }
        else {
          while(tmp->getNext() != nullptr) {
            tmp = tmp->getNext();
          }
          tmp->setNext(action);
          action->setPrevious(tmp);
        }
      }
      else {
        Action * tmp = character->getAction();
        if(tmp == nullptr) {
          character->setAction(action);
        }
        else {
          while(tmp->getNext() != nullptr) {
            tmp = tmp->getNext();
          }
          tmp->setNext(action);
          action->setPrevious(tmp);
        }
      }
      break;
    }
    case SOCKET_MSG_PAUSE:
      // ignore SETTINGS_PAUSE_NO_ACTION because it's already paused if characer is not moving
      if(Settings::getPauseMode() == SETTINGS_PAUSE_ALL || (master && Settings::getPauseMode() == SETTINGS_PAUSE_MASTER) ) {
        pause = true;
      }
      break;
    case SOCKET_MSG_UNPAUSE:
      pause = false;
      break;
    case SOCKET_MSG_QUIT:
      std::cout << "quit" << std::endl;
      markClosed();
      break;
    case SOCKET_MSG_SHUTDOWN:
        std::cout << "shutdown" << std::endl;
      if(master) {
        std::cout << "master shutdown" << std::endl;
        shuting_down = true;
      }
      markClosed();
      break;
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
      if(character->needToSend()) {
        Server::sendCharacter(s, character, adventure);
      }
      Server::sendState(s, character, adventure);
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
bool Link::isShutingDown() { return master && shuting_down; }
bool Link::isReady() { return ready; }
bool Link::isMaster() { return master; }
void Link::markClosed() { closed = true; }
Character * Link::getCharacter() { return character; }
void Link::changeSocket(Socket s) { this->s = s; closed = false; }
bool Link::isPaused() {
  return !closed && (pause || (Settings::getPauseMode() == SETTINGS_PAUSE_NO_ACTION && character->getAction() == nullptr && character->getLegAction() == nullptr));
}
bool Link::getNeedToUpdateActions() {
  const std::lock_guard<std::mutex> guard(mutex);
  return !character->isInWeakState() && character->getAction() == nullptr;
}
