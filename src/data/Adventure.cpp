#include "ai/AI.h"
#include "ai/PlayerAI.h"

#include "data/Action.h"
#include "data/Map.h"
#include "data/World.h"
#include "data/Tile.h"
#include "data/Character.h"
#include "data/Quest.h"
#include "data/Event.h"
#include "data/Save.h"
#include "data/Database.h"

#include "data/Adventure.h"

Save * Adventure::save() {
  return new Save(this);
}

void Adventure::softMoveCharacterToMap(Character * character, long map_id, long y, long x) {
  Map * map = world->getMap(map_id);
  long i = x;
  long j = y;
  bool conflict = false;
  int power = -1; // power = 0 in first loop turn
  int k = NORTH;
  while(conflict) {
    if(k % 8 == 0) {
      power++;
    }
    switch(k++ % 8 + 1) { // ORIENTATION
      case NORTH:
        j = y + power;
        break;
      case NORTH_EAST:
        j = y + power;
        i = x + power;
        break;
      case EAST:
        i = x + power;
        break;
      case SOUTH_EAST:
        j = y - power;
        i = x + power;
        break;
      case SOUTH:
        j = y - power;
        break;
      case SOUTH_WEST:
        j = y - power;
        i = x - power;
        break;
      case WEST:
        i = x - power;
        break;
      case NORTH_WEST:
        j = y + power;
        i = x - power;
        break;
    }
    if((conflict = map->getTile(j, i)->untraversable)) {
      continue;
    }
    for(auto c : map->getCharacters()) {
      if(c->getX() == i && c->getY() == j) {
        conflict = true;
        break;
      }
    }
  }
  world->getMap(character->getCurrentMapId())->removeCharacter(character);
  map->addCharacter(character);
  character->move(j, i);
  character->setCurrentMapId(map_id);
}

void Adventure::hardMoveCharacterToMap(Character * character, long map_id, long y, long x) {
  Map * map = world->getMap(map_id);
  for(auto c : map->getCharacters()) {
    if(c->getX() == x && c->getY() == y) {
      map->killCharacter(c, character);
    }
  }
  // TODO TOCHECK will probably break because c should already be nullptr
  for(auto c : party) {
    if(c->getX() == x && c->getY() == y) {
      delete c;
      removePlayer(c);
    }
  }
  if(map->getTile(y, x)->untraversable) {
    world->getMap(character->getCurrentMapId())->removeCharacter(character);
    delete character;
    return;
  }
  world->getMap(character->getCurrentMapId())->removeCharacter(character);
  map->addCharacter(character);
  character->move(y, x);
  character->setCurrentMapId(map_id);
}

void Adventure::addPlayer(Character * player) { party.push_back(player); }

void Adventure::removePlayer(Character * player) { party.remove(player); }

bool Adventure::isWiped() { return party.empty(); }

std::list<Character *> Adventure::getParty() { return party; }

std::list<Character *> Adventure::getPreservedPlayers() { return preserved_players; }

void Adventure::resurrect(Character * player, long map_id, long y, long x) {
  if(std::find(preserved_players.begin(), preserved_players.end(), player) != preserved_players.end()) {
    softMoveCharacterToMap(player, map_id, y, x);
  }
}

void Adventure::event() {
  for(auto e : events) {
    if(tick == e->tick) {
      e->activate(this);
    }
  }
}

long Adventure::getTick() { return tick; }
void Adventure::incrTick() { tick++; }
World * Adventure::getWorld() { return world; }
int Adventure::getLight() { return light; }
std::list<Attributes *> Adventure::getStartingAttributes() { return startingAttributes; }
std::list<Way *> Adventure::getStartingWays() { return startingWays; }
Database * Adventure::getDatabase() { return database; }
void Adventure::addQuest(Quest * quest) { quests.push_back(quest); }
void Adventure::removeQuest(Quest * quest) { quests.remove(quest); }
std::list<Character *> Adventure::getNPCs() {
  std::list<Character *> npcs = std::list<Character *>();
  for (auto pair : world->getMaps()) {
    for (Character * character : pair.second->getCharacters()) {
      // no check on player_character, because we want mind controlled players to act as npc
      // this imply that the player AI needs to send nullptr when asked for an Action
      // otherwise players will have 2 Actions per round
      npcs.push_back(character);
    }
  }
  return npcs;
}

