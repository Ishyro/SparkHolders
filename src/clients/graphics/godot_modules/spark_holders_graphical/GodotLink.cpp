#include "GodotLink.h"

#include "data/Character.h"
#include "data/Database.h"
#include "data/Map.h"
#include "data/Region.h"

#include "data/items/Gear.h"

#include "data/ways/Way.h"
#include "data/ways/Attributes.h"
#include "data/ways/Race.h"

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
  link->sendChoices("test", "TXT_HOMUNCULIST", "TXT_LEPIDOPTERAN", "TXT_GREEN_TOWN", "TXT_WOODLAND", "TXT_HIBERO", "TXT_SCHOLAR");
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

float GodotLink::getMoveCost(int64_t character_id, Vector3 ori, Vector3 dest) {
  float result = link->getPlayer((long) character_id)->getRegion()->getMoveCost(link->getPlayer((long) character_id), MapUtil::makeVector3(ori.z, ori.x, ori.y), MapUtil::makeVector3(dest.z, dest.x, dest.y));
  return result;
}

float GodotLink::getOrientationToTarget(Vector2 a, Vector2 b) {
  return MapUtil::getOrientationToTarget(a.x, a.y, b.x, b.y);
}

Array GodotLink::getAvaillableBlocks() {
  Array result = Array();
  for(auto pair : link->getAdventure()->getDatabase()->getAvaillableBlocks()) {
    result.push_back(pair.first.c_str());
  }
  return result;
}

Dictionary GodotLink::getBlocks(int64_t character_id) {
  Dictionary result = Dictionary();
  for(auto pair : link->getPlayer((long) character_id)->getRegion()->getBlocks()) {
    result[Vector3(pair.first.y, pair.first.z, pair.first.x)] = pair.second->name.c_str();
  }
  return result;
}

Array GodotLink::getLights(int64_t character_id) {
  //Map * map = state->maps.at((long) character_id);
  Array result = Array();
  /*
  for(int y = map->offsetY; y < map->offsetY + map->sizeY; y++) {
    Array result_y = Array();
    for(int x = map->offsetX; x < map->offsetX + map->sizeX; x++) {
      result_y.push_back(map->getLight(x, y));
    }
    result.push_back(result_y);
  }
  */
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
    result[ (int64_t) character->id] = getDataFromCharacter(character);
  }
  return result;
}

Dictionary GodotLink::getProjectiles() {
  Dictionary result = Dictionary();
  for(ProjectileDisplay * projectile : state->projectiles) {
    result[ (int64_t) projectile->id] = getDataFromProjectile(projectile);
  }
  return result;
}

Dictionary GodotLink::getFurnitures() {
  Dictionary result = Dictionary();
  for(Character * player : link->getPlayers()) {
    for(Furniture * furniture : player->getRegion()->getFurnitures(player)) {
      result[Vector3(furniture->getCoord().y, furniture->getCoord().z, furniture->getCoord().x)] = getDataFromFurniture(furniture);
    }
  }
  return result;
}

