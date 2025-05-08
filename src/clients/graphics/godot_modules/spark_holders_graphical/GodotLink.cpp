#include "GodotLink.h"

#include "data/Character.h"
#include "data/Database.h"
#include "data/Region.h"

#include "data/items/Gear.h"

#include "data/skills/Skill.h"

#include "data/ways/Way.h"
#include "data/ways/Attributes.h"
#include "data/ways/Race.h"

#include "util/FileOpener.h"
#include "util/MathUtil.h"

void listener(void * param) {
  Link * link = (Link *) param;
  while(link != nullptr && !link->isClosed()) {
    link->listen();
  }
}

void GodotLink::initialize(String ip, int64_t port, String password) {
  #ifdef LOG
    log << "initialize(" << ip.utf8().get_data() << ", " << port << ", " << password.utf8().get_data() <<")" << std::endl;
  #endif
  s = Socket();
  s.connect(std::string(ip.utf8().get_data()), port);
  link = new Link(s);
  link->initialize(password.utf8().get_data());
  #ifdef _WIN32_WINNT
    thread = (HANDLE) _beginthreadex(NULL, 0, (_beginthreadex_proc_type) listener, (void *) link, 0, NULL);
  #else
    thread = std::thread(listener, (void *) link);
  #endif
  while(!link->isStarted()) {
    usleep(1);
  }
}

int64_t GodotLink::getTickRate() {
  return link->getTickRate();
}

String GodotLink::getIncompatible(String tocheck, String attributes, String race, String origin, String culture, String religion, String profession) {
  std::list<String> currents = std::list<String>();
  currents.push_back(attributes);
  currents.push_back(race);
  currents.push_back(origin);
  currents.push_back(culture);
  currents.push_back(religion);
  currents.push_back(profession);
  for(String way : currents) {
    for(std::pair<const std::string, const std::string> incompatibility : link->getAdventure()->getDatabase()->getWaysIncompatibilities()) {
      if( (incompatibility.first.c_str() == tocheck && incompatibility.second.c_str() == way) || (incompatibility.first.c_str() == way && incompatibility.second.c_str() == tocheck) ) {
        return way;
      }
    }
  }
  return "";
}

String GodotLink::getEnglishFromKey(String key) {
  return link->getEnglishFromKey(key.utf8().get_data()).c_str();
}