std::list<Projectile *> Adventure::getProjectiles() {
  std::list<Projectile *> projectiles = std::list<Projectile *>();
  for (auto pair : world->getMaps()) {
    for (Projectile * projectile : pair.second->getProjectiles()) {
      projectiles.push_back(projectile);
    }
  }
  return projectiles;
}

void Adventure::applyDayLight() {
  for (auto pair : world->getMaps()) {
    if(pair.second->outside) {
      pair.second->applyDayLight(light);
    }
  }
}

void Adventure::incrDayLight() {
  if(tick % Settings::getLighDuration() == 0) {
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

std::string Adventure::getTime() {
  int year = (Settings::getStartingYear() * Settings::getYearDurationInTick() + tick) / Settings::getYearDurationInTick();
  int month = 1 + ((Settings::getStartingMonth() * Settings::getMonthDurationInTick() + tick) % Settings::getYearDurationInTick()) / Settings::getMonthDurationInTick();
  int week = 1 + ((Settings::getStartingWeek() * Settings::getWeekDurationInTick() + tick) % Settings::getMonthDurationInTick()) / Settings::getWeekDurationInTick();
  int day = 1 + ((Settings::getStartingDay() * Settings::getDayDurationInTick() + tick) % Settings::getWeekDurationInTick()) / Settings::getDayDurationInTick();
  int hour = ((Settings::getStartingHour() * Settings::getHourDurationInTick() + tick) % Settings::getDayDurationInTick()) / Settings::getHourDurationInTick();
  int minutes = Settings::getHourDuration() * ((float) (tick  % Settings::getHourDurationInTick())) / ( (float) Settings::getHourDurationInTick());
  return std::to_string(year) + std::string("|") + std::to_string(month) + std::string("|") + std::to_string(week)
  + std::string("|") + std::to_string(day) + std::string("|") + std::to_string(hour) + std::string("|") + std::to_string(minutes);
}

std::list<Action *> Adventure::getNPCsActions() {
  std::list<Action *> actions = std::list<Action *>();
  for(Character * npc : getNPCs()) {
    Action * action = npc->getAI()->getAction(this, npc);
    if(action != nullptr) {
      actions.push_back(action);
    }
  }
  return actions;
}

void Adventure::executeActions(std::list<Action *> actions) {
  for(Action * action : actions) {
    // the user might have been killed and deleted
    if(action != nullptr && action->getUser() != nullptr) {
      action->execute(this);
    }
    delete action;
  }
}

void Adventure::actAllProjectiles() {
  for (auto pair : world->getMaps()) {
    pair.second->actAllProjectiles(this);
  }
}
Character * Adventure::spawnPlayer(std::string name, Attributes * attr, Way * race, Way * origin, Way * culture, Way * religion, Way * profession) {
  Spawn * spawn = spawns.front();
  Character * player = new Character(
    database->getCharacter("TXT_PLAYER"),
    name,
    spawn->x,
    spawn->y,
    spawn->orientation,
    spawn->map_id,
    "party",
    new PlayerAI(),
    attr,
    race,
    origin,
    culture,
    religion,
    profession
  );
  world->getMap(spawn->map_id)->addCharacter(player);
  spawns.remove(spawn);
  delete spawn;
  return player;
}

void Adventure::applySoulBurn() {
  for(Character * c : getNPCs()) {
    c->applySoulBurn();
    if(!c->isAlive()) {
      getWorld()->getMap(c->getCurrentMapId())->killCharacter(c, c);
    }
  }
}

void Adventure::applyLevelUps() {
  for(Character * c : getNPCs()) {
    c->gainLevel();
  }
}
