#include "ai/AI.h"
#include "ai/PlayerAI.h"

#include "data/actions/Action.h"

#include "data/Map.h"
#include "data/World.h"
#include "data/Tile.h"
#include "data/Character.h"
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

void Adventure::softMoveCharacterToMap(Character * character, int map_id, float x, float y, float z) {
  Map * map = world->getMap(map_id);
  world->getMap(character->getCurrentMap()->id)->removeCharacter(character);
  for(Projectile * p : world->getMap(character->getCurrentMap()->id)->getProjectiles()) {
    if(p->getTarget()->type == TARGET_CHARACTER && p->getTarget()->id == character->id && !p->homing) {
      p->setLost(true);
    }
    // recalculate path since the homing projectile always knows where is its target 
    else if(p->homing) {
      p->setOrientation(world->setPathToTarget(p->getCurrentMapId(), p->getX(), p->getY(), p->getTarget()));
    }
  }
  map->addCharacter(character);
  character->move(x, y, z, character->getOrientation(), world);
}

void Adventure::hardMoveCharacterToMap(Character * character, int map_id, float x, float y, float z) {
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
  if(map->getTile(x, y)->untraversable) {
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
      p->setOrientation(world->setPathToTarget(p->getCurrentMapId(), p->getX(), p->getY(), p->getTarget()));
    }
  }
  map->addCharacter(character);
  character->move(x, y, z, character->getOrientation(), world);
}

void Adventure::addPlayer(Character * player) { party.push_back(player); }

void Adventure::removePlayer(Character * player) { party.remove(player); }

bool Adventure::isWiped() { return party.empty(); }

std::list<Character *> Adventure::getParty() { return party; }

std::list<Character *> Adventure::getPreservedPlayers() { return preserved_players; }

void Adventure::resurrect(Character * player, int map_id, float x, float y, float z) {
  if(std::find(preserved_players.begin(), preserved_players.end(), player) != preserved_players.end()) {
    softMoveCharacterToMap(player, map_id, x, y, z);
  }
}

void Adventure::event() {
  for(Event * e : events) {
    if(round == e->round) {
      e->activate(this);
    }
  }
}

long Adventure::getRound() { return round; }
int Adventure::getTick() { return tick; }
void Adventure::incrTick() {
  if( (++tick) == Settings::getMinuteDuration()) {
    tick = 0;
    round++;
    incrDayLight();
  }
}

World * Adventure::getWorld() { return world; }
int Adventure::getLight() { return light; }
std::list<Attributes *> Adventure::getStartingAttributes() { return startingAttributes; }
std::list<Way *> Adventure::getStartingWays() { return startingWays; }
Database * Adventure::getDatabase() { return database; }
void Adventure::addQuest(Quest * quest) { quests.push_back(quest); }
void Adventure::removeQuest(Quest * quest) { quests.remove(quest); }
std::list<Character *> Adventure::getCharacters() {
  return world->getCharacters();
}

Character * Adventure::getCharacter(long id) {
  return world->getCharacter(id);
}

std::list<Projectile *> Adventure::getProjectiles() {
  std::list<Projectile *> projectiles = std::list<Projectile *>();
  for(Map * map : world->getMaps()) {
    for(Projectile * projectile : map->getProjectiles()) {
      projectiles.push_back(projectile);
    }
  }
  return projectiles;
}

void Adventure::applyDayLight() {
  for(Map * map : world->getMaps()) {
    if(map->outside) {
      map->applyDayLight(light);
    }
  }
}

void Adventure::incrDayLight() {
  if(round % Settings::getLighDuration() == 0) {
    lightUp ? light++ : light--;
    if(light == Settings::getLightMaxPower()) {
      lightUp = false;
    }
    if(light == 0) {
      lightUp = true;
    }
    applyDayLight();
  }
}