Array GodotLink::getUpdatedFurnitures() {
  Array result = Array();
  for(Furniture * furniture : state->changed_furnitures) {
    result.push_back(Vector3(furniture->getCoord().y, furniture->getCoord().z, furniture->getCoord().x));
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

Dictionary GodotLink::getDataFromItem(Item * item) {
  Dictionary result = Dictionary();
  if(item == nullptr) {
    return result;
  }
  result["name"] = item->name.c_str();
  result["id"] = (int64_t) item->id;
  result["type"] = item->type;
  result["type2"] = item->type2;
  result["tier"] = item->tier;
  result["max_tier"] = item->max_tier;
  result["weight"] = item->weight;
  result["sizeX"] = item->sizeX;
  result["sizeY"] = item->sizeY;
  result["gold_value"] = item->gold_value;
  result["droppable"] = item->droppable;
  result["usable"] = item->usable;
  result["consumable"] = item->consumable;
  result["use_time"] = item->use_time;
  if(item->type == ITEM_SERIAL || item->type == ITEM_AMMUNITION) {
    result["number"] = ((SerialItem *) item)->getNumber();
    result["max"] = ((SerialItem *) item)->max;
  }
  if(item->type == ITEM_AMMUNITION) {
    result["projectile"] = "";//((ItemAmmunition *) item)->projectile;
  }
  if(item->type == ITEM_ARMOR || item->type == ITEM_WEAPON || item->type == ITEM_CONTAINER) {
    result["swap_time"] = ((GearItem *) item)->swap_time;
  }
  if(item->type == ITEM_ARMOR) {
    Array damage_reductions = Array();
    for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
      damage_reductions.push_back(((ArmorItem *) item)->getDamageReductionFromType(i));
    }
    result["damage_reductions"] = damage_reductions;
  }
  if(item->type == ITEM_WEAPON) {
    result["range"] = ((WeaponItem *) item)->range;
    result["strike_time"] = ((WeaponItem *) item)->strike_time;
    result["use_projectile"] = ((WeaponItem *) item)->use_projectile;
    result["use_ammo"] = ((WeaponItem *) item)->use_ammo;
    result["ammo_type"] = ((WeaponItem *) item)->ammo_type;
    result["capacity"] = ((WeaponItem *) item)->capacity;
    result["reload_time"] = ((WeaponItem *) item)->reload_time;
    Array damages = Array();
    for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
      damages.push_back(((WeaponItem *) item)->getDamageFromType(i));
    }
    result["damages"] = damages;
  }
  if(item->type == ITEM_CONTAINER) {
    result["canTakeFrom"] = ((ContainerItem *) item)->canTakeFrom;
    result["repercuteWeight"] = ((ContainerItem *) item)->repercuteWeight;
    result["isLimited"] = ((ContainerItem *) item)->isLimited;
    result["limit_type"] = ((ContainerItem *) item)->limit_type;
    result["contentX"] = ((ContainerItem *) item)->contentX;
    result["contentY"] = ((ContainerItem *) item)->contentY;
  }
  String path = link->getAdventure()->getDatabase()->getItemFile(item->name).c_str();
  result["path"] = path;
  return result;
}

Dictionary GodotLink::getDataFromBlock(String tile_name) {
  Dictionary result = Dictionary();
  Block * block = (Block *) link->getAdventure()->getDatabase()->getBlock(std::string(tile_name.utf8().get_data()));
  result["type"] = block->type;
  result["unwalkable"] = block->unwalkable;
  result["opaque"] = block->opaque;
  result["light"] = block->light;
  result["orientation"] = block->orientation;
  result["ap_cost"] = block->ap_cost;
  result["material"] = block->material.c_str();
  return result;
}


