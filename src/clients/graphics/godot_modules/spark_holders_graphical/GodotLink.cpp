#include "GodotLink.h"

#include "data/Database.h"
#include "data/Map.h"

#include "util/FileOpener.h"
#include "util/MapUtil.h"

#include "data/ClientSettings.h"

void listener(void * param) {
  Link * link = (Link *) param;
  while(!link->isClosed()) {
    link->listen();
  }
}

void GodotLink::initialize(String ip) {
  Database * temp = new Database();
  FileOpener::ClientSettingsOpener("data" + FileOpener::PATH_DELIMITER + "settings_client.data", temp);
  delete temp;
  s = Socket();
  s.connect(std::string(ip.utf8().get_data()), ClientSettings::getPort());
  link = new Link(s, ClientSettings::getLang());
  link->initialize("tester", "admin");
  #ifdef _WIN32_WINNT
    thread = (HANDLE) _beginthreadex(NULL, 0, (_beginthreadex_proc_type) listener, (void *) link, 0, NULL);
  #else
    thread = std::thread(listener, (void *) link);
  #endif
  while(!link->isStarted()) {
    usleep(1);
  }
  std::vector<std::string> choices;
  // choices = Display::selectChoices(link->getStartingAttributes(), link->getStartingWays(), link->getWaysIncompatibilities(), t);
  link->sendChoices("test", "TXT_DUELIST", "TXT_HUMAN", "TXT_ARMYTHAS", "TXT_TAGRAN", "TXT_BEASTIONA_FOLLOWER", "TXT_SOLDIER");
  link->sendReady();
}

bool GodotLink::hasState() {
  return link->hasState();
}

bool GodotLink::getState() {
  delete state;
  state = link->getState();
  return state != nullptr;
}

float GodotLink::getMoveCost(int64_t character_id, float oriX, float oriY, float destX, float destY) {
  float result = state->maps.at((long) character_id)->getMoveCost(link->getPlayer((long) character_id), oriX, oriY, destX, destY);
  return result;
}

float GodotLink::getOrientationToTarget(Vector2 a, Vector2 b) {
  return MapUtil::getOrientationToTarget(a.x, a.y, b.x, b.y);
}

bool GodotLink::needTilesUpdate(int64_t character_id) {
  bool result = true;
  if(mist_nbs.count(character_id) > 0) {
    result = mist_nbs.at(character_id) > state->maps.at((long) character_id)->getMistNb();
    mist_nbs.erase(character_id);
  }
  mist_nbs.insert(std::make_pair(character_id, state->maps.at((long) character_id)->getMistNb()));
  return result;
}

Vector3 GodotLink::getSizes(int64_t character_id) {
  Map * map = state->maps.at((long) character_id);
  return Vector3(map->sizeY, map->sizeZ, map->sizeX);
}

Vector3 GodotLink::getOffsets(int64_t character_id) {
  Map * map = state->maps.at((long) character_id);
  return Vector3(map->offsetY, map->offsetZ, map->offsetX);
}

Array GodotLink::getAvaillableTiles() {
  Array result = Array();
  for(auto pair : link->getAdventure()->getDatabase()->getAvaillableTiles()) {
    result.push_back(pair.first.c_str());
  }
  return result;
}

Array GodotLink::getTiles(int64_t character_id) {
  Map * map = state->maps.at((long) character_id);
  Array result = Array();
  for(int y = map->offsetY; y < map->offsetY + map->sizeY; y++) {
    Array result_y = Array();
    for(int x = map->offsetX; x < map->offsetX + map->sizeX; x++) {
      result_y.push_back(map->getTile(x, y)->name.c_str());
    }
    result.push_back(result_y);
  }
  return result;
}

Array GodotLink::getLights(int64_t character_id) {
  Map * map = state->maps.at((long) character_id);
  Array result = Array();
  for(int y = map->offsetY; y < map->offsetY + map->sizeY; y++) {
    Array result_y = Array();
    for(int x = map->offsetX; x < map->offsetX + map->sizeX; x++) {
      result_y.push_back(map->getLight(x, y));
    }
    result.push_back(result_y);
  }
  return result;
}