void GodotLink::sendChoices(String character, String attributes, String race, String origin, String culture, String religion, String profession) {
  #ifdef LOG
    log << "sendChoices(" << character.utf8().get_data() << ", " << attributes.utf8().get_data() << ", " << race.utf8().get_data() << ", " << origin.utf8().get_data() << ", " << culture.utf8().get_data() << ", " << religion.utf8().get_data() << ", " << profession.utf8().get_data() <<")" << std::endl;
  #endif
  link->sendChoices(character.utf8().get_data(), attributes.utf8().get_data(), race.utf8().get_data(), origin.utf8().get_data(), culture.utf8().get_data(), religion.utf8().get_data(), profession.utf8().get_data());
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

Dictionary GodotLink::getMacros() {
  #ifdef LOG
    log << "getMacros()" << std::endl;
  #endif
  Dictionary result = Dictionary();
  for(auto pair : link->getAdventure()->getDatabase()->getMacros()) {
    result[pair.first.c_str()] = (int64_t) pair.second;
  }
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
    log << "getClock(" << terran_day << ")" << std::endl;
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

Array GodotLink::getChunkIds(int64_t sizeZ, int64_t radius) {
  #ifdef LOG
    log << "getChunkIds(" << sizeZ << ", " << radius << ")" << std::endl;
  #endif
  Array result = Array();
  MathUtil::Vector3i coord = BlocksChunk::getChunkId(link->getPlayer()->getCoord());
  MathUtil::Vector3i current;
  for(current.z = coord.z - sizeZ * CHUNK_SIZE; current.z <= coord.z + sizeZ * CHUNK_SIZE; current.z += CHUNK_SIZE) {
    for(current.y = coord.y - radius * CHUNK_SIZE; current.y <= coord.y + radius * CHUNK_SIZE; current.y += CHUNK_SIZE) {
      for(current.x = coord.x - radius * CHUNK_SIZE; current.x <= coord.x + radius * CHUNK_SIZE; current.x += CHUNK_SIZE) {
        if(link->getAdventure()->getWorld()->getChunk(current) != nullptr) {
          result.push_back(Vector3(current.y, current.z, current.x));
        }
      }
    }
  }
  return result;
}

Dictionary GodotLink::getBlocks(Vector3 chunk_id) {
  #ifdef LOG
    log << "getBlocks(" << chunk_id.x << ", " << chunk_id.y << ", " << chunk_id.z << ")" << std::endl;
  #endif
  Dictionary result = Dictionary();
  MathUtil::Vector3i coord = MathUtil::Vector3i(chunk_id.z, chunk_id.x, chunk_id.y);
  BlocksChunk * chunk = link->getAdventure()->getWorld()->getChunk(coord);
  if(chunk != nullptr) {
    std::map<const MathUtil::Vector3i, Block *> blocks = chunk->getBlocks(coord);
    for(std::pair<MathUtil::Vector3i, Block *> pair : blocks) {
      // add the block only if it is in contact with air
      if(blocks.find(MathUtil::Vector3i(pair.first.x - 1, pair.first.y, pair.first.z)) == blocks.end() ||
        blocks.find(MathUtil::Vector3i(pair.first.x + 1, pair.first.y, pair.first.z)) == blocks.end() ||
        blocks.find(MathUtil::Vector3i(pair.first.x, pair.first.y - 1, pair.first.z)) == blocks.end() ||
        blocks.find(MathUtil::Vector3i(pair.first.x, pair.first.y + 1, pair.first.z)) == blocks.end() ||
        blocks.find(MathUtil::Vector3i(pair.first.x, pair.first.y, pair.first.z - 1)) == blocks.end() ||
        blocks.find(MathUtil::Vector3i(pair.first.x, pair.first.y, pair.first.z + 1)) == blocks.end()
      ) {
        result[Vector3(pair.first.y, pair.first.z, pair.first.x)] = pair.second->name.c_str();
      }
    }
  }
  return result;
}

Dictionary GodotLink::getFurnitures(Vector3 chunk_id) {
  #ifdef LOG
    log << "getFurnitures(" << chunk_id.x << ", " << chunk_id.y << ", " << chunk_id.z << ")" << std::endl;
  #endif
  Dictionary result = Dictionary();
  MathUtil::Vector3i coord = MathUtil::Vector3i(chunk_id.z, chunk_id.x, chunk_id.y);
  for(Furniture * furniture : link->getAdventure()->getWorld()->getFurnituresInChunk(coord)) {
    result[Vector3(furniture->getCoord().y, furniture->getCoord().z, furniture->getCoord().x)] = getDataFromFurniture(furniture);
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

Dictionary GodotLink::getUpdatedFurnitures() {
  #ifdef LOG
    log << "getUpdatedFurnitures()" << std::endl;
  #endif
  Dictionary result = Dictionary();
  for(Furniture * furniture : state->changed_furnitures) {
    result[Vector3(furniture->getCoord().y, furniture->getCoord().z, furniture->getCoord().x)] = getDataFromFurniture(furniture);
  }
  return result;
}

String GodotLink::getRelation(String team1, String team2) {
  #ifdef LOG
    log << "getRelation(" << team1.utf8().get_data() << ", " << team2.utf8().get_data() << ")" << std::endl;
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
  result["subtype"] = item->subtype;
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
    result["range"] = Vector3( ((WeaponItem *) item)->range.x, ((WeaponItem *) item)->range.y, ((WeaponItem *) item)->range.z);
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
    Dictionary content = Dictionary();
    for(ItemSlot * slot : ((ContainerItem *) item)->getItems()) {
      Vector2 vec = Vector2(slot->x, slot->y);
      content[vec] = getDataFromItem(slot->item);
    }
    result["content"] = content;
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
  result["orientation_z"] = block->orientation_z;
  result["speed"] = block->speed;
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
  result["baseStrength"] = attributes->baseDamageMult;
  result["baseThreshold"] = attributes->baseSoulBurn;
  result["baseFlow"] = attributes->baseFlow;
  result["baseTranscendence"] = 0;
  result["baseAttunement"] = 0;
  result["baseVisionRange"] = attributes->baseVisionRange;
  result["baseVisionPower"] = attributes->baseVisionPower;
  result["baseDetectionRange"] = attributes->baseDetectionRange;
  result["hpIncr"] = attributes->hpIncr;
  result["manaIncr"] = attributes->manaIncr;
  result["shieldIncr"] = attributes->shieldIncr;
  result["strengthIncr"] = attributes->damageMultIncr;
  result["thresholdIncr"] = attributes->soulBurnIncr;
  result["flowIncr"] = attributes->flowIncr;
  result["transcendenceIncr"] = 0;
  result["attunementIncr"] = 0;
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
  result["baseStrength"] = race->baseDamageMult;
  result["baseThreshold"] = race->baseSoulBurn;
  result["baseFlow"] = race->baseFlow;
  result["baseTranscendence"] = 0;
  result["baseAttunement"] = 0;
  result["baseVisionRange"] = race->baseVisionRange;
  result["baseVisionPower"] = race->baseVisionPower;
  result["baseDetectionRange"] = race->baseDetectionRange;
  result["hpIncr"] = race->hpIncr;
  result["manaIncr"] = race->manaIncr;
  result["shieldIncr"] = race->shieldIncr;
  result["strengthIncr"] = race->damageMultIncr;
  result["thresholdIncr"] = race->soulBurnIncr;
  result["flowIncr"] = race->flowIncr;
  result["transcendenceIncr"] = 0;
  result["attunementIncr"] = 0;
  result["size"] = Vector3(race->size.x, race->size.y, race->size.z);
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
  result["baseStrength"] = way->baseDamageMult;
  result["baseThreshold"] = way->baseSoulBurn;
  result["baseFlow"] = way->baseFlow;
  result["baseTranscendence"] = 0;
  result["baseAttunement"] = 0;
  result["baseVisionRange"] = way->baseVisionRange;
  result["baseVisionPower"] = way->baseVisionPower;
  result["baseDetectionRange"] = way->baseDetectionRange;
  result["hpIncr"] = way->hpIncr;
  result["manaIncr"] = way->manaIncr;
  result["shieldIncr"] = way->shieldIncr;
  result["strengthIncr"] = way->damageMultIncr;
  result["thresholdIncr"] = way->soulBurnIncr;
  result["flowIncr"] = way->flowIncr;
  result["transcendenceIncr"] = 0;
  result["attunementIncr"] = 0;
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
  result["coord"] = Vector3(character->x, character->y, character->z);
  result["speed"] = Vector3(character->vx, character->vy, character->vz);
  result["size"] = Vector3(character->sizeX, character->sizeY, character->sizeZ);
  result["orientation"] = Vector3(character->orientation_x, 0.F, character->orientation_z);
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
  result["threshold"] = character->getSoulBurnThreshold();
  result["hp"] = character->getHp();
  result["mana"] = character->getMana();
  result["shield"] = character->getShield();
  result["currentSoulBurn"] = character->getCurrentSoulBurn();
  result["strength"] = (int64_t) std::floor((character->getDamageMultiplier() - 1.F) * 100.F);
  result["flow"] = character->getFlow();
  result["transcendence"] = 0;
  result["attunement"] = 0;
  result["cloaking"] = character->cloakPower();
  result["visionRange"] = character->getVisionRange();
  result["visionPower"] = character->getVisionPower();
  result["detectionRange"] = character->getDetectionRange();
  result["hunger"] = character->getHunger();
  result["thirst"] = character->getThirst();
  result["stamina"] = character->getStamina();
  result["sanity"] = character->getSanity();
  //result["channeledMana"] = character->getChanneledMana();
  result["sizeX"] = character->getSize().x;
  result["sizeY"] = character->getSize().y;
  result["sizeZ"] = character->getSize().z;
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
  result["backup_weapon"] = getDataFromItem(gear->getBackupWeapon());
  result["bag"] = getDataFromItem(gear->getBag());
  Dictionary base_inventory = Dictionary();
  for(ItemSlot * slot : gear->getBaseInventory()->getItems()) {
    Vector2 vec = Vector2(slot->x, slot->y);
    base_inventory[vec] = getDataFromItem(slot->item);
  }
  result["base_inventory"] = base_inventory;
  Dictionary bag_content = Dictionary();
  if(gear->getBag() != nullptr) {
    for(ItemSlot * slot : gear->getBag()->getItems()) {
      Vector2 vec = Vector2(slot->x, slot->y);
      bag_content[vec] = getDataFromItem(slot->item);
    }
  }
  result["bag_content"] = bag_content;
  return result;
}

Dictionary GodotLink::getDataFromSkill(String skill_name) {
  #ifdef LOG
    log << "getDataFromSkill(" << skill_name.utf8().get_data() << ")" << std::endl;
  #endif
  Dictionary result = Dictionary();
  if(skill_name == "") {
    return result;
  }
  Skill * skill = (Skill *) link->getAdventure()->getDatabase()->getSkill(std::string(skill_name.utf8().get_data()));
  result["name"] = skill->name.c_str();
  result["level"] = (int64_t) skill->level;
  result["school"] = (int64_t) skill->school;
  result["target_type"] = (int64_t) skill->target_type;
  result["instant"] = skill->instant;
  result["blockable"] = skill->blockable;
  result["toggle"] = skill->toggle;
  result["overcharge_power_type"] = (int64_t) skill->overcharge_power_type;
  result["overcharge_duration_type"] = (int64_t) skill->overcharge_duration_type;
  result["overcharge_range_type"] = (int64_t) skill->overcharge_range_type;
  result["range"] = skill->range;
  result["time"] = skill->time;
  result["path"] = link->getAdventure()->getDatabase()->getSkillFile(skill->name).c_str();
  return result;
}

Array GodotLink::getSkillsFromCharacter() {
  #ifdef LOG
    log << "getSkillsFromCharacter()" << std::endl;
  #endif
  Array result = Array();
  for(Skill * skill : link->getPlayer()->getSkills()) {
    result.push_back(getDataFromSkill(skill->name.c_str()));
  }
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
  result["orientation_z"] = projectile->orientation_z;
  Array damages = Array();
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages.push_back(projectile->damages[i]);
  }
  result["damages"] = damages;
  result["speed"] = projectile->speed;
  result["area"] = projectile->area;
  result["waste_per_tick"] = projectile->waste_per_tick;
  result["waste_per_area"] = projectile->waste_per_area;
  result["pierce_power"] = projectile->pierce_power;
  return result;
}

Dictionary GodotLink::getDataFromFurniture(Furniture * furniture) {
  #ifdef LOG
    log << "getDataFromFurniture(" << furniture->name << ")" << std::endl;
  #endif
  Dictionary result = Dictionary();
  result["id"] = furniture->id;
  result["name"] = furniture->name.c_str();
  result["type"] = furniture->type;
  result["coord"] = Vector3(furniture->getCoord().x, furniture->getCoord().y, furniture->getCoord().z);
  result["size"] = Vector3(furniture->sizeX, furniture->sizeY, furniture->sizeZ);
  result["orientation_z"] = furniture->getOrientationZ();
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

Array GodotLink::getStartingAttributes() {
  #ifdef LOG
    log << "getStartingAttributes()" << std::endl;
  #endif
  Array result;
  for(Attributes * attributes : link->getAdventure()->getStartingAttributes()) {
    result.push_back(getDataFromClass(attributes->name.c_str()));
  }
  return result;
}

Array GodotLink::getStartingWays() {
  #ifdef LOG
    log << "getStartingWays()" << std::endl;
  #endif
  Array result;
  for(Way * way : link->getAdventure()->getStartingWays()) {
    result.push_back(getDataFromWay(way->name.c_str()));
  }
  return result;
}

Dictionary GodotLink::getItemSlot(int64_t item_id) {
  #ifdef LOG
    log << "getItemSlot(" << item_id << ")" << std::endl;
  #endif
  Dictionary result;
  for(ItemSlot * slot : link->getPlayer()->getGear()->getItems()) {
    if(slot->item->id == item_id) {
      result["id"] = link->getPlayer()->id;
      result["x"] = slot->x;
      result["y"] = slot->y;
      result["slot"] = slot->slot;
      return result;
    }
  }
}

void GodotLink::send_action(Dictionary action) {
  #ifdef LOG
    log << "send_action()" << std::endl;
  #endif
  int32_t type = (int32_t) action["type"];
  void * arg1 = 0;
  void * arg2 = 0;
  int32_t mana_cost = 1;
  switch(type) {
    case ACTION_IDLE:
    case ACTION_REST:
    case ACTION_RUN:
    case ACTION_JUMP:
    case ACTION_BREAKPOINT:
      break;
    case ACTION_MOVE: {
      Vector3 given = (Vector3) action["arg1"];
      MathUtil::Vector3 orientation = MathUtil::Vector3(given.x, given.y, given.z);
      #ifdef LOG
        log << "MOVE: " << orientation.x << " " << orientation.y << "" << orientation.z << std::endl;
      #endif
      arg1 = (void *) &orientation;
      break;
    }
    case ACTION_ACTIVATION: {
      Dictionary target_ori = action["arg1"];
      MathUtil::Target * target = new MathUtil::Target();
      target->type = (int32_t) target_ori["type"];
      target->next = nullptr;
      switch(target->type) {
        case TARGET_CHARACTER:
          target->id = target_ori["id"];
          break;
        case TARGET_FURNITURE:
        case TARGET_COORDINATES: {
          Vector3 pos = (Vector3) target_ori["pos"];
          target->coord = MathUtil::Vector3(pos.x, pos.y, pos.z);
          break;
        }
      }
      arg1 = (void *) target;
      break;
    }
    case ACTION_RELOAD:
    case ACTION_GRAB:
    case ACTION_USE_ITEM: {
      Dictionary slot_ori = action["arg1"];
      ItemSlot * slot = new ItemSlot();
      slot->id = (int64_t) slot_ori["id"];
      slot->x = (int32_t) slot_ori["x"];
      slot->y = (int32_t) slot_ori["y"];
      slot->slot = (int32_t) slot_ori["slot"];
      arg1 = (void *) slot;
      break;
    }
    case ACTION_SWAP_GEAR: {
      Dictionary slot1_ori = action["arg1"];
      ItemSlot * slot1 = new ItemSlot();
      slot1->id = (int64_t) slot1_ori["id"];
      slot1->x = (int32_t) slot1_ori["x"];
      slot1->y = (int32_t) slot1_ori["y"];
      slot1->slot = (int32_t) slot1_ori["slot"];
      arg1 = (void *) slot1;
      Dictionary slot2_ori = action["arg2"];
      ItemSlot * slot2 = new ItemSlot();
      slot2->id = (int64_t) slot2_ori["id"];
      slot2->x = (int32_t) slot2_ori["x"];
      slot2->y = (int32_t) slot2_ori["y"];
      slot2->slot = (int32_t) slot2_ori["slot"];
      arg2 = (void *) slot2;
      break;
    }
    case ACTION_USE_SKILL:
    case ACTION_ATTACK:
    case ACTION_BLOCK: {
      Dictionary target_ori = action["arg1"];
      MathUtil::Target * target = new MathUtil::Target();
      target->type = (int32_t) target_ori["type"];
      #ifdef LOG
        log << "target.type: " << target->type << std::endl;
      #endif
      if((int64_t) target_ori["id"] != 0) {
        target->character = link->getAdventure()->getCharacter((int64_t) target_ori["id"]);
        #ifdef LOG
          log << "target.id: " << target->character->id << std::endl;
        #endif
      }
      Vector3 pos = (Vector3) target_ori["pos"];
      target->coord = MathUtil::Vector3(pos.x, pos.y, pos.z);
      #ifdef LOG
        log << "target.pos_x: " << target->coord.x << std::endl;
        log << "target.pos_y: " << target->coord.y << std::endl;
        log << "target.pos_z: " << target->coord.z << std::endl;
      #endif
      target->next = nullptr;
      arg1 = (void *) target;
      Skill * skill = nullptr;
      if(type == ACTION_USE_SKILL) {
        skill = (Skill *) link->getAdventure()->getDatabase()->getSkill(std::string( ( (String) action["arg2"]).utf8().get_data()));
      }
      arg2 = (void *) skill;
      mana_cost = (int32_t) action["mana_cost"];
      #ifdef LOG
        log << "mana_cost: " << mana_cost << std::endl;
      #endif
      break;
    }
  }
  link->sendAction(type, arg1, arg2, mana_cost);
}

void GodotLink::pause() {
  #ifdef LOG
    log << "pause()" << std::endl;
  #endif
  link->sendPause();
}

void GodotLink::unpause() {
  #ifdef LOG
    log << "unpause()" << std::endl;
  #endif
  link->sendUnpause();
}

void GodotLink::close(bool shutdown) {
  #ifdef LOG
    log << "close()" << std::endl;
  #endif
  link->close(shutdown);
  if(link != nullptr) {
    delete link;
    link = nullptr;
  }
  if(state != nullptr) {
    delete state;
    state = nullptr;
  }
  #ifdef LOG
    log.close();
  #endif
}

void GodotLink::_bind_methods() {
  ClassDB::bind_method(D_METHOD("initialize", "ip", "port", "password"), &GodotLink::initialize);
  ClassDB::bind_method(D_METHOD("getTickRate"), &GodotLink::getTickRate);
  ClassDB::bind_method(D_METHOD("getIncompatible", "tocheck", "attributes", "race", "origin", "culture", "religion", "profession"), &GodotLink::getIncompatible);
  ClassDB::bind_method(D_METHOD("getEnglishFromKey", "key"), &GodotLink::getEnglishFromKey);
  ClassDB::bind_method(D_METHOD("sendChoices", "character", "attributes", "race", "origin", "culture", "religion", "profession"), &GodotLink::sendChoices);
  ClassDB::bind_method(D_METHOD("hasState"), &GodotLink::hasState);
  ClassDB::bind_method(D_METHOD("getState"), &GodotLink::getState);
  ClassDB::bind_method(D_METHOD("getMacros"), &GodotLink::getMacros);
  ClassDB::bind_method(D_METHOD("getTime"), &GodotLink::getTime);
  ClassDB::bind_method(D_METHOD("getClock", "terran_day"), &GodotLink::getClock);
  ClassDB::bind_method(D_METHOD("getLight"), &GodotLink::getLight);
  ClassDB::bind_method(D_METHOD("getBaseLight"), &GodotLink::getBaseLight);
  ClassDB::bind_method(D_METHOD("getMaxLight"), &GodotLink::getMaxLight);
  ClassDB::bind_method(D_METHOD("getOrientationToTarget", "a", "b"), &GodotLink::getOrientationToTarget);
  ClassDB::bind_method(D_METHOD("getAvaillableBlocks"), &GodotLink::getAvaillableBlocks);
  ClassDB::bind_method(D_METHOD("getChunkIds", "sizeZ", "radius"), &GodotLink::getChunkIds);
  ClassDB::bind_method(D_METHOD("getBlocks", "chunk_id"), &GodotLink::getBlocks);
  ClassDB::bind_method(D_METHOD("getFurnitures", "chunk_id"), &GodotLink::getFurnitures);
  ClassDB::bind_method(D_METHOD("getPlayerId"), &GodotLink::getPlayerId);
  ClassDB::bind_method(D_METHOD("getCharacters"), &GodotLink::getCharacters);
  ClassDB::bind_method(D_METHOD("getProjectiles"), &GodotLink::getProjectiles);
  ClassDB::bind_method(D_METHOD("getUpdatedFurnitures"), &GodotLink::getUpdatedFurnitures);
  ClassDB::bind_method(D_METHOD("getRelation", "team1", "team2"), &GodotLink::getRelation);
  ClassDB::bind_method(D_METHOD("getDataFromBlock", "block"), &GodotLink::getDataFromBlock);
  ClassDB::bind_method(D_METHOD("getDataFromClass", "class"), &GodotLink::getDataFromClass);
  ClassDB::bind_method(D_METHOD("getDataFromRace", "race"), &GodotLink::getDataFromRace);
  ClassDB::bind_method(D_METHOD("getDataFromWay", "way"), &GodotLink::getDataFromWay);
  ClassDB::bind_method(D_METHOD("getStatsFromCharacter"), &GodotLink::getStatsFromCharacter);
  ClassDB::bind_method(D_METHOD("getDataFromSkill", "skill"), &GodotLink::getDataFromSkill);
  ClassDB::bind_method(D_METHOD("getSkillsFromCharacter"), &GodotLink::getSkillsFromCharacter);
  ClassDB::bind_method(D_METHOD("getInventoryFromCharacter"), &GodotLink::getInventoryFromCharacter);
  ClassDB::bind_method(D_METHOD("getStartingAttributes"), &GodotLink::getStartingAttributes);
  ClassDB::bind_method(D_METHOD("getStartingWays"), &GodotLink::getStartingWays);
  ClassDB::bind_method(D_METHOD("getItemSlot", "item_id"), &GodotLink::getItemSlot);
  ClassDB::bind_method(D_METHOD("send_action", "action"), &GodotLink::send_action);
  ClassDB::bind_method(D_METHOD("pause"), &GodotLink::pause);
  ClassDB::bind_method(D_METHOD("unpause"), &GodotLink::unpause);
  ClassDB::bind_method(D_METHOD("close", "shutdown"), &GodotLink::close);
}