Dictionary GodotLink::getDataFromClass(String class_name) {
  Dictionary result = Dictionary();
  if(class_name == "") {
    return result;
  }
  Attributes * attributes = (Attributes *) link->getAdventure()->getDatabase()->getAttributes(std::string(class_name.utf8().get_data()));
  result["name"] = attributes->name.c_str();
  result["type"] = attributes->type;
  result["tier"] = attributes->tier;
  result["baseHp"] = attributes->baseHp;
  result["baseMana"] = attributes->baseMana;
  result["baseShield"] = attributes->baseShield;
  result["baseDamageMult"] = attributes->baseDamageMult;
  result["baseSoulBurn"] = attributes->baseSoulBurn;
  result["baseFlow"] = attributes->baseFlow;
  result["baseVisionRange"] = attributes->baseVisionRange;
  result["baseVisionPower"] = attributes->baseVisionPower;
  result["baseDetectionRange"] = attributes->baseDetectionRange;
  result["hpIncr"] = attributes->hpIncr;
  result["manaIncr"] = attributes->manaIncr;
  result["shieldIncr"] = attributes->shieldIncr;
  result["damageMultIncr"] = attributes->damageMultIncr;
  result["soulBurnIncr"] = attributes->soulBurnIncr;
  result["flowIncr"] = attributes->flowIncr;
  result["path"] = link->getAdventure()->getDatabase()->getAttributesFile(attributes->name).c_str();
  return result;
}
Dictionary GodotLink::getDataFromRace(String race_name) {
  Dictionary result = Dictionary();
  if(race_name == "") {
    return result;
  }
  Race * race = (Race *) link->getAdventure()->getDatabase()->getWay(std::string(race_name.utf8().get_data()));
  result["name"] = race->name.c_str();
  result["type"] = race->type;
  result["tier"] = race->tier;
  result["baseHp"] = race->baseHp;
  result["baseMana"] = race->baseMana;
  result["baseShield"] = race->baseShield;
  result["baseDamageMult"] = race->baseDamageMult;
  result["baseSoulBurn"] = race->baseSoulBurn;
  result["baseFlow"] = race->baseFlow;
  result["baseVisionRange"] = race->baseVisionRange;
  result["baseVisionPower"] = race->baseVisionPower;
  result["baseDetectionRange"] = race->baseDetectionRange;
  result["hpIncr"] = race->hpIncr;
  result["manaIncr"] = race->manaIncr;
  result["shieldIncr"] = race->shieldIncr;
  result["damageMultIncr"] = race->damageMultIncr;
  result["soulBurnIncr"] = race->soulBurnIncr;
  result["flowIncr"] = race->flowIncr;
  result["size"] = race->size;
  result["need_to_eat"] = race->need_to_eat;
  result["can_eat_food"] = race->can_eat_food;
  result["need_to_sleep"] = race->need_to_sleep;
  result["has_soulspark"] = race->has_soulspark;
  result["action_time_modifier"] = race->action_time_modifier;
  result["strike_time_modifier"] = race->strike_time_modifier;
  result["skill_time_modifier"] = race->skill_time_modifier;
  result["movement_time_modifier"] = race->movement_time_modifier;
  String path = link->getAdventure()->getDatabase()->getWayFile(race->name).c_str();
  result["path"] = path;
  result["path_3d"] = path.replace(".png", ".glb");
  return result;
}
Dictionary GodotLink::getDataFromWay(String way_name) {
  Dictionary result = Dictionary();
  if(way_name == "") {
    return result;
  }
  Way * way = (Way *) link->getAdventure()->getDatabase()->getWay(std::string(way_name.utf8().get_data()));
  result["name"] = way->name.c_str();
  result["type"] = way->type;
  result["tier"] = way->tier;
  result["baseHp"] = way->baseHp;
  result["baseMana"] = way->baseMana;
  result["baseShield"] = way->baseShield;
  result["baseDamageMult"] = way->baseDamageMult;
  result["baseSoulBurn"] = way->baseSoulBurn;
  result["baseFlow"] = way->baseFlow;
  result["baseVisionRange"] = way->baseVisionRange;
  result["baseVisionPower"] = way->baseVisionPower;
  result["baseDetectionRange"] = way->baseDetectionRange;
  result["hpIncr"] = way->hpIncr;
  result["manaIncr"] = way->manaIncr;
  result["shieldIncr"] = way->shieldIncr;
  result["damageMultIncr"] = way->damageMultIncr;
  result["soulBurnIncr"] = way->soulBurnIncr;
  result["flowIncr"] = way->flowIncr;
  result["path"] = link->getAdventure()->getDatabase()->getWayFile(way->name).c_str();
  return result;
}

