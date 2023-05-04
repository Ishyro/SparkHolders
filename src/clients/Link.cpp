#include "data/actions/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "communication/Socket.h"

#include "clients/Link.h"

#include "communication/Client.h"

void Link::loadChoices() {
  attributes = new std::vector<Attributes *>();
  ways = new std::vector<Way *>();
  waysIncompatibilities = new std::list<std::pair<const std::string, const std::string>>();
  try {
    Client::receiveWaysIncompabilities(s, waysIncompatibilities);
    Client::receiveStartingPossibilites(s, attributes, ways);
  } catch (const CloseException &e) {
    throw e;
  }
}

std::list<std::string> Link::receiveTranslationPaths() {
  try {
    return Client::receiveTranslationPaths(s);
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendChoices(std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
  try {
    player = Client::sendChoices(s, name, attributes, race, origin, culture, religion, profession);
    serverCharacterId = 0;
  } catch (const CloseException &e) {
    throw e;
  }
}

MapDisplay * Link::receiveMap() {
  try {
    MapDisplay * map = Client::receiveMap(s, &player, &serverCharacterId, &need_to_update_actions);
    for(CharacterDisplay * display : map->characters) {
      if(serverCharacterId == display->id) {
        player->move(display->y + (float) map->offsetY, display->x + (float) map->offsetX, display->orientation, map->id);
        player->setHp(display->hp);
        player->setMana(display->mana);
        player->setStamina(display->stamina);
        player->setSatiety(display->satiety);
        player->setCurrentSoulBurn(display->soulBurn);
        player->gainXP(display->xp - player->getXP());
      }
    }
    return map;
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendAction(int type, void * arg1 = nullptr, void * arg2 = nullptr, int overcharge_power = 1, int overcharge_duration = 1, int overcharge_range = 1) {
  try {
    switch(type) {
      case IDLE:
      case RESPITE:
      case REST:
      case BREAKPOINT:
        Client::sendBaseAction(s, type);
        break;
      case MOVE:
      case STRIKE:
      case HEAVY_STRIKE:
      case SHOOT:
        Client::sendTargetedAction(s, type, (Target *) arg1);
        break;
      case RELOAD:
      case SWAP_GEAR:
      case GRAB:
      case USE_ITEM:
        Client::sendGearAction(s, type, (GearPiece *) arg1);
        break;
      case USE_SKILL:
        Client::sendSkillAction(s, type, (Target *) arg1, (Skill *) arg2, overcharge_power, overcharge_duration, overcharge_range);
        break;
      case TALKING:
      case ECONOMICS:
        // TODO
        break;
      default: ;
    }
  } catch (const CloseException &e) {
    throw e;
  }
}

std::vector<Attributes *> Link::getStartingAttributes() { return *attributes; }
std::vector<Way *> Link::getStartingWays() { return *ways; }
std::list<std::pair<const std::string, const std::string>> Link::getWaysIncompatibilities() { return *waysIncompatibilities; }

Character * Link::getPlayer() { return player; }

bool Link::getNeedToUpdateActions() { return need_to_update_actions; }
