#include "ai/AI.h"
#include "ai/PlayerAI.h"

#include "data/actions/Action.h"

#include "data/World.h"
#include "data/Region.h"
#include "data/Block.h"
#include "data/Character.h"
#include "data/furnitures/Furniture.h"
#include "data/furnitures/SwitchFurniture.h"
#include "data/Quest.h"
#include "data/Event.h"
#include "data/Save.h"
#include "data/Database.h"

#include "communication/SpeechManager.h"

#include "data/Adventure.h"

#include "util/String.h"

Save * Adventure::save() {
  return new Save(this);
}

void Adventure::softMoveCharacterToMap(Character * character, MathUtil::Vector3 coord) {
  /*
  for(Projectile * p : world->getMap(character->getCurrentMap()->id)->getProjectiles()) {
    if(p->getTarget()->type == TARGET_CHARACTER && p->getTarget()->id == character->id && !p->homing) {
      p->setLost(true);
    }
    // recalculate path since the homing projectile always knows where is its target 
    else if(p->homing) {
      p->setOrientationZ(world->setPathToTarget(p->getCurrentMapId(), p->getX(), p->getY(), p->getTarget()));
    }
  }
  */
  character->move(coord, character->getOrientation(), world);
}

void Adventure::hardMoveCharacterToMap(Character * character, MathUtil::Vector3 coord) {
  /*
  Map * map = world->getMap(map_id);
  for(Character * c : map->getCharacters()) {
    if(c->getX() == x && c->getY() == y) {
      map->killCharacter(c, character);
    }
  }
  // TODO TOCHECK will probably break because c should already be nullptr
  for(Character * c : party) {
    if(c->getX() == x && c->getY() == y) {
      delete c;
      removePlayer(c);
    }
  }
  if(map->getBlock(x, y)->unwalkable) {
    world->getMap(character->getCurrentMap()->id)->removeCharacter(character);
    delete character;
    return;
  }
  world->getMap(character->getCurrentMap()->id)->removeCharacter(character);
  for(Projectile * p : world->getMap(character->getCurrentMap()->id)->getProjectiles()) {
    if(p->getTarget()->type == TARGET_CHARACTER && p->getTarget()->id == character->id && !p->homing) {
      p->setLost(true);
    }
    // recalculate path since the homing projectile always knows where is its target 
    else if(p->homing) {
      p->setOrientationZ(world->setPathToTarget(p->getCurrentMapId(), p->getX(), p->getY(), p->getTarget()));
    }
  }
  map->addCharacter(character);
  character->move(x, y, z, character->getOrientation().z, world);
  */
}

void Adventure::addPlayer(Character * player) { party.push_back(player); }

void Adventure::removePlayer(Character * player) { party.remove(player); }

bool Adventure::isWiped() { return party.empty(); }

std::list<Character *> Adventure::getParty() { return party; }

std::list<Character *> Adventure::getPreservedPlayers() { return preserved_players; }

void Adventure::resurrect(Character * player, MathUtil::Vector3 coord) {
  if(std::find(preserved_players.begin(), preserved_players.end(), player) != preserved_players.end()) {
    softMoveCharacterToMap(player, coord);
  }
}

void Adventure::event() {
  for(Event * e : events) {
    if(round == e->round) {
      e->activate(this);
    }
  }
}

int64_t Adventure::getRound() { return round; }
float Adventure::getTick() { return tick; }
void Adventure::incrTick() {
  tick += Settings::getTickDurationInSeconds();
  if(tick >= Settings::getMinuteDuration()) {
    tick -= Settings::getMinuteDuration();
    round++;
  }
}

World * Adventure::getWorld() { return world; }
std::list<Attributes *> Adventure::getStartingAttributes() { return startingAttributes; }
std::list<Way *> Adventure::getStartingWays() { return startingWays; }
Database * Adventure::getDatabase() { return database; }
void Adventure::addQuest(Quest * quest) { quests.push_back(quest); }
void Adventure::removeQuest(Quest * quest) { quests.remove(quest); }
std::list<Character *> Adventure::getCharacters() {
  return world->getCharacters();
}

Character * Adventure::getCharacter(int64_t id) {
  return world->getCharacter(id);
}

Furniture * Adventure::getFurniture(MathUtil::Vector3i furniture_coord) {
  return world->getFurniture(furniture_coord);
}

void Adventure::actAllProjectiles() {
  std::list<Projectile *> projectiles = std::list<Projectile *>();
  /*
  for(Map * map : world->getMaps()) {
    for(Projectile * p : map->getProjectiles()) {
      projectiles.push_back(p);
    }
    map->clearProjectiles();
  }
  for(Projectile * p : projectiles) {
    bool keep = true;
    float speed = p->getSpeed();
    Map * current_map = new Map(world->getMap(p->getCurrentMapId()), p, database, world);
    while(keep && speed > 0.F) {
      speed = current_map->actProjectile(p, this, speed);
      if(p->noDamage()) {
        keep = false;
        delete p;
      }
    }
    delete current_map;
    if(keep) {
      world->getMap(p->getX(), p->getY(), p->getZ())->addProjectile(p);
    }
  }
  */
}

