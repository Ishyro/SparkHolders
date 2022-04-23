
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
  try {
    Client::receiveStartingPossibilites(s, attributes, ways, adventure);
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendChoices(std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
  try {
    Client::sendChoices(s, name, attibutes, race, origin, culture, religion, profession, adventure);
  } catch (const CloseException &e) {
    throw e;
  }
}

MapDisplay * Link::receiveMap() {
  try {
    return Client::receiveMap(s, adventure);
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