Dictionary GodotLink::getDataFromCharacter(CharacterDisplay * character) {
  Dictionary result = Dictionary();
  result["name"] = character->name.c_str();
  result["hp"] = character->hp;
  result["maxHp"] = character->maxHp;
  result["mana"] = character->mana;
  result["maxMana"] = character->maxMana;
  result["shield"] = character->shield;
  result["maxShield"] = character->maxShield;
  result["stamina"] = character->stamina;
  result["satiety"] = character->satiety;
  result["currentSoulBurn"] = character->soulBurn;
  result["soulBurnThreshold"] = character->soulBurnTreshold;
  result["flow"] = character->flow;
  result["player_character"] = character->player_character;
  result["type"] = character->type;
  result["x"] = character->x;
  result["y"] = character->y;
  result["z"] = character->z;
  result["size"] = character->size;
  result["orientation"] = character->orientation;
  result["team"] = character->team.c_str();
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
  return result;
}

Dictionary GodotLink::getStatsFromCharacter(int64_t character_id) {
  Dictionary result = Dictionary();
  Character * character = link->getPlayer( (long) character_id);
  result["name"] = character->name.c_str();
  result["maxHp"] = character->getMaxHp();
  result["maxMana"] = character->getMaxMana();
  result["maxShield"] = character->getMaxShield();
  result["soulBurnThreshold"] = character->getSoulBurnThreshold();
  result["hp"] = character->getHp();
  result["mana"] = character->getMana();
  result["shield"] = character->getShield();
  result["currentSoulBurn"] = character->getCurrentSoulBurn();
  result["strengthening"] = (int64_t) std::floor((character->getDamageMultiplier() - 1.F) * 100.F);
  result["flow"] = character->getFlow();
  result["cloaking"] = character->cloakPower();
  result["visionRange"] = character->getVisionRange();
  result["visionPower"] = character->getVisionPower();
  result["detectionRange"] = character->getDetectionRange();
  result["stamina"] = character->getStamina();
  result["satiety"] = character->getSatiety();
  //result["channeledMana"] = character->getChanneledMana();
  result["size"] = character->getSize();
  result["xp"] = (int64_t) character->getXP();
  result["level"] = character->getLevel();
  result["rawPower"] = (int64_t) character->getRawPower();
  result["globalSpeed"] = character->getActionTimeModifier();
  result["handActionSpeed"] = character->getHandActionTimeModifier();
  result["movementSpeed"] = character->getMovementTimeModifier();
  Attributes * main_class = character->getMainClass();
  if(main_class != nullptr) {
    result["main_class"] = main_class->name.c_str();
  }
  else {
    result["main_class"] = "";
  }
  Attributes * sub_class = character->getSecondClass();;
  if(sub_class != nullptr) {
    result["sub_class"] = sub_class->name.c_str();
  }
  else {
    result["sub_class"] = "";
  }
  Attributes * spec_class = character->getSpecClass();;
  if(spec_class != nullptr) {
    result["spec_class"] = spec_class->name.c_str();
  }
  else {
    result["spec_class"] = "";
  }
  result["race"] = character->getRace()->name.c_str();
  Array race_modifiers = Array();
  for(Race * race : character->getRaceModifiers()) {
    race_modifiers.push_back(race->name.c_str());
  }
  result["race_modifiers"] = race_modifiers;
  result["origin"] = character->getOrigin()->name.c_str();
  result["culture"] = character->getCulture()->name.c_str();
  result["religion"] = character->getReligion()->name.c_str();
  result["profession"] = character->getProfession()->name.c_str();
  Array titles = Array();
  for(Way * title : character->getTitles()) {
    titles.push_back(title->name.c_str());
  }
  result["titles"] = titles;
  return result;
}