void Adventure::getNPCsActions() {
  for(Character * npc : getCharacters()) {
    if(!npc->player_character && npc->getNeedToUpdateActions()) {
      Action * action = npc->getAI()->getActions(this, npc);
      action->computeTime(this);
      if(action != nullptr) {
        Action * to_remove = nullptr;
        for(Action * a : actions) {
          if(a->getUser() == npc) {
            to_remove = a;
            break;
          }
        }
        if(to_remove != nullptr) {
          actions.remove(to_remove);
          delete to_remove;
        }
        actions.push_back(action);
        npc->setNeedToUpdateActions(false);
      }
    }
  }
}

void Adventure::mergeActions(std::list<Action *> to_add) {
  to_add.sort();
  actions.merge(to_add);
}

void Adventure::executeActions() {
  std::list<Action *> next_actions;
  for(Action * action : actions) {
    // the user might have been killed and deleted
    if(action != nullptr && action->getTick() <= 1.F && action->getUser() != nullptr) {
      Action * next = action->execute(this);
      if(next != nullptr) {
        next_actions.push_back(next);
      }
      delete action;
      action = nullptr;
    }
    else {
      action->computeTick(1);
      next_actions.push_back(action);
    }
  }
  actions.clear();
  actions = next_actions;
  actions.sort();
}

void Adventure::actAllProjectiles() {
  std::list<Projectile *> projectiles = std::list<Projectile *>();
  for(Map * map : world->getMaps()) {
    for(Projectile * p : map->getProjectiles()) {
      projectiles.push_back(p);
    }
    map->clearProjectiles();
  }
  for(Projectile * p : projectiles) {
    bool keep = true;
    float speed = p->getSpeed();
    while(keep && speed > 0.F) {
      speed = world->getMap(p->getCurrentMapId())->actProjectile(p, this, speed);
      if(p->noDamage()) {
        keep = false;
        delete p;
      }
    }
    if(keep) {
      world->getMap(p->getCurrentMapId())->addProjectile(p);
    }
  }
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
    spawn->x,
    spawn->y,
    spawn->z,
    90.F,
    world->getMap(spawn->y, spawn->x, spawn->z),
    "party",
    new PlayerAI(),
    attr,
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
  world->getMap(spawn->y, spawn->x, spawn->z)->addCharacter(player);
  delete spawn;
  delete race_modifiers;
  delete titles;
  return player;
}

void Adventure::applyIteration() {
  for(Character * c : getCharacters()) {
    c->gainLevel();
    c->applyEffects();
    // might have been killed by an effect
    if(c != nullptr) {
      c->applySoulBurn();
      if(tick == 0) {
        c->applyTiredness();
        c->applyHunger();
      }
      if(!c->isAlive()) {
        getWorld()->getMap(c->getCurrentMap()->id)->killCharacter(c, c);
      }
    }
  }
}

