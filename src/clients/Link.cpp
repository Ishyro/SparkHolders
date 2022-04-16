
#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "communication/Socket.h"
#include "communication/Client.h"

#include "clients/Link.h"

void Link::loadChoices() {
  attributes = new std::list<Attributes *>();
  ways = new std::list<Way *>();
  Client::receiveStartingPossibilites(s, attributes, ways, adventure);
}

void Link::sendChoices(std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
  Client::sendChoices(s, name, attibutes, race, origin, culture, religion, profession, adventure);
}

MapDisplay * Link::receiveMap() { return Client::receiveMap(s, adventure); }
void Link::sendAction(Action * action) { Client::sendAction(s, action, adventure); }
std::list<Attributes *> Link::getStartingAttributes() { return *attributes; }
std::list<Way *> Link::getStartingWays() { return *ways; }
