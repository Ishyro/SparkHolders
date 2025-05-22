#include <sstream>
#include <mutex>

#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Database.h"
#include "data/World.h"

#include "data/actions/Action.h"
#include "data/actions/BaseAction.h"
#include "data/actions/OrientedAction.h"
#include "data/actions/TargetedAction.h"
#include "data/actions/GearAction.h"
#include "data/actions/SkillAction.h"

#include "data/ways/Race.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"

#include "server/Link.h"

#include "util/String.h"
#include "util/Random.h"

void Link::initialize(Socket * s) {
  this->s = s;
}

void Link::listen() {
  std::string msg;
  try {
    msg = s->read();
  }
  catch (const CloseException &e) {
    markClosed();
    throw e;
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
        sendAdventure();
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
      character = receiveChoices(ss->str());
      try {
        sendCharacter();
      }
      catch (const CloseException &e) {
        markClosed();
      }
      break;
    }
    case SOCKET_MSG_ACTION: {
      Action * action = receiveAction(ss->str());
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
      if(master) {
        std::cout << "shutdown" << std::endl;
        shuting_down = true;
      }
      else {
        std::cout << "quit" << std::endl;
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
        sendCharacter();
      }
      std::stringstream * ss = new std::stringstream();
      String::insert_int(ss, SOCKET_MSG_STATE);
      String::insert(ss, adventure->state_to_string(character));
      String::insert_bool(ss, character->getAction() == nullptr);
      s->write(ss->str());
      delete ss;
    }
    catch (const CloseException &e) {
      markClosed();
    }
  }
}

void Link::sendStart() {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, SOCKET_MSG_READY);
  try {
    s->write(ss->str());
  } catch (const CloseException &e) {
    markClosed();
  }
}

bool Link::isClosed() { return closed; }
bool Link::isShutingDown() { return master && shuting_down; }
bool Link::isReady() { return ready; }
bool Link::isMaster() { return master; }
void Link::markClosed() { closed = true; delete s; }
Character * Link::getCharacter() { return character; }
void Link::changeSocket(Socket * s) { this->s = s; closed = false; }
bool Link::isPaused() {
  return !closed && (pause || (Settings::getPauseMode() == SETTINGS_PAUSE_NO_ACTION && character->getAction() == nullptr && character->getLegAction() == nullptr));
}
bool Link::getNeedToUpdateActions() {
  const std::lock_guard<std::mutex> guard(mutex);
  return !character->isInWeakState() && character->getAction() == nullptr;
}

Action * Link::receiveAction(std::string msg) {
  Action * result;
  std::stringstream * ss = new std::stringstream(msg);
  // ignore socket_msg_type
  String::extract(ss);
  result = readAction(String::extract(ss));
  delete ss;
  return result;
}

Action * Link::readAction(std::string msg) {
  std::stringstream * ss = new std::stringstream(msg);
  int32_t type = String::extract_int(ss);
  Action * action;
  switch(type) {
    case ACTION_IDLE:
    case ACTION_REST:
    case ACTION_RUN:
    case ACTION_JUMP:
    case ACTION_BREAKPOINT:
      action = new BaseAction(type, adventure, nullptr, character);
      break;
    case ACTION_MOVE: {
      float x = String::extract_float(ss);
      float y = String::extract_float(ss);
      float z = String::extract_float(ss);
      action = new OrientedAction(type, adventure, nullptr, character, MathUtil::Vector3(x, y, z));
      break;
    }
    case ACTION_ACTIVATION: {
      std::string msg2 = String::extract(ss);
      //MathUtil::Target * target = MathUtil::target_from_string(String::extract(ss), adventure);
      MathUtil::Target * target = MathUtil::target_from_string(msg2, adventure);
      action = new TargetedAction(type, adventure, nullptr, character, target);
      break;
    }
    case ACTION_RELOAD:
    case ACTION_GRAB:
    case ACTION_USE_ITEM: {
      ItemSlot * slot = new ItemSlot();
      slot->id = String::extract_long(ss);
      slot->x = String::extract_int(ss);
      slot->y = String::extract_int(ss);
      slot->slot = String::extract_int(ss);
      action = new GearAction(type, adventure, nullptr, character, slot, nullptr);
      break;
    }
    case ACTION_SWAP_GEAR: {
      ItemSlot * slot1 = new ItemSlot();
      slot1->id = String::extract_long(ss);
      slot1->x = String::extract_int(ss);
      slot1->y = String::extract_int(ss);
      slot1->slot = String::extract_int(ss);
      ItemSlot * slot2 = new ItemSlot();
      slot2->id = String::extract_long(ss);
      slot2->x = String::extract_int(ss);
      slot2->y = String::extract_int(ss);
      slot2->slot = String::extract_int(ss);
      action = new GearAction(type, adventure, nullptr, character, slot1, slot2);
      break;
    }
    case ACTION_USE_SKILL: {
      MathUtil::Target * target = MathUtil::target_from_string(String::extract(ss), adventure);
      Skill * skill = (Skill *) adventure->getDatabase()->getSkill(String::extract(ss));
      int32_t mana_cost = String::extract_int(ss);
      action = new SkillAction(type, adventure, nullptr, character, target, skill, mana_cost);
      break;
    }
    case ACTION_ATTACK: {
      MathUtil::Target * target = MathUtil::target_from_string(String::extract(ss), adventure);
      int32_t mana_cost = String::extract_int(ss);
      action = new SkillAction(type, adventure, nullptr, character, target, nullptr, mana_cost);
      break;
    }
    case ACTION_BLOCK: {
      MathUtil::Target * target = MathUtil::target_from_string(String::extract(ss), adventure);
      int32_t mana_cost = String::extract_int(ss);
      action = new SkillAction(type, adventure, nullptr, character, target, nullptr, mana_cost);
      break;
    }
    case ACTION_TALKING: {
      action = new BaseAction(ACTION_IDLE, adventure, nullptr, character);
      break;
    }
    case ACTION_ECONOMICS: {
      action = new BaseAction(ACTION_IDLE, adventure, nullptr, character);
      break;
    }
    default:
      delete ss;
      action = new BaseAction(ACTION_IDLE, adventure, nullptr, character);
  }
  return action;
}

