#include "data/actions/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Database.h"
#include "data/World.h"

#include "data/items/Gear.h"

#include "data/skills/Skill.h"

#include "communication/Socket.h"

#include "clients/Link.h"

#include "util/FileOpener.h"
#include "util/Random.h"
#include "util/String.h"

#include "util/Logger.h"
#ifdef LOG
  #include <regex>
#endif

void Link::initialize(std::string password) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_CONNECT);
  String::insert(ss, password);
  try {
    s->write(ss->str());
  }
  catch (const CloseException &e) {
    close(true);
    throw e;
  }
}

void Link::listen() {
  std::string msg;
  try {
    msg = s->read();
  }
  catch (const CloseException &e) {
    close(true);
  }
  std::stringstream * ss = new std::stringstream(msg);
  int32_t socket_msg_type = String::extract_int(ss);
  switch(socket_msg_type) {
    case SOCKET_MSG_ADVENTURE:
      receiveAdventure(ss->str());
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

void Link::sendAction(int32_t type, void * arg1, void * arg2, int32_t mana_cost) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, SOCKET_MSG_ACTION);
    String::insert(ss, writeAction(type, arg1, arg2, mana_cost));
    try {
      s->write(ss->str());
      delete ss;
    }
    catch (const CloseException &e) {
      close(true);
    }
  }

void Link::receiveState(std::string msg) {
  std::stringstream * ss = new std::stringstream(msg);
  // ignore socket_msg_type
  String::extract(ss);
  StateDisplay * game_state = adventure->update_state(String::extract(ss));
  game_state->need_to_send_actions = String::extract_bool(ss);
  delete ss;
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
    s->write(ss->str());
    delete ss;
  }
  catch (const CloseException &e) {
    close(true);
  }
}

void Link::sendPause() {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_PAUSE);
  try {
    s->write(ss->str());
    delete ss;
  }
  catch (const CloseException &e) {
    close(true);
  }
}

void Link::sendUnpause() {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_UNPAUSE);
  try {
    s->write(ss->str());
    delete ss;
  }
  catch (const CloseException &e) {
    close(true);
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
  if(s->isOpen()) {
    std::stringstream * ss = new std::stringstream();
    if(shutdown && master) {
      String::insert_int(ss, SOCKET_MSG_SHUTDOWN);
    }
    else {
      String::insert_int(ss, SOCKET_MSG_QUIT);
    }
    try {
      s->write(ss->str());
    }
    catch (const CloseException &e) {}
    try {
      s->close();
    }
    catch (const CloseException &e) {}
    delete ss;
  }
  #ifdef _WIN32_WINNT
    WSACleanup();
  #endif
}

void Link::receiveAdventure(std::string msg) {
  std::stringstream * ss = new std::stringstream(msg);
  // ignore socket_msg_type
  String::extract(ss);
  #ifdef LOG
    std::string adventure_name = String::extract(ss);
    LOGGER_TRACE("receive adventure: " + adventure_name);
    adventure = FileOpener::AdventureOpener(adventure_name, false);
  #else
    adventure = FileOpener::AdventureOpener(String::extract(ss), false);
  #endif
  tickrate = String::extract_long(ss);
  #ifdef LOG
    LOGGER_TRACE("tickrate: " + tickrate);
  #endif
  Random::setWorldGenerationSeed(String::extract_long(ss));
  master = String::extract_bool(ss);
}

std::string Link::writeAction(int32_t type, void * arg1, void * arg2, int32_t mana_cost) {
  switch(type) {
    case ACTION_IDLE:
    case ACTION_REST:
    case ACTION_RUN:
    case ACTION_JUMP:
    case ACTION_BREAKPOINT:
      return writeBaseAction(type);
      break;
    case ACTION_MOVE:
      return writeOrientedAction(type, *((MathUtil::Vector3 *) arg1));
      break;
    case ACTION_ACTIVATION:
      return writeTargetedAction(type, (MathUtil::Target *) arg1);
      break;
    case ACTION_RELOAD:
    case ACTION_SWAP_GEAR:
    case ACTION_GRAB:
    case ACTION_USE_ITEM:
      return writeGearAction(type, (ItemSlot *) arg1, (ItemSlot *) arg2);
      break;
    case ACTION_USE_SKILL:
    case ACTION_ATTACK:
    case ACTION_BLOCK:
      return writeSkillAction(type, (MathUtil::Target *) arg1, (Skill *) arg2, mana_cost);
      break;
    case ACTION_TALKING:
    case ACTION_ECONOMICS:
      // TODO
      break;
    default: ;
  }
  return "";
}
  
std::string Link::writeBaseAction(int32_t type) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, type);
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Link::writeGearAction(int32_t type, ItemSlot * slot1, ItemSlot * slot2) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, type);
  String::insert_long(ss, slot1->id);
  String::insert_int(ss, slot1->x);
  String::insert_int(ss, slot1->y);
  String::insert_int(ss, slot1->slot);
  if(slot2 != nullptr) {
    String::insert_long(ss, slot2->id);
    String::insert_int(ss, slot2->x);
    String::insert_int(ss, slot2->y);
    String::insert_int(ss, slot2->slot);
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Link::writeOrientedAction(int32_t type, MathUtil::Vector3 orientation) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, type);
  String::insert_float(ss, orientation.x);
  String::insert_float(ss, orientation.y);
  String::insert_float(ss, orientation.z);
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Link::writeTargetedAction(int32_t type, MathUtil::Target * target) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, type);
  String::insert(ss, MathUtil::target_to_string(target));
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Link::writeSkillAction(int32_t type, MathUtil::Target * target, Skill * skill, int32_t mana_cost) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, type);
  String::insert(ss, MathUtil::target_to_string(target));
  if(type == ACTION_USE_SKILL) {
    String::insert(ss, skill->name);
  }
  String::insert_int(ss, mana_cost);
  std::string result = ss->str();
  delete ss;
  return result;
}

void Link::sendChoices(std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_CHOICE);
  String::insert(ss, name);
  String::insert(ss, attributes);
  String::insert(ss, race);
  String::insert(ss, origin);
  String::insert(ss, culture);
  String::insert(ss, religion);
  String::insert(ss, profession);
  try {
    s->write(ss->str());
    delete ss;
  }
  catch (const CloseException &e) {
    close(true);
  }
}
