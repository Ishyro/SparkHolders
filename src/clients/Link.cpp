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
      Character * character = Character::full_from_string(String::extract(ss), adventure);
      characters.erase(character->id);
      characters.insert(std::make_pair(character->id, character));
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

void Link::sendActions(
    std::vector<int64_t> ids,
    std::vector<std::vector<int32_t>> types,
    std::vector<std::vector<void *>> args1,
    std::vector<std::vector<void *>> args2,
    std::vector<std::vector<int32_t>> overcharge_powers,
    std::vector<std::vector<int32_t>> overcharge_durations,
    std::vector<std::vector<int32_t>> overcharge_ranges
  ) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, SOCKET_MSG_ACTION);
    for(int32_t i = 0; i < (int32_t) ids.size(); i++) {
      String::insert(ss, Client::writeActions(ids[i], types[i], args1[i], args2[i], overcharge_powers[i], overcharge_durations[i], overcharge_ranges[i]));
    }
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
    for(auto pair : characters) {
      if(pair.second->id == display->id) {
        pair.second->move(MathUtil::makeVector3(display->x, display->y, display->z), display->orientation, adventure->getWorld());
        pair.second->setHp(display->hp);
        pair.second->setMana(display->mana);
        pair.second->setShield(display->shield);
        pair.second->setStamina(display->stamina);
        pair.second->setSatiety(display->satiety);
        pair.second->setCurrentSoulBurn(display->soulBurn);
        pair.second->gainXP(display->xp - pair.second->getXP());
      }
    }
  }
  for(auto pair : characters) {
    adventure->getWorld()->changeRegion(pair.second);
    //Map * map = new Map(adventure->getWorld()->getMap(pair.second->getX(), pair.second->getY(), pair.second->getZ()), pair.second, adventure->getDatabase(), adventure->getWorld());
    //game_state->maps.erase(pair.first);
    //game_state->maps.insert(std::make_pair(pair.first, map));
  }
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

Character * Link::getPlayer(int64_t id) { return characters.at(id); }

std::list<int64_t> Link::getPlayersId() {
  std::list<int64_t> result = std::list<int64_t>();
  for(auto pair : characters) {
    result.push_back(pair.first);
  }
  return result;
}

std::list<Character *> Link::getPlayers() {
  std::list<Character *> result = std::list<Character *>();
  for(auto pair : characters) {
    result.push_back(pair.second);
  }
  return result;
}

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
