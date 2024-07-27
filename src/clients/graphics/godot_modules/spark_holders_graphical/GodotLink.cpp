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
#include "util/MathUtil.h"

void listener(void * param) {
  Link * link = (Link *) param;
  while(!link->isClosed()) {
    link->listen();
  }
}

void GodotLink::initialize(String ip, int64_t port, String lang) {
  s = Socket();
  s.connect(std::string(ip.utf8().get_data()), port);
  link = new Link(s, std::string(lang.utf8().get_data()));
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
  #ifdef LOG
    log << "sendChoices(test, TXT_SPIRITUALIST, TXT_LEPIDOPTERAN, TXT_GREEN_TOWN, TXT_WOODLAND, TXT_HIBERO, TXT_SCHOLAR)" << std::endl;
  #endif
  link->sendChoices("test", "TXT_SPIRITUALIST", "TXT_LEPIDOPTERAN", "TXT_GREEN_TOWN", "TXT_WOODLAND", "TXT_HIBERO", "TXT_SCHOLAR");
  #ifdef LOG
    log << "sendReady()" << std::endl;
  #endif
  link->sendReady();
}

bool GodotLink::hasState() {
  #ifdef LOG
    log << "hasState()" << std::endl;
  #endif
  return link->hasState();
}

bool GodotLink::getState() {
  #ifdef LOG
    log << "getState()" << std::endl;
  #endif
  delete state;
  state = link->getState();
  return state != nullptr;
}

float GodotLink::getMoveCost(Vector3 ori, Vector3 dest) {
  #ifdef LOG
    log << "getMoveCost( (" << ori.x << "," << ori.y << "," << ori.z << "), (" << dest.x << "," << dest.y << "," << dest.z << ") )" << std::endl;
  #endif
  float result = link->getPlayer()->getRegion()->getMoveCost(link->getPlayer(), MathUtil::makeVector3(ori.z, ori.x, ori.y), MathUtil::makeVector3(dest.z, dest.x, dest.y));
  return result;
}

float GodotLink::getOrientationToTarget(Vector2 a, Vector2 b) {
  #ifdef LOG
    log << "getOrientationToTarget( (" << a.x << "," << a.y << "), (" << b.x << "," << b.y << ") )" << std::endl;
  #endif
  return MathUtil::getOrientationToTarget(a.x, a.y, b.x, b.y);
}

String GodotLink::getTime() {
  #ifdef LOG
    log << "getTime()" << std::endl;
  #endif
  return link->getAdventure()->getTime().to_string_day().c_str();
}

String GodotLink::getClock(bool terran_day) {
  #ifdef LOG
    log << "getClock( " << terran_day << " )" << std::endl;
  #endif
  if(terran_day) {
    return link->getAdventure()->getTime().to_string_clock_terra().c_str();
  }
  else {
    return link->getAdventure()->getTime().to_string_clock().c_str();
  }
}

int64_t GodotLink::getLight() {
  #ifdef LOG
    log << "getLight()" << std::endl;
  #endif
  return (int64_t) link->getAdventure()->getLight(link->getPlayer()->getCoord());
}

int64_t GodotLink::getBaseLight() {
  #ifdef LOG
    log << "getLight()" << std::endl;
  #endif
  return (int64_t) MathUtil::getLight(link->getPlayer()->getWorldCoords(), link->getAdventure()->getTime());
}


int64_t GodotLink::getMaxLight() {
  #ifdef LOG
    log << "getMaxLight()" << std::endl;
  #endif
  int32_t max = 0;
  for(int32_t i = 0; i < Settings::getWeekDuration(); i++) {
    max = std::max(max, Settings::getZenithLightPower(i));
  }
  return (int64_t) max;
}

Array GodotLink::getAvaillableBlocks() {
  #ifdef LOG
    log << "getAvaillableBlocks()" << std::endl;
  #endif
  Array result = Array();
  for(auto pair : link->getAdventure()->getDatabase()->getAvaillableBlocks()) {
    result.push_back(pair.first.c_str());
  }
  return result;
}