Character * Adventure::spawnPlayer(std::string name, Attributes * attr, Race * race, Way * origin, Way * culture, Way * religion, Way * profession) {
  Spawn * spawn = spawns.front();
  spawns.remove(spawn);
  std::list<Race *> * race_modifiers = new std::list<Race *>();
  std::list<Way *> * titles = new std::list<Way *>();
  Character * player = new Character(
    database->getCharacter("TXT_PLAYER"),
    name,
    0,
    50,
    spawn->coord.x,
    spawn->coord.y,
    spawn->coord.z,
    0.F,
    90.F,
    nullptr,
    "party",
    new PlayerAI(),
    attr,
    (Attributes *) database->getAttributes("TXT_NO_ATTRIBUTES"),
    (Attributes *) database->getAttributes("TXT_NO_ATTRIBUTES"),
    (Gear *) database->getGear("TXT_LIGHT_STARTING_GEAR"),
    race,
    *race_modifiers,
    origin,
    culture,
    religion,
    profession,
    *titles
  );
  world->addCharacter(player);
  for(std::pair<const MathUtil::Vector3i, Block *> pair : player->getRegion()->getBlocks()) {
    // TODO
    ;
  }
  delete race_modifiers;
  delete titles;
  return player;
}

void Adventure::applyIteration() {
  std::list<Character *> to_remove = std::list<Character *>();
  for(Character * c : getCharacters()) {
    if(c != nullptr && !c->isMarkedDead()) {
      c->gainLevel();
      c->applyEffects();
      c->applyBodyNeeds();
      c->applySoulNeeds();
      c->applySpiritNeeds();
      c->applySoulBurn();
      c->getRegion()->move(c, world);
      Environment e;
      c->hungerStep(e);
      c->thirstStep(e);
      c->staminaStep(e);
      c->sanityStep(e);
      Action * action = c->getAction();
      if(action != nullptr) {
        if(action->getTick() <= 1.F) {
          action->execute(this);
          delete action;
          action = nullptr;
        }
        else {
          action->computeTick(1);
        }
      }
      action = c->getLegAction();
      if(action != nullptr) {
        if(action->getTick() <= 1.F) {
          action->execute(this);
          delete action;
          action = nullptr;
        }
        else {
          action->computeTick(1);
        }
      }
    }
    if(!c->isAlive()) {
      to_remove.push_back(c);
    }
  }
  for(Character * c : to_remove) {
    world->eraseCharacter(c);
  }
}

Time Adventure::getTime() {
  Time time = Time();
  time.year = Settings::getStartingYear() + round / Settings::getYearDurationInRound();
  time.month = 1 + (((Settings::getStartingMonth() - 1) * Settings::getMonthDurationInRound() + round) % Settings::getYearDurationInRound()) / Settings::getMonthDurationInRound();
  time.week = ((Settings::getWeekDurationInRound() + round) % Settings::getMonthDurationInRound()) / Settings::getWeekDurationInRound();
  time.day = 1 + (((Settings::getStartingDay() - 1) * Settings::getDayDurationInRound() + round) % Settings::getMonthDurationInRound()) / Settings::getDayDurationInRound();
  time.hour = ((Settings::getStartingHour() * Settings::getHourDurationInRound() + round) % Settings::getDayDurationInRound()) / Settings::getHourDurationInRound();
  time.minutes = Settings::getHourDuration() * ((float) (round % Settings::getHourDurationInRound())) / ( (float) Settings::getHourDurationInRound());
  time.seconds = (int32_t) std::floor(tick);
  return time;
}

int32_t Adventure::getLight(MathUtil::Vector3i coord) {
  int32_t base_light = MathUtil::getLight(MathUtil::getCoords(coord), getTime());
  switch(world->getLightening(coord)) {
    case LIGHTENING_INDOORS:
      return base_light / 3600;
    case LIGHTENING_OUTDOORS:
      return base_light;
    case LIGHTENING_DARK:
      return 0;
    default:
      return 0;
  }
}

int32_t Adventure::getLight(MathUtil::Vector3 coord) {
  return getLight(MathUtil::Vector3i(coord));
}

