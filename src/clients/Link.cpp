
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
  Client::receiveStartingPossibilites(s, attributes, ways, adventure);
}

void Link::sendChoices(std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
  Client::sendChoices(s, name, attibutes, race, origin, culture, religion, profession, adventure);
}

MapDisplay * Link::receiveMap() { return Client::receiveMap(s, adventure); }
void Link::sendAction(int type, int orientation, ProjectileDisplay * projectile, Skill * skill, CharacterDisplay * target, Item * item, Weapon * weapon) {
  Client::sendAction(s, type, orientation, projectile, skill, target, item, weapon, adventure);
}
std::vector<Attributes *> Link::getStartingAttributes() { return *attributes; }
std::vector<Way *> Link::getStartingWays() { return *ways; }