Dictionary GodotLink::getInventoryFromCharacter(int64_t character_id) {
  Dictionary result = Dictionary();
  Gear * gear = link->getPlayer( (long) character_id)->getGear();
  result["mantle"] = getDataFromItem(gear->getMantle());
  result["helmet"] = getDataFromItem(gear->getHelmet());
  result["armor"] = getDataFromItem(gear->getArmor());
  result["gauntlets"] = getDataFromItem(gear->getGauntlets());
  result["boots"] = getDataFromItem(gear->getBoots());
  result["lantern"] = getDataFromItem(gear->getLantern());
  result["amulet"] = getDataFromItem(gear->getAmulet());
  result["ring_1"] = getDataFromItem(gear->getRing_1());
  result["ring_2"] = getDataFromItem(gear->getRing_2());
  result["weapon_1"] = getDataFromItem(gear->getWeapon_1());
  result["weapon_2"] = getDataFromItem(gear->getWeapon_2());
  result["backup_weapon_1"] = getDataFromItem(gear->getBackupWeapon_1());
  result["backup_weapon_2"] = getDataFromItem(gear->getBackupWeapon_2());
  result["bag"] = getDataFromItem(gear->getBag());
  result["belt"] = getDataFromItem(gear->getBelt());
  Dictionary belt_content = Dictionary();
  for(ItemSlot * slot : gear->getBelt()->getItems()) {
    Vector2 vec = Vector2(slot->x, slot->y);
    belt_content[vec] = getDataFromItem(slot->item);
  }
  result["belt_content"] = belt_content;
  Dictionary bag_content = Dictionary();
  for(ItemSlot * slot : gear->getBag()->getItems()) {
    Vector2 vec = Vector2(slot->x, slot->y);
    bag_content[vec] = getDataFromItem(slot->item);
  }
  result["bag_content"] = bag_content;
  return result;
}

Dictionary GodotLink::getSkillsFromCharacter(int64_t character_id) {
  Dictionary result = Dictionary();

  return result;
}


Dictionary GodotLink::getDataFromProjectile(ProjectileDisplay * projectile) {
  Dictionary result = Dictionary();
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
  return result;
}