Character * Link::receiveChoices(std::string msg) {
  std::stringstream * ss = new std::stringstream(msg);
  // ignore socket_msg_type
  String::extract(ss);
  std::string  name = String::extract(ss);
  Attributes * attr = nullptr;
  std::string attr_name = String::extract(ss);
  for(Attributes * attr2 : adventure->getStartingAttributes()) {
    if(attr2->name == attr_name) {
      attr = attr2;
      break;
    }
  }
  if(attr == nullptr) {
    return nullptr;
  }
  Race * race = nullptr;
  std::string race_name = String::extract(ss);
  for(Way * way : adventure->getStartingWays()) {
    if(way->name == race_name && way->type == WAY_RACE) {
      race = (Race *) way;
      break;
    }
  }
  if(race == nullptr) {
    return nullptr;
  }
  Way * origin = nullptr;
  std::string origin_name = String::extract(ss);
  for(Way * way : adventure->getStartingWays()) {
    if(way->name == origin_name && way->type == WAY_ORIGIN) {
      origin = way;
      break;
    }
  }
  if(origin == nullptr) {
    return nullptr;
  }
  Way * culture = nullptr;
  std::string culture_name = String::extract(ss);
  for(Way * way : adventure->getStartingWays()) {
    if(way->name == culture_name && way->type == WAY_CULTURE) {
      culture = way;
      break;
    }
  }
  if(culture == nullptr) {
    return nullptr;
  }
  Way * religion = nullptr;
  std::string religion_name = String::extract(ss);
  for(Way * way : adventure->getStartingWays()) {
    if(way->name == religion_name && way->type == WAY_RELIGION) {
      religion = way;
      break;
    }
  }
  if(religion == nullptr) {
    return nullptr;
  }
  Way * profession = nullptr;
  std::string profession_name = String::extract(ss);
  for(Way * way : adventure->getStartingWays()) {
    if(way->name == profession_name && way->type == WAY_PROFESSION) {
      profession = way;
      break;
    }
  }
  if(profession == nullptr) {
    return nullptr;
  }
  std::list<std::pair<const std::string, const std::string>> pairs = std::list<std::pair<const std::string, const std::string>>();
  pairs.push_back(std::make_pair(race->name, origin->name));
  pairs.push_back(std::make_pair(race->name, culture->name));
  pairs.push_back(std::make_pair(race->name, religion->name));
  pairs.push_back(std::make_pair(race->name, profession->name));
  pairs.push_back(std::make_pair(origin->name, culture->name));
  pairs.push_back(std::make_pair(origin->name, religion->name));
  pairs.push_back(std::make_pair(origin->name, profession->name));
  pairs.push_back(std::make_pair(culture->name, religion->name));
  pairs.push_back(std::make_pair(culture->name, profession->name));
  pairs.push_back(std::make_pair(religion->name, profession->name));
  for(std::pair<const std::string, const std::string> pair1 : pairs) {
    for(std::pair<const std::string, const std::string> pair2 : adventure->getDatabase()->getWaysIncompatibilities()) {
      if((pair1.first == pair2.first && pair1.second == pair2.second) ||
        (pair1.first == pair2.second && pair1.second == pair2.first)) {
          return nullptr;
      }
    }
  }
  Character * character = adventure->spawnPlayer(name, attr, race, origin, culture, religion, profession);
  delete ss;
  return character;
}

void Link::sendAdventure() {
  try {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, SOCKET_MSG_ADVENTURE);
    String::insert(ss, adventure->filePath);
    String::insert_long(ss, Settings::getTickRate().count());
    String::insert_long(ss, Random::getWorldGenerationSeed());
    String::insert_bool(ss, master);
    s->write(ss->str());
    delete ss;
  }
  catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendCharacter() {
  try {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, SOCKET_MSG_CHARACTER);
    String::insert(ss, character->full_to_string(adventure));
    character->setNeedToSend(false);
    s->write(ss->str());
    delete ss;
  }
  catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendAction() {
  try {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, SOCKET_MSG_ACTION);
    s->write(ss->str());
    delete ss;
  }
  catch (const CloseException &e) {
    throw e;
  }
}