Dictionary GodotLink::getBlocks() {
  #ifdef LOG
    log << "getBlocks()" << std::endl;
  #endif
  Dictionary result = Dictionary();
  for(auto pair : link->getPlayer()->getRegion()->getBlocks()) {
    result[Vector3(pair.first.y, pair.first.z, pair.first.x)] = pair.second->name.c_str();
  }
  return result;
}

int64_t GodotLink::getPlayerId() {
  #ifdef LOG
    log << "getPlayerId()" << std::endl;
  #endif
  return link->getPlayer()->id;
}

Dictionary GodotLink::getCharacters() {
  #ifdef LOG
    log << "getCharacters()" << std::endl;
  #endif
  Dictionary result = Dictionary();
  for(CharacterDisplay * character : state->characters) {
    result[ (int64_t) character->id] = getDataFromCharacter(character);
  }
  return result;
}

Dictionary GodotLink::getProjectiles() {
  #ifdef LOG
    log << "getProjectiles()" << std::endl;
  #endif
  Dictionary result = Dictionary();
  for(ProjectileDisplay * projectile : state->projectiles) {
    result[projectile->id] = getDataFromProjectile(projectile);
  }
  return result;
}

Dictionary GodotLink::getFurnitures() {
  #ifdef LOG
    log << "getFurnitures()" << std::endl;
  #endif
  Dictionary result = Dictionary();
  Character * player = link->getPlayer();
  for(Furniture * furniture : player->getRegion()->getFurnitures(player)) {
    result[Vector3(furniture->getCoord().y, furniture->getCoord().z, furniture->getCoord().x)] = getDataFromFurniture(furniture);
  }
  return result;
}

Array GodotLink::getUpdatedFurnitures() {
  #ifdef LOG
    log << "getUpdatedFurnitures()" << std::endl;
  #endif
  Array result = Array();
  for(Furniture * furniture : state->changed_furnitures) {
    result.push_back(Vector3(furniture->getCoord().y, furniture->getCoord().z, furniture->getCoord().x));
  }
  return result;
}


String GodotLink::getRelation(String team1, String team2) {
  #ifdef LOG
    log << "getUpdatedFurnitures(" << team1.utf8().get_data() << ", " << team2.utf8().get_data() << ")" << std::endl;
  #endif
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
  #ifdef LOG
    log << "getDataFromItem(" << item->name << ")" << std::endl;
  #endif
  result["name"] = item->name.c_str();
  result["id"] = item->id;
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
    for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
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
    for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
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
  #ifdef LOG
    log << "getDataFromBlock(" << tile_name.utf8().get_data() << ")" << std::endl;
  #endif
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
  #ifdef LOG
    log << "getDataFromClass(" << class_name.utf8().get_data() << ")" << std::endl;
  #endif
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
  #ifdef LOG
    log << "getDataFromRace(" << race_name.utf8().get_data() << ")" << std::endl;
  #endif
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
  #ifdef LOG
    log << "getDataFromWay(" << way_name.utf8().get_data() << ")" << std::endl;
  #endif
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
  #ifdef LOG
    log << "getDataFromCharacter(" << character->id << ")" << std::endl;
  #endif
  Dictionary result = Dictionary();
  result["name"] = character->name.c_str();
  result["hp"] = character->hp;
  result["maxHp"] = character->maxHp;
  result["mana"] = character->mana;
  result["maxMana"] = character->maxMana;
  result["shield"] = character->shield;
  result["maxShield"] = character->maxShield;
  result["hunger"] = character->hunger;
  result["thirst"] = character->thirst;
  result["stamina"] = character->stamina;
  result["sanity"] = character->sanity;
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
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damage_reductions.push_back(character->damage_reductions[i]);
  }
  result["damage_reductions"] = damage_reductions;
  Array damages = Array();
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages.push_back(character->damages[i]);
  }
  result["damages"] = damages;
  result["teamRelation"] = character->teamRelation;
  return result;
}

