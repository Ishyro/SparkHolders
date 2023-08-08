#include "GodotLink.h"

#include "data/Database.h"
#include "data/Map.h"

#include "util/FileOpener.h"
#include "util/MapUtil.h"

#include "data/ClientSettings.h"

void GodotLink::initialize(String ip) {
  Database * temp = new Database();
  FileOpener::ClientSettingsOpener("data" + FileOpener::PATH_DELIMITER + "settings_client.data", temp);
  delete temp;
  s = Socket();
  s.connect(std::string(ip.utf8().get_data()), ClientSettings::getPort());
  link = new Link(s);
  translator = link->initialize(ClientSettings::getLang());
  std::vector<std::string> choices;
  // choices = Display::selectChoices(link->getStartingAttributes(), link->getStartingWays(), link->getWaysIncompatibilities(), t);
  link->sendChoices("test", "TXT_DUELIST", "TXT_HUMAN", "TXT_ARMYTHAS", "TXT_TAGRAN", "TXT_BEASTIONA_FOLLOWER", "TXT_SOLDIER");
}

void GodotLink::receiveState() {
  state = link->receiveState();
}

float GodotLink::getMoveCost(int64_t character_id, float oriX, float oriY, float destX, float destY) {
  float result = state->map->getMoveCost(link->getPlayer((long) character_id), oriX, oriY, destX, destY);
  return result;
}

float GodotLink::getOrientationToTarget(Vector2 a, Vector2 b) {
  return MapUtil::getOrientationToTarget(a.x, a.y, b.x, b.y);
}

Vector3 GodotLink::getSizes() {
  return Vector3(state->map->sizeY, state->map->sizeZ, state->map->sizeX);
}

Vector3 GodotLink::getOffsets() {
  return Vector3(state->map->offsetY, state->map->offsetZ, state->map->offsetX);
}

Array GodotLink::getAvaillableTiles() {
  Array result = Array();
  for(auto pair : link->getAdventure()->getDatabase()->getAvaillableTiles()) {
    result.push_back(pair.first.c_str());
  }
  return result;
}

Array GodotLink::getTiles() {
  Array result = Array();
  for(int y = state->map->offsetY; y < state->map->offsetY + state->map->sizeY; y++) {
    Array result_y = Array();
    for(int x = state->map->offsetX; x < state->map->offsetX + state->map->sizeX; x++) {
      result_y.push_back(state->map->getTile(x, y)->name.c_str());
    }
    result.push_back(result_y);
  }
  return result;
}

Array GodotLink::getLights() {
  Array result = Array();
  for(int y = state->map->offsetY; y < state->map->offsetY + state->map->sizeY; y++) {
    Array result_y = Array();
    for(int x = state->map->offsetX; x < state->map->offsetX + state->map->sizeX; x++) {
      result_y.push_back(state->map->getLight(x, y));
    }
    result.push_back(result_y);
  }
  return result;
}

Array GodotLink::getControlledParty() {
  Array result = Array();
  for(CharacterDisplay * character : state->characters) {
    if(character->id == link->getPlayer()->id) {
      result.push_back( (int64_t) character->id);
    }
  }
  return result;
}

Dictionary GodotLink::getCharacters() {
  Dictionary result = Dictionary();
  for(CharacterDisplay * character : state->characters) {
    result[ (int64_t) character->id] = getDataFromCharacter(character->id);
  }
  return result;
}

Dictionary GodotLink::getProjectiles() {
  Dictionary result = Dictionary();
  for(ProjectileDisplay * projectile : state->projectiles) {
    result[ (int64_t) projectile->id] = getDataFromProjectile(projectile->id);
  }
  return result;
}

String GodotLink::getRelation(String team1, String team2) {
  switch(link->getAdventure()->getDatabase()->getRelation(std::string(team1.utf8().get_data()), std::string(team2.utf8().get_data()))) {
    case TEAM_SAME:
      return "SAME";
    case TEAM_ALLY:
      return "ALLY";
    case TEAM_NEUTRAL:
      return "NEUTRAL";
    case TEAM_ENEMY:
      return "ENEMY";
  }
}

