#include "GodotLink.h"

#include "data/Database.h"
#include "data/Map.h"

#include "util/FileOpener.h"

#include "data/ClientSettings.h"

void GodotLink::initialize(String ip) {//std::string p_ip, int p_port, std::string p_lang) {
  Database * temp = new Database();
  FileOpener::ClientSettingsOpener("data" + FileOpener::PATH_DELIMITER + "settings_client.data", temp);
  delete temp;
  s = Socket();
  s.connect(std::string(ip.utf8().get_data()), ClientSettings::getPort());
  link = new Link(s);
  translator = link->initialize(ClientSettings::getLang());
  std::vector<std::string> choices;
  // choices = Display::selectChoices(link->getStartingAttributes(), link->getStartingWays(), link->getWaysIncompatibilities(), t);
  link->sendChoices("test", "TXT_DUELIST", "TXT_HUMAN", "TXT_ARMYTHAS", "TXT_TAGRAN", "TXT_BESTIONA", "TXT_SOLIDER");
}

void GodotLink::receiveState() {
  state = link->receiveState();
}

float GodotLink::getMoveCost(int64_t character_id, int y, int x) {
  return state->map->getMoveCost(link->getAdventure()->getCharacter((long) character_id), y, x);
}

Array GodotLink::getAvaillableTiles() {
  Array result = Array();
  for(auto pair : link->getAdventure()->getDatabase()->getAvaillableTiles()) {
    result.push_back(pair.first.c_str());
  } 
  return result;
}

String GodotLink::getPathFromTile(String tile) {
  return link->getAdventure()->getDatabase()->getTileRes(std::string(tile.utf8().get_data())).c_str();
}

void GodotLink::_bind_methods() {
  ClassDB::bind_method(D_METHOD("initialize", "ip"), &GodotLink::initialize);
  ClassDB::bind_method(D_METHOD("receiveState"), &GodotLink::receiveState);
  ClassDB::bind_method(D_METHOD("getMoveCost", "id", "x", "z"), &GodotLink::getMoveCost);
  ClassDB::bind_method(D_METHOD("getAvaillableTiles"), &GodotLink::getAvaillableTiles);
  ClassDB::bind_method(D_METHOD("getPathFromTile", "tile"), &GodotLink::getPathFromTile);
}
