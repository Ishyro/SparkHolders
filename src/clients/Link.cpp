#include "data/actions/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Database.h"
#include "data/World.h"

#include "communication/Socket.h"

#include "clients/Link.h"

#include "communication/Client.h"

#include "util/String.h"

void Link::initialize(std::string password) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_CONNECT);
  String::insert(ss, password);
  try {
    s.write(ss->str());
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::listen() {
  std::string msg;
  try {
    msg = s.read();
  }
  catch (const CloseException &e) {
    closed = true;
  }
  std::stringstream * ss = new std::stringstream(msg);
  int32_t socket_msg_type = String::extract_int(ss);
  switch(socket_msg_type) {
    case SOCKET_MSG_ADVENTURE:
      adventure = Client::receiveAdventure(ss->str(), tickrate, master);
      key_holder = new EnglishKeyHolder(adventure->getDatabase()->getKeysPaths());
      started = true;
      adventure->getWorld()->generateWorld();
      break;
    case SOCKET_MSG_READY:
      ready = true;
      break;
    case SOCKET_MSG_STATE:
      receiveState(ss->str());
      break;
    case SOCKET_MSG_CHARACTER: {
      Character * new_character = Character::full_from_string(String::extract(ss), adventure);
      if(character != nullptr) {
        delete character;
      }
      character = new_character;
      break;
    }
    // client shouldn't receive these
    case SOCKET_MSG_ACTION:
      break;
    case SOCKET_MSG_CHOICE:
      break;
    default:
      ;
  }
}

void Link::sendChoices(std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
  try {
    Client::sendChoices(s, adventure, name, attributes, race, origin, culture, religion, profession);
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendAction(int32_t type, void * arg1, void * arg2, int32_t mana_cost) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, SOCKET_MSG_ACTION);
    String::insert(ss, Client::writeAction(type, arg1, arg2, mana_cost));
    try {
      s.write(ss->str());
      delete ss;
    } catch (const CloseException &e) {
      throw e;
    }
  }

void Link::receiveState(std::string msg) {
  StateDisplay * game_state = Client::receiveState(msg, adventure);
  for(CharacterDisplay * display : game_state->characters) {
    if(character->id == display->id) {
      character->move(MathUtil::Vector3(display->x, display->y, display->z), MathUtil::Vector3(display->orientation_x, 0.F, display->orientation_z), adventure->getWorld());
      character->setHp(display->hp);
      character->setMana(display->mana);
      character->setShield(display->shield);
      character->setHunger(display->hunger);
      character->setThirst(display->thirst);
      character->setStamina(display->stamina);
      character->setSanity(display->sanity);
      character->setCurrentSoulBurn(display->soulBurn);
      character->gainXP(display->xp - character->getXP());
      break;
    }
  }
  // only usefull for the first state received
  adventure->getWorld()->changeRegion(character);
  state_pile.push_back(game_state);
}

void Link::sendReady() {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_READY);
  try {
    s.write(ss->str());
    delete ss;
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendPause() {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_PAUSE);
  try {
    s.write(ss->str());
    delete ss;
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendUnpause() {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_UNPAUSE);
  try {
    s.write(ss->str());
    delete ss;
  } catch (const CloseException &e) {
    throw e;
  }
}

int64_t Link::getTickRate() { return tickrate; }

std::vector<Attributes *> Link::getStartingAttributes() {
  std::list<Attributes *> l = adventure->getStartingAttributes();
  std::vector<Attributes *> v{ std::begin(l), std::end(l) };
  return v;
}

std::vector<Way *> Link::getStartingWays() {
  std::list<Way *> l = adventure->getStartingWays();
  std::vector<Way *> v{ std::begin(l), std::end(l) };
  return v;
}
std::list<std::pair<const std::string, const std::string>> Link::getWaysIncompatibilities() { return adventure->getDatabase()->getWaysIncompatibilities(); }

Character * Link::getPlayer() { return character; }

Adventure * Link::getAdventure() { return adventure; }
bool Link::hasState() { return closed || !state_pile.empty(); }

StateDisplay * Link::getState() {
  while(!hasState()) {
    usleep(1);
  }
  if(!closed) {
    StateDisplay * result = state_pile.front();
    state_pile.pop_front();
    return result;
  }
  return nullptr;
}

std::string Link::getEnglishFromKey(std::string key) { return key_holder->getEnglishFromKey(key); }

bool Link::isClosed() { return closed; }
bool Link::isReady() { return ready; }
bool Link::isStarted() { return started; }
void Link::close(bool shutdown) {
  closed = true;
  std::stringstream * ss = new std::stringstream();
  if(shutdown && master) {
    String::insert_int(ss, SOCKET_MSG_SHUTDOWN);
  }
  else {
    String::insert_int(ss, SOCKET_MSG_QUIT);
  }
  try {
    s.write(ss->str());
    s.close();
    delete ss;
  } catch (const CloseException &e) {
    throw e;
  }
}