Dictionary GodotLink::getDataFromTile(String tile_name) {
  Dictionary result = Dictionary();
  Tile * tile = (Tile *) link->getAdventure()->getDatabase()->getTile(std::string(tile_name.utf8().get_data()));
  result["untraversable"] = tile->untraversable;
  result["opaque"] = tile->opaque;
  result["solid"] = tile->solid;
  result["light"] = tile->light;
  result["ap_cost"] = tile->ap_cost;
  result["path"] = link->getAdventure()->getDatabase()->getTileRes(std::string(tile_name.utf8().get_data())).c_str();
  return result;
}

Dictionary GodotLink::getDataFromCharacter(long id) {
  Dictionary result = Dictionary();
  for(CharacterDisplay * character : state->characters) {
    if(character->id == id) {
      result["name"] = character->name.c_str();
      result["hp"] = character->hp;
      result["maxHp"] = character->maxHp;
      result["mana"] = character->mana;
      result["maxMana"] = character->maxMana;
      result["stamina"] = character->stamina;
      result["satiety"] = character->satiety;
      result["soulBurn"] = character->soulBurn;
      result["soulBurnTreshold"] = character->soulBurnTreshold;
      result["flow"] = character->flow;
      result["player_character"] = character->player_character;
      result["type"] = character->type;
      result["x"] = character->x;
      result["y"] = character->y;
      result["z"] = character->z;
      result["size"] = character->size;
      result["orientation"] = character->orientation;
      result["team"] = character->team.c_str();
      result["armor"] = character->armor;
      result["xp"] = character->xp;
      result["level"] = character->level;
      Array damage_reductions = Array();
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damage_reductions.push_back(character->damage_reductions[i]);
      }
      result["damage_reductions"] = damage_reductions;
      Array damages = Array();
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages.push_back(character->damages[i]);
      }
      result["damages"] = damages;
      result["teamRelation"] = character->teamRelation;
      break;
    }
  }
  return result;
}

Dictionary GodotLink::getDataFromProjectile(long id) {
  Dictionary result = Dictionary();
  for(ProjectileDisplay * projectile : state->projectiles) {
    if(projectile->id == id) {
      result["name"] = projectile->name.c_str();
      result["projectile_type"] = projectile->projectile_type;
      result["size"] = projectile->size;
      result["x"] = projectile->x;
      result["y"] = projectile->y;
      result["z"] = projectile->z;
      result["orientation"] = projectile->orientation;
      Array damages = Array();
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages.push_back(projectile->damages[i]);
      }
      result["damages"] = damages;
      result["speed"] = projectile->speed;
      result["area"] = projectile->area;
      result["waste_per_tick"] = projectile->waste_per_tick;
      result["waste_per_area"] = projectile->waste_per_area;
      result["waste_per_hit"] = projectile->waste_per_hit;
      break;
    }
  }
  return result;
}

void GodotLink::close() {
  s.close();
  delete link;
  delete state;
  delete translator;
}

void GodotLink::_bind_methods() {
  ClassDB::bind_method(D_METHOD("initialize", "ip"), &GodotLink::initialize);
  ClassDB::bind_method(D_METHOD("receiveState"), &GodotLink::receiveState);
  ClassDB::bind_method(D_METHOD("getMoveCost", "id", "oriX", "oriY", "destX", "destY"), &GodotLink::getMoveCost);
  ClassDB::bind_method(D_METHOD("getOrientationToTarget", "a", "b"), &GodotLink::getOrientationToTarget);
  ClassDB::bind_method(D_METHOD("getSizes"), &GodotLink::getSizes);
  ClassDB::bind_method(D_METHOD("getOffsets"), &GodotLink::getOffsets);
  ClassDB::bind_method(D_METHOD("getAvaillableTiles"), &GodotLink::getAvaillableTiles);
  ClassDB::bind_method(D_METHOD("getTiles"), &GodotLink::getTiles);
  ClassDB::bind_method(D_METHOD("getLights"), &GodotLink::getLights);
  ClassDB::bind_method(D_METHOD("getControlledParty"), &GodotLink::getControlledParty);
  ClassDB::bind_method(D_METHOD("getCharacters"), &GodotLink::getCharacters);
  ClassDB::bind_method(D_METHOD("getProjectiles"), &GodotLink::getProjectiles);
  ClassDB::bind_method(D_METHOD("getRelation", "team1", "team2"), &GodotLink::getRelation);
  ClassDB::bind_method(D_METHOD("getDataFromTile", "tile"), &GodotLink::getDataFromTile);
  ClassDB::bind_method(D_METHOD("close"), &GodotLink::close);
}