Dictionary GodotLink::getStatsFromCharacter() {
  #ifdef LOG
    log << "getStatsFromCharacter()" << std::endl;
  #endif
  Dictionary result = Dictionary();
  Character * character = link->getPlayer();
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
  result["hunger"] = character->getHunger();
  result["thirst"] = character->getThirst();
  result["stamina"] = character->getStamina();
  result["sanity"] = character->getSanity();
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

Dictionary GodotLink::getInventoryFromCharacter() {
  #ifdef LOG
    log << "getInventoryFromCharacter()" << std::endl;
  #endif
  Dictionary result = Dictionary();
  Gear * gear = link->getPlayer()->getGear();
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

Dictionary GodotLink::getSkillsFromCharacter() {
  #ifdef LOG
    log << "getSkillsFromCharacter()" << std::endl;
  #endif
  Dictionary result = Dictionary();

  return result;
}


Dictionary GodotLink::getDataFromProjectile(ProjectileDisplay * projectile) {
  #ifdef LOG
    log << "getDataFromProjectile(" << projectile->name << ")" << std::endl;
  #endif
  Dictionary result = Dictionary();
  result["name"] = projectile->name.c_str();
  result["projectile_type"] = projectile->projectile_type;
  result["size"] = projectile->size;
  result["x"] = projectile->x;
  result["y"] = projectile->y;
  result["z"] = projectile->z;
  result["orientation"] = projectile->orientation;
  Array damages = Array();
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
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
  #ifdef LOG
    log << "getDataFromFurniture(" << furniture->name << ")" << std::endl;
  #endif
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

void GodotLink::send_action(Dictionary action) {
  #ifdef LOG
    log << "send_action()" << std::endl;
  #endif
  int32_t type = (int32_t) (int64_t) action["type"];
  void * arg1 = 0;
  void * arg2 = 0;
  int32_t overcharge_power = 1;
  int32_t overcharge_duration = 1;
  int32_t overcharge_range = 1;
  switch(type) {
    case ACTION_IDLE:
    case ACTION_RESPITE:
    case ACTION_REST:
    case ACTION_BREAKPOINT:
    case ACTION_CHANNEL:
      break;
    case ACTION_MOVE: {
      float orientation = action["arg1"];
      #ifdef LOG
        log << "orientation: " << orientation << std::endl;
      #endif
      arg1 = (void *) &orientation;
      break;
    }
    case ACTION_STRIKE:
    case ACTION_ACTIVATION: {
      Dictionary target_ori = action["arg1"];
      Target * target = new Target();
      target->type = (int32_t) (int64_t) target_ori["type"];
      target->id = (int64_t) (int64_t) target_ori["id"];
      Vector3 pos = (Vector3) target_ori["pos"];
      target->coord = MathUtil::makeVector3(pos.x, pos.y, pos.z);
      arg1 = (void *) target;
      break;
    }
    case ACTION_RELOAD:
    case ACTION_GRAB:
    case ACTION_USE_ITEM: {
      Dictionary slot_ori = action["arg1"];
      ItemSlot * slot = new ItemSlot();
      slot->x = (int32_t) (int64_t) slot_ori["x"];
      slot->y = (int32_t) (int64_t) slot_ori["y"];
      slot->slot = (int32_t) (int64_t) slot_ori["slot"];
      arg1 = (void *) slot;
      break;
    }
    case ACTION_SWAP_GEAR: {
      Dictionary slot1_ori = action["arg1"];
      ItemSlot * slot1 = new ItemSlot();
      slot1->x = (int32_t) (int64_t) slot1_ori["x"];
      slot1->y = (int32_t) (int64_t) slot1_ori["y"];
      slot1->slot = (int32_t) (int64_t) slot1_ori["slot"];
      arg1 = (void *) slot1;
      Dictionary slot2_ori = action["arg2"];
      ItemSlot * slot2 = new ItemSlot();
      slot2->x = (int32_t) (int64_t) slot2_ori["x"];
      slot2->y = (int32_t) (int64_t) slot2_ori["y"];
      slot2->slot = (int32_t) (int64_t) slot2_ori["slot"];
      arg2 = (void *) slot2;
      break;
    }
    case ACTION_USE_SKILL: {
      Dictionary target_ori = action["arg1"];
      Target * target = new Target();
      target->type = (int32_t) (int64_t) target_ori["type"];
      target->id = (int64_t) (int64_t) target_ori["id"];
      Vector3 pos = (Vector3) target_ori["pos"];
      target->coord = MathUtil::makeVector3(pos.x, pos.y, pos.z);
      arg1 = (void *) target;
      Skill * skill = (Skill *) link->getAdventure()->getDatabase()->getSkill(std::string( ( (String) action["arg2"]).utf8().get_data()));
      arg2 = (void *) skill;
      overcharge_power = (int32_t) action["overchage_power"];
      overcharge_duration = (int32_t) action["overchage_duration"];
      overcharge_range = (int32_t) action["overchage_range"];
      break;
    }
  }
  link->sendAction(type, arg1, arg2, overcharge_power, overcharge_duration, overcharge_range);
}

void GodotLink::close() {
  #ifdef LOG
    log << "close()" << std::endl;
  #endif
  link->markClosed();
  s.close();
  #ifdef LOG
    log.close();
  #endif
  delete link;
  delete state;
  delete translator;
}

void GodotLink::_bind_methods() {
  ClassDB::bind_method(D_METHOD("initialize", "ip", "port", "lang"), &GodotLink::initialize);
  ClassDB::bind_method(D_METHOD("hasState"), &GodotLink::hasState);
  ClassDB::bind_method(D_METHOD("getState"), &GodotLink::getState);
  ClassDB::bind_method(D_METHOD("getMoveCost", "ori", "dest"), &GodotLink::getMoveCost);
  ClassDB::bind_method(D_METHOD("getTime"), &GodotLink::getTime);
  ClassDB::bind_method(D_METHOD("getClock", "terran_day"), &GodotLink::getClock);
  ClassDB::bind_method(D_METHOD("getLight"), &GodotLink::getLight);
  ClassDB::bind_method(D_METHOD("getBaseLight"), &GodotLink::getBaseLight);
  ClassDB::bind_method(D_METHOD("getMaxLight"), &GodotLink::getMaxLight);
  ClassDB::bind_method(D_METHOD("getOrientationToTarget", "a", "b"), &GodotLink::getOrientationToTarget);
  ClassDB::bind_method(D_METHOD("getAvaillableBlocks"), &GodotLink::getAvaillableBlocks);
  ClassDB::bind_method(D_METHOD("getBlocks"), &GodotLink::getBlocks);
  ClassDB::bind_method(D_METHOD("getPlayerId"), &GodotLink::getPlayerId);
  ClassDB::bind_method(D_METHOD("getCharacters"), &GodotLink::getCharacters);
  ClassDB::bind_method(D_METHOD("getProjectiles"), &GodotLink::getProjectiles);
  ClassDB::bind_method(D_METHOD("getFurnitures"), &GodotLink::getFurnitures);
  ClassDB::bind_method(D_METHOD("getUpdatedFurnitures"), &GodotLink::getUpdatedFurnitures);
  ClassDB::bind_method(D_METHOD("getRelation", "team1", "team2"), &GodotLink::getRelation);
  ClassDB::bind_method(D_METHOD("getDataFromBlock", "block"), &GodotLink::getDataFromBlock);
  ClassDB::bind_method(D_METHOD("getDataFromClass", "class"), &GodotLink::getDataFromClass);
  ClassDB::bind_method(D_METHOD("getDataFromRace", "race"), &GodotLink::getDataFromRace);
  ClassDB::bind_method(D_METHOD("getDataFromWay", "way"), &GodotLink::getDataFromWay);
  ClassDB::bind_method(D_METHOD("getStatsFromCharacter"), &GodotLink::getStatsFromCharacter);
  ClassDB::bind_method(D_METHOD("getInventoryFromCharacter"), &GodotLink::getInventoryFromCharacter);
  ClassDB::bind_method(D_METHOD("send_action", "action"), &GodotLink::send_action);
  ClassDB::bind_method(D_METHOD("close"), &GodotLink::close);
}