Dictionary GodotLink::getDataFromFurniture(Furniture * furniture) {
  Dictionary result = Dictionary();
  result["name"] = furniture->name.c_str();
  result["type"] = furniture->type;
  result["sizeX"] = furniture->sizeX;
  result["sizeY"] = furniture->sizeY;
  result["sizeZ"] = furniture->sizeZ;
  result["orientation"] = furniture->getOrientation();
  result["opaque"] = furniture->getOpaque();
  result["solid"] = furniture->getSolid();
  result["unwalkable"] = furniture->getUnwalkable();
  result["light"] = furniture->getLight();
  if(furniture->type != FURNITURE_BASIC) {
    result["activation_time"] = ((ActivableFurniture *) furniture)->activation_time;
  }
  if(furniture->getLight() != 0) {
    result["fire_type"] = 0; // unused
    result["fire_pos"] = Vector3(furniture->fire_posX, furniture->fire_posY, furniture->fire_posZ);
    result["fire_size"] = furniture->fire_size;
  }
  String path = link->getAdventure()->getDatabase()->getFurnitureFile(furniture->name).c_str();
  if(furniture->type == FURNITURE_SWITCH) {
    result["isOn"] = ((SwitchFurniture *) furniture)->getIsOn();
    result["path_off"] = path.replace(".glb", "_off.glb");
  }
  else {
    result["isOn"] = true;
  }
  result["path"] = path;
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
        case ACTION_CHANNEL:
          break;
        case ACTION_MOVE:
        case ACTION_STRIKE:
        case ACTION_ACTIVATION: {
          Dictionary target_ori = ( (Array) ( (Dictionary) actions["arg1"])[id])[i];
          Target * target = new Target();
          target->type = (int) (int64_t) target_ori["type"];
          target->id = (long) (int64_t) target_ori["id"];
          Vector3 pos = (Vector3) target_ori["pos"];
          target->coord = MapUtil::makeVector3(pos.x, pos.y, pos.z);
          arg1 = (void *) target;
          break;
        }
        case ACTION_RELOAD:
        case ACTION_GRAB:
        case ACTION_USE_ITEM: {
          Dictionary slot_ori = ( (Array) ( (Dictionary) actions["arg1"])[id])[i];
          ItemSlot * slot = new ItemSlot();
          slot->x = (int) (int64_t) slot_ori["x"];
          slot->y = (int) (int64_t) slot_ori["y"];
          slot->slot = (int) (int64_t) slot_ori["slot"];
          arg1 = (void *) slot;
          break;
        }
        case ACTION_SWAP_GEAR: {
          Dictionary slot1_ori = ( (Array) ( (Dictionary) actions["arg1"])[id])[i];
          ItemSlot * slot1 = new ItemSlot();
          slot1->x = (int) (int64_t) slot1_ori["x"];
          slot1->y = (int) (int64_t) slot1_ori["y"];
          slot1->slot = (int) (int64_t) slot1_ori["slot"];
          arg1 = (void *) slot1;
          Dictionary slot2_ori = ( (Array) ( (Dictionary) actions["arg2"])[id])[i];
          ItemSlot * slot2 = new ItemSlot();
          slot2->x = (int) (int64_t) slot2_ori["x"];
          slot2->y = (int) (int64_t) slot2_ori["y"];
          slot2->slot = (int) (int64_t) slot2_ori["slot"];
          arg2 = (void *) slot2;
          break;
        }
        case ACTION_USE_SKILL: {
          Dictionary target_ori = ( (Array) ( (Dictionary) actions["arg1"])[id])[i];
          Target * target = new Target();
          target->type = (int) (int64_t) target_ori["type"];
          target->id = (long) (int64_t) target_ori["id"];
          Vector3 pos = (Vector3) target_ori["pos"];
          target->coord = MapUtil::makeVector3(pos.x, pos.y, pos.z);
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
  ClassDB::bind_method(D_METHOD("getMoveCost", "id", "ori", "dest"), &GodotLink::getMoveCost);
  ClassDB::bind_method(D_METHOD("getOrientationToTarget", "a", "b"), &GodotLink::getOrientationToTarget);
  ClassDB::bind_method(D_METHOD("getAvaillableBlocks"), &GodotLink::getAvaillableBlocks);
  ClassDB::bind_method(D_METHOD("getBlocks", "id"), &GodotLink::getBlocks);
  ClassDB::bind_method(D_METHOD("getLights", "id"), &GodotLink::getLights);
  ClassDB::bind_method(D_METHOD("getControlledParty"), &GodotLink::getControlledParty);
  ClassDB::bind_method(D_METHOD("getCharacters"), &GodotLink::getCharacters);
  ClassDB::bind_method(D_METHOD("getProjectiles"), &GodotLink::getProjectiles);
  ClassDB::bind_method(D_METHOD("getFurnitures"), &GodotLink::getFurnitures);
  ClassDB::bind_method(D_METHOD("getUpdatedFurnitures"), &GodotLink::getUpdatedFurnitures);
  ClassDB::bind_method(D_METHOD("getRelation", "team1", "team2"), &GodotLink::getRelation);
  ClassDB::bind_method(D_METHOD("getDataFromBlock", "block"), &GodotLink::getDataFromBlock);
  ClassDB::bind_method(D_METHOD("getDataFromClass", "class"), &GodotLink::getDataFromClass);
  ClassDB::bind_method(D_METHOD("getDataFromRace", "race"), &GodotLink::getDataFromRace);
  ClassDB::bind_method(D_METHOD("getDataFromWay", "way"), &GodotLink::getDataFromWay);
  ClassDB::bind_method(D_METHOD("getStatsFromCharacter", "id"), &GodotLink::getStatsFromCharacter);
  ClassDB::bind_method(D_METHOD("getInventoryFromCharacter", "id"), &GodotLink::getInventoryFromCharacter);
  ClassDB::bind_method(D_METHOD("send_actions", "actions"), &GodotLink::send_actions);
  ClassDB::bind_method(D_METHOD("close"), &GodotLink::close);
}