Array GodotLink::getControlledParty() {
  Array result = Array();
  for(long id : link->getPlayersId()) {
    result.push_back( (int64_t) id);
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

Dictionary GodotLink::getCurrentRegions() {
  Dictionary result = Dictionary();
  for(auto pair : state->maps) {
    for(long region_id : link->getAdventure()->getWorld()->getRegion(pair.second->id)) {
      result[(int64_t) region_id] = (int64_t) 0;
      for(long neighbour_id : link->getAdventure()->getWorld()->getNeighbours(region_id)) {
         result[(int64_t) neighbour_id] = (int64_t) 0;
        for(long neighbour_region_id : link->getAdventure()->getWorld()->getRegion(neighbour_id)) {
          result[(int64_t) neighbour_region_id] = (int64_t) 0;
        }
      }
    }
  }
  return result;
}

int64_t GodotLink::getMapFromCoords(Vector3 coords) {
  return (int64_t) link->getAdventure()->getWorld()->getMap(coords.z, coords.x, coords.y)->id;
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

void GodotLink::send_actions(Dictionary actions) {
  std::vector<long> ids = std::vector<long>();
  std::vector<std::vector<int>> types = std::vector<std::vector<int>>();
  std::vector<std::vector<void *>> args1 = std::vector<std::vector<void *>>();
  std::vector<std::vector<void *>> args2 = std::vector<std::vector<void *>>();
  std::vector<std::vector<int>> overcharge_powers = std::vector<std::vector<int>>();
  std::vector<std::vector<int>> overcharge_durations = std::vector<std::vector<int>>();
  std::vector<std::vector<int>> overcharge_ranges = std::vector<std::vector<int>>();
  for(int64_t iterator_id = 0; iterator_id < ((Array) actions["ids"]).size(); iterator_id++) {
    int64_t id = (int64_t) ((Array) actions["ids"])[iterator_id];
    ids.push_back( (long) id);
    std::vector<int> types_i = std::vector<int>();
    std::vector<void *> args1_i = std::vector<void *>();
    std::vector<void *> args2_i = std::vector<void *>();
    std::vector<int> overcharge_powers_i = std::vector<int>();
    std::vector<int> overcharge_durations_i = std::vector<int>();
    std::vector<int> overcharge_ranges_i = std::vector<int>();
    for(int64_t i = 0; i < ( (Array) ( (Dictionary) actions["types"])[id]).size(); i++) {
      int type = (int) (int64_t) ( (Array) ( (Dictionary) actions["types"])[id])[i];
      void * arg1 = 0;
      void * arg2 = 0;
      int overcharge_power = 1;
      int overcharge_duration = 1;
      int overcharge_range = 1;
      switch(type) {
        case ACTION_IDLE:
        case ACTION_RESPITE:
        case ACTION_REST:
        case ACTION_BREAKPOINT:
          break;
        case ACTION_MOVE:
        case ACTION_STRIKE:
        case ACTION_HEAVY_STRIKE:
        case ACTION_SHOOT: {
          Dictionary target_ori = ( (Array) ( (Dictionary) actions["arg1"])[id])[i];
          Target * target = new Target();
          target->type = (int) (int64_t) target_ori["type"];
          target->id = (long) (int64_t) target_ori["id"];
          Vector3 pos = (Vector3) target_ori["pos"];
          target->x = pos.x;
          target->y = pos.y;
          target->z = pos.z;
          arg1 = (void *) target;
          break;
        }
        case ACTION_RELOAD:
        case ACTION_SWAP_GEAR:
        case ACTION_GRAB:
        case ACTION_USE_ITEM: {
          long item_id = (long) (int64_t) ( (Array) ( (Dictionary) actions["arg2"])[id])[i];
          arg1 = (void *) item_id;
          break;
        }
        case ACTION_USE_SKILL: {
          Dictionary target_ori = ( (Array) ( (Dictionary) actions["arg1"])[id])[i];
          Target * target = new Target();
          target->type = (int) (int64_t) target_ori["type"];
          target->id = (long) (int64_t) target_ori["id"];
          Vector3 pos = (Vector3) target_ori["pos"];
          target->x = pos.x;
          target->y = pos.y;
          target->z = pos.z;
          arg1 = (void *) target;
          Skill * skill = (Skill *) link->getAdventure()->getDatabase()->getSkill(std::string( ( (String) ( (Array) ( (Dictionary) actions["arg2"])[id])[i]).utf8().get_data()));
          arg2 = (void *) skill;
          overcharge_power = (int) ( (Array) ( (Dictionary) actions["overchage_power"])[id])[i];
          overcharge_duration = (int) ( (Array) ( (Dictionary) actions["overchage_duration"])[id])[i];
          overcharge_range = (int) ( (Array) ( (Dictionary) actions["overchage_range"])[id])[i];
          break;
        }
      }
      types_i.push_back(type);
      args1_i.push_back(arg1);
      args2_i.push_back(arg2);
      overcharge_powers_i.push_back(overcharge_power);
      overcharge_durations_i.push_back(overcharge_duration);
      overcharge_ranges_i.push_back(overcharge_range);
    }
    types.push_back(types_i);
    args1.push_back(args1_i);
    args2.push_back(args2_i);
    overcharge_powers.push_back(overcharge_powers_i);
    overcharge_durations.push_back(overcharge_durations_i);
    overcharge_ranges.push_back(overcharge_ranges_i);
  }
  link->sendActions(ids, types, args1, args2, overcharge_powers, overcharge_durations, overcharge_ranges);
}

void GodotLink::close() {
  link->markClosed();
  s.close();
  log.close();
  delete link;
  delete state;
  delete translator;
}

void GodotLink::_bind_methods() {
  ClassDB::bind_method(D_METHOD("initialize", "ip"), &GodotLink::initialize);
  ClassDB::bind_method(D_METHOD("hasState"), &GodotLink::hasState);
  ClassDB::bind_method(D_METHOD("getState"), &GodotLink::getState);
  ClassDB::bind_method(D_METHOD("getMoveCost", "id", "oriX", "oriY", "destX", "destY"), &GodotLink::getMoveCost);
  ClassDB::bind_method(D_METHOD("getOrientationToTarget", "a", "b"), &GodotLink::getOrientationToTarget);
  ClassDB::bind_method(D_METHOD("needTilesUpdate", "id"), &GodotLink::needTilesUpdate);
  ClassDB::bind_method(D_METHOD("getSizes", "id"), &GodotLink::getSizes);
  ClassDB::bind_method(D_METHOD("getOffsets", "id"), &GodotLink::getOffsets);
  ClassDB::bind_method(D_METHOD("getAvaillableTiles"), &GodotLink::getAvaillableTiles);
  ClassDB::bind_method(D_METHOD("getTiles", "id"), &GodotLink::getTiles);
  ClassDB::bind_method(D_METHOD("getLights", "id"), &GodotLink::getLights);
  ClassDB::bind_method(D_METHOD("getControlledParty"), &GodotLink::getControlledParty);
  ClassDB::bind_method(D_METHOD("getCharacters"), &GodotLink::getCharacters);
  ClassDB::bind_method(D_METHOD("getProjectiles"), &GodotLink::getProjectiles);
  ClassDB::bind_method(D_METHOD("getRelation", "team1", "team2"), &GodotLink::getRelation);
  ClassDB::bind_method(D_METHOD("getCurrentRegions"), &GodotLink::getCurrentRegions);
  ClassDB::bind_method(D_METHOD("getMapFromCoords", "coords"), &GodotLink::getMapFromCoords);
  ClassDB::bind_method(D_METHOD("getDataFromTile", "tile"), &GodotLink::getDataFromTile);
  ClassDB::bind_method(D_METHOD("send_actions", "actions"), &GodotLink::send_actions);
  ClassDB::bind_method(D_METHOD("close"), &GodotLink::close);
}
