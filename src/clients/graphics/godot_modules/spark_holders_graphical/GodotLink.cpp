#include "GodotLink.h"

#include "data/Adventure.h"
#include "data/Database.h"

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
  // link->sendChoices(choices[0], choices[1], choices[2], choices[3], choices[4], choices[5], choices[6]);
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
  ClassDB::bind_method(D_METHOD("getAvaillableTiles"), &GodotLink::getAvaillableTiles);
  ClassDB::bind_method(D_METHOD("getPathFromTile", "tile"), &GodotLink::getPathFromTile);
}
