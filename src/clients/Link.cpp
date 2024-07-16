#include "data/actions/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "communication/Socket.h"

#include "clients/Link.h"

#include "communication/Client.h"

#include "util/String.h"

void Link::initialize(std::string username, std::string password) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_CONNECT);
  String::insert(ss, username);
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
  } catch (const CloseException &e) {
    closed = true;
  }
  std::stringstream * ss = new std::stringstream(msg);
  int32_t socket_msg_type = String::extract_int(ss);
  switch(socket_msg_type) {
    case SOCKET_MSG_ADVENTURE:
      adventure = Client::receiveAdventure(ss->str(), master);
      t = new Translator(adventure->getDatabase()->getTranslationPaths(), language);
      started = true;
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
    case SOCKET_MSG_SWITCH:
      break;
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

void Link::sendAction(int32_t type, void * arg1, void * arg2, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, SOCKET_MSG_ACTION);
    String::insert(ss, Client::writeAction(type, arg1, arg2, overcharge_power, overcharge_duration, overcharge_range));
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
      character->move(MathUtil::makeVector3(display->x, display->y, display->z), display->orientation, adventure->getWorld());
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

Translator * Link::getTranslator() { return t; }

void Link::changeLanguage(std::string language) {
  this->language = language;
  Translator * old = t;
  delete old;
  t = new Translator(adventure->getDatabase()->getTranslationPaths(), language);
}

bool Link::isClosed() { return closed; }
bool Link::isReady() { return ready; }
bool Link::isStarted() { return started; }
void Link::markClosed() { closed = true; }
