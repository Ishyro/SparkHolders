#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "communication/Socket.h"
#include "communication/Client.h"

#include "clients/Link.h"

void Link::loadChoices() {
  attributes = new std::vector<Attributes *>();
  ways = new std::vector<Way *>();
  waysIncompatibilities = new std::list<std::pair<const std::string, const std::string>>();
  try {
    Client::receiveWaysIncompabilities(s, waysIncompatibilities);
    Client::receiveStartingPossibilites(s, attributes, ways, adventure);
  } catch (const CloseException &e) {
    throw e;
  }
}

std::list<std::string> Link::receiveTraductionPaths() {
  try {
    return Client::receiveTraductionPaths(s);
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendChoices(std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
  try {
    player = Client::sendChoices(s, name, attibutes, race, origin, culture, religion, profession, adventure);
  } catch (const CloseException &e) {
    throw e;
  }
}

MapDisplay * Link::receiveMap() {
  try {
    MapDisplay * map = Client::receiveMap(s, adventure);
    for(CharacterDisplay * display : map->characters) {
      if(display->id == player->id) {
        player->move(display->y + map->offsetY, display->x + map->offsetX, display->orientation);
        player->setHp(display->hp);
        player->setMana(display->mana);
        player->setCurrentSoulBurn(display->soulBurn);
      }
    }
    return map;
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendAction(int type, int orientation, ProjectileDisplay * projectile, Skill * skill, CharacterDisplay * target, Item * item, Weapon * weapon) {
  try {
    Client::sendAction(s, type, orientation, projectile, skill, target, item, weapon, adventure);
  } catch (const CloseException &e) {
    throw e;
  }
}

std::vector<Attributes *> Link::getStartingAttributes() { return *attributes; }
std::vector<Way *> Link::getStartingWays() { return *ways; }
std::list<std::pair<const std::string, const std::string>> Link::getWaysIncompatibilities() { return *waysIncompatibilities; }

Character * Link::getPlayer() { return player; }