std::string Adventure::state_to_string(Character * player) {
  std::stringstream * ss = new std::stringstream();
  std::stringstream * ss_blocks = new std::stringstream();
  std::stringstream * ss_characters = new std::stringstream();
  std::stringstream * ss_projectiles = new std::stringstream();
  std::stringstream * ss_loots = new std::stringstream();
  std::stringstream * ss_furnitures = new std::stringstream();
  std::stringstream * ss_speeches = new std::stringstream();
  String::insert_long(ss, round);
  String::insert_float(ss, tick);
  Region * region = player->getRegion();
  /*
  Map * map = world->getMap(player->getX(), player->getY(), player->getZ());
  Map * baseMap = (Map *) database->getMap(map->baseName);
  Map * visionMap = new Map(map, player, database, world);
  player->setCurrentMap(visionMap);
  for(int32_t y = visionMap->offsetY; y < map->sizeY + visionMap->offsetY; y++) {
    for(int32_t x = visionMap->offsetX; x < map->sizeX + visionMap->offsetX; x++) {
      if(visionMap->getBlock(x, y) != nullptr && visionMap->getBlock(x, y)->name != "TXT_MIST" &&
        world->getBlock(x, y, visionMap->offsetZ) != baseMap->getBlock(x + map->offsetX, y + map->offsetY)) {
        // TODO
        // String::insert(ss_blocks, map->tile_to_string(y, x));
      }
    }
  }
  */
  for(Character * character : region->getCharacters(player, world)) {
    String::insert(ss_characters, character->to_string());
    String::insert_int(ss_characters, database->getRelation(character->getTeam(), player->getTeam()));
  }
  /*
  for(Projectile * projectile : region->getProjectiles()) {
    String::insert(ss_projectiles, projectile->to_string());
  }
  for(Loot * loot : region->getLoots()) {
    String::insert_int(ss_loots, loot->type);
    String::insert_float(ss_loots, loot->x);
    String::insert_float(ss_loots, loot->y);
    String::insert_float(ss_loots, loot->size);
  }
  */
  for(Furniture * furniture : region->getFurnitures(player, world)) {
    if(furniture->type == FURNITURE_SWITCH) {
      String::insert_long(ss_furnitures, furniture->getCoord().x);
      String::insert_long(ss_furnitures, furniture->getCoord().y);
      String::insert_long(ss_furnitures, furniture->getCoord().z);
      String::insert_bool(ss_furnitures, ( (SwitchFurniture *) furniture)->getIsOn());
    }
  }
  String::insert(ss, ss_blocks->str());
  delete ss_blocks;
  String::insert(ss, ss_characters->str());
  delete ss_characters;
  String::insert(ss, ss_projectiles->str());
  delete ss_projectiles;
  String::insert(ss, ss_loots->str());
  delete ss_loots;
  String::insert(ss, ss_furnitures->str());
  delete ss_furnitures;
  for(Speech * speech : SpeechManager::get()) {
    String::insert(ss_speeches, speech->to_string());
  }
  String::insert(ss, ss_speeches->str());
  delete ss_speeches;
  std::string result = ss->str();
  delete ss;
  return result;
}

StateDisplay * Adventure::update_state(std::string to_read) {
  StateDisplay * display = new StateDisplay();
  std::stringstream * ss = new std::stringstream(to_read);
  round = String::extract_long(ss);
  tick = String::extract_float(ss);
  std::stringstream * ss_blocks = new std::stringstream(String::extract(ss));
  while(ss_blocks->rdbuf()->in_avail() != 0) {
    std::stringstream * ss_block = new std::stringstream(String::extract(ss_blocks));
    int32_t x = String::extract_int(ss_block);
    int32_t y = String::extract_int(ss_block);
    int32_t z = String::extract_int(ss_block);
    world->setBlock(MathUtil::Vector3i(x, y, z), (Block *) database->getBlock(String::extract(ss_block)));
    delete ss_block;
  }
  delete ss_blocks;
  std::stringstream * ss_characters = new std::stringstream(String::extract(ss));
  while(ss_characters->rdbuf()->in_avail() != 0) {
    CharacterDisplay * characterDisplay = Character::from_string(String::extract(ss_characters));
    characterDisplay->teamRelation = String::extract_int(ss_characters);
    display->characters.push_back(characterDisplay);
  }
  delete ss_characters;
  std::stringstream * ss_projectiles = new std::stringstream(String::extract(ss));
  while(ss_projectiles->rdbuf()->in_avail() != 0) {
    display->projectiles.push_back(Projectile::from_string(String::extract(ss_projectiles)));
  }
  delete ss_projectiles;
  std::stringstream * ss_loots = new std::stringstream(String::extract(ss));
  while(ss_loots->rdbuf()->in_avail() != 0) {
    Loot * loot = new Loot();
    loot->type = String::extract_int(ss_loots);
    loot->x = String::extract_float(ss_loots);
    loot->y = String::extract_float(ss_loots);
    loot->size = String::extract_float(ss_loots);
    display->loots.push_back(loot);
  }
  delete ss_loots;
  std::stringstream * ss_furnitures = new std::stringstream(String::extract(ss));
  while(ss_furnitures->rdbuf()->in_avail() != 0) {
    MathUtil::Vector3i furniture_coord = MathUtil::Vector3i();
    furniture_coord.x = String::extract_long(ss_furnitures);
    furniture_coord.y = String::extract_long(ss_furnitures);
    furniture_coord.z = String::extract_long(ss_furnitures);
    SwitchFurniture * furniture = (SwitchFurniture *) getFurniture(furniture_coord);
    bool isOn = String::extract_bool(ss_furnitures);
    if(furniture->getIsOn() != isOn) {
      furniture->setIsOn(isOn);
      display->changed_furnitures.push_back(furniture);
    }
  }
  delete ss_furnitures;
  std::stringstream * ss_speeches = new std::stringstream(String::extract(ss));
  while(ss_speeches->rdbuf()->in_avail() != 0) {
    display->speeches.push_back(Speech::from_string(String::extract(ss_speeches)));
  }
  delete ss_speeches;
  delete ss;
  return display;
}