std::string Adventure::getTime() {
  int year = (Settings::getStartingYear() * Settings::getYearDurationInRound() + round) / Settings::getYearDurationInRound();
  int month = 1 + (((Settings::getStartingMonth() - 1) * Settings::getMonthDurationInRound() + round) % Settings::getYearDurationInRound()) / Settings::getMonthDurationInRound();
  int week = 1 + (((Settings::getStartingWeek() - 1) * Settings::getWeekDurationInRound() + round) % Settings::getMonthDurationInRound()) / Settings::getWeekDurationInRound();
  int day = 1 + (((Settings::getStartingDay() - 1) * Settings::getDayDurationInRound() + round) % Settings::getWeekDurationInRound()) / Settings::getDayDurationInRound();
  int hour = ((Settings::getStartingHour() * Settings::getHourDurationInRound() + round) % Settings::getDayDurationInRound()) / Settings::getHourDurationInRound();
  int minutes = Settings::getHourDuration() * ((float) (round  % Settings::getHourDurationInRound())) / ( (float) Settings::getHourDurationInRound());
  int charHoursSize = std::to_string(Settings::getDayDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
  int charMinutesSize = std::to_string(Settings::getHourDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
  int charSecondsSize = std::to_string(Settings::getMinuteDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
  std::string hour_str = std::to_string(hour);
  while(hour_str.length() - charHoursSize > 0) {
    hour_str = std::to_string(0) + hour_str;
  }
  std::string minutes_str = std::to_string(minutes);
  while(minutes_str.length() - charMinutesSize > 0) {
    minutes_str = std::to_string(0) + minutes_str;
  }
  std::string seconds_str = std::to_string(tick);
  while(seconds_str.length() - charSecondsSize > 0) {
    seconds_str = std::to_string(0) + seconds_str;
  }
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, year);
  String::insert_int(ss, month);
  String::insert_int(ss, week);
  String::insert_int(ss, day);
  String::insert(ss, hour_str);
  String::insert(ss, minutes_str);
  String::insert(ss, seconds_str);
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Adventure::state_to_string(Character * c) {
  std::stringstream * ss = new std::stringstream();
  String::insert_long(ss, round);
  String::insert_int(ss, tick);
  String::insert_int(ss, light);
  Map * map = world->getMap(c->getX(), c->getY(), c->getZ());
  String::insert_long(ss, map->id);
  Map * baseMap = (Map *) database->getMap(map->baseName);
  Map * visionMap = new Map(map, c, database, world);
  c->setCurrentMap(visionMap);
  std::stringstream * ss_tiles = new std::stringstream();
  for(int y = visionMap->offsetY; y < map->sizeY + visionMap->offsetY; y++) {
    for(int x = visionMap->offsetX; x < map->sizeX + visionMap->offsetX; x++) {
      if(visionMap->getTile(x, y) != nullptr && visionMap->getTile(x, y)->name != "TXT_MIST" &&
        world->getTile(x, y, visionMap->offsetZ) != baseMap->getTile(x + map->offsetX, y + map->offsetY)) {
        // TODO
        // String::insert(ss_tiles, map->tile_to_string(y, x));
      }
    }
  }
  String::insert(ss, ss_tiles->str());
  delete ss_tiles;
  std::stringstream * ss_characters = new std::stringstream();
  for(Character * character : visionMap->getCharacters()) {
    String::insert(ss_characters, character->to_string());
    String::insert_int(ss_characters, database->getRelation(character->getTeam(), c->getTeam()));
  }
  String::insert(ss, ss_characters->str());
  delete ss_characters;
  std::stringstream * ss_projectiles = new std::stringstream();
  for(Projectile * projectile : visionMap->getProjectiles()) {
    String::insert(ss_projectiles, projectile->to_string());
  }
  String::insert(ss, ss_projectiles->str());
  delete ss_projectiles;
  std::stringstream * ss_loots = new std::stringstream();
  for(Loot * loot : visionMap->getLoots()) {
    String::insert_int(ss_loots, loot->type);
    String::insert_float(ss_loots, loot->x);
    String::insert_float(ss_loots, loot->y);
    String::insert_float(ss_loots, loot->size);
  }
  String::insert(ss, ss_loots->str());
  delete ss_loots;
  std::stringstream * ss_speeches = new std::stringstream();
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
  tick = String::extract_int(ss);
  light = String::extract_int(ss);
  applyDayLight();
  display->map_id = String::extract_long(ss);
  std::stringstream * ss_tiles = new std::stringstream(String::extract(ss));
  while(ss_tiles->rdbuf()->in_avail() != 0) {
    std::stringstream * ss_tile = new std::stringstream(String::extract(ss_tiles));
    long id = String::extract_long(ss_tile);
    int x = String::extract_int(ss_tile);
    int y = String::extract_int(ss_tile);
    world->getMap(id)->setTile(x, y, (Tile *) database->getTile(String::extract(ss_tile)));
    delete ss_tile;
  }
  delete ss_tiles;
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
  std::stringstream * ss_speeches = new std::stringstream(String::extract(ss));
  while(ss_speeches->rdbuf()->in_avail() != 0) {
    display->speeches.push_back(Speech::from_string(String::extract(ss_speeches)));
  }
  delete ss_speeches;
  delete ss;
  return display;
}
