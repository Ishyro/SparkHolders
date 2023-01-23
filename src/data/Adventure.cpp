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

void Adventure::softMoveCharacterToMap(Character * character, int map_id, int y, int x) {
  Map * map = world->getMap(map_id);
  bool conflict = true;
  int power = -1; // power = 0 in first loop turn
  int k = NORTH;
  int i;
  int j;
  while(conflict && (power <= map->sizeX || power <= map->sizeY)) {
    if(k % 8 == 0) {
      power++;
    }
    i = x;
    j = y;
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
    if(!(j >= 0 && j < map->sizeY) || !(i >= 0 && i < map->sizeX) || (conflict = map->getTile(j, i)->untraversable)) {
      continue;
    }
    for(Character * c : map->getCharacters()) {
      if(c->getX() == i && c->getY() == j) {
        conflict = true;
        break;
      }
    }
  }
  world->getMap(character->getCurrentMapId())->removeCharacter(character);
  for(Projectile * p : world->getMap(character->getCurrentMapId())->getProjectiles()) {
    if(p->getTarget() == character && !p->homing) {
      p->setLost(true);
    }
  }
  map->addCharacter(character);
  character->move(j, i, 0.5F, 0.5F, character->getOrientation());
  character->setCurrentMapId(map_id);
}

void Adventure::hardMoveCharacterToMap(Character * character, int map_id, int y, int x) {
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
  if(map->getTile(y, x)->untraversable) {
    world->getMap(character->getCurrentMapId())->removeCharacter(character);
    delete character;
    return;
  }
  world->getMap(character->getCurrentMapId())->removeCharacter(character);
  for(Projectile * p : world->getMap(character->getCurrentMapId())->getProjectiles()) {
    if(p->getTarget() == character && !p->homing) {
      p->setLost(true);
    }
  }
  map->addCharacter(character);
  character->move(y, x, 0.5F, 0.5F, character->getOrientation());
  character->setCurrentMapId(map_id);
}

void Adventure::addPlayer(Character * player) { party.push_back(player); }

void Adventure::removePlayer(Character * player) { party.remove(player); }

bool Adventure::isWiped() { return party.empty(); }

std::list<Character *> Adventure::getParty() { return party; }

std::list<Character *> Adventure::getPreservedPlayers() { return preserved_players; }

void Adventure::resurrect(Character * player, int map_id, int y, int x) {
  if(std::find(preserved_players.begin(), preserved_players.end(), player) != preserved_players.end()) {
    softMoveCharacterToMap(player, map_id, y, x);
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
void Adventure::incrRound() { round++; }
World * Adventure::getWorld() { return world; }
int Adventure::getLight() { return light; }
std::list<Attributes *> Adventure::getStartingAttributes() { return startingAttributes; }
std::list<Way *> Adventure::getStartingWays() { return startingWays; }
Database * Adventure::getDatabase() { return database; }
void Adventure::addQuest(Quest * quest) { quests.push_back(quest); }
void Adventure::removeQuest(Quest * quest) { quests.remove(quest); }
std::list<Character *> Adventure::getCharacters() {
  std::list<Character *> characters = std::list<Character *>();
  for (auto pair : world->getMaps()) {
    for (Character * character : pair.second->getCharacters()) {
      // no check on player_character, because we want mind controlled players to act as npc
      // this imply that the player AI needs to send nullptr when asked for an Action
      // otherwise players will have 2 Actions per round
      characters.push_back(character);
    }
  }
  return characters;
}

Character * Adventure::getCharacter(long id) {
  if(id != 0) {
    for(Character * c : getCharacters()) {
      if(id == c->id) {
        return c;
      }
    }
  }
  return nullptr;
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

std::string Adventure::getTime() {
  int year = (Settings::getStartingYear() * Settings::getYearDurationInRound() + round) / Settings::getYearDurationInRound();
  int month = 1 + (((Settings::getStartingMonth() - 1) * Settings::getMonthDurationInRound() + round) % Settings::getYearDurationInRound()) / Settings::getMonthDurationInRound();
  int week = 1 + (((Settings::getStartingWeek() - 1) * Settings::getWeekDurationInRound() + round) % Settings::getMonthDurationInRound()) / Settings::getWeekDurationInRound();
  int day = 1 + (((Settings::getStartingDay() - 1) * Settings::getDayDurationInRound() + round) % Settings::getWeekDurationInRound()) / Settings::getDayDurationInRound();
  int hour = ((Settings::getStartingHour() * Settings::getHourDurationInRound() + round) % Settings::getDayDurationInRound()) / Settings::getHourDurationInRound();
  int minutes = Settings::getHourDuration() * ((float) (round  % Settings::getHourDurationInRound())) / ( (float) Settings::getHourDurationInRound());
  int charHoursSize = std::to_string(Settings::getDayDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
  int charMinutesSize = std::to_string(Settings::getHourDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
  std::string result = std::to_string(year) + std::string("|") + std::to_string(month) + std::string("|")
  + std::to_string(week) + std::string("|") + std::to_string(day) + std::string("|");
  std::string hour_str = std::to_string(hour) + std::string("|");
  while(hour_str.length() - charHoursSize - 1 > 0) {
    hour_str = std::to_string(0) + hour_str;
  }
  result += hour_str;
  std::string minutes_str = std::to_string(minutes) + std::string("|");
  while(minutes_str.length() - charMinutesSize - 1 > 0) {
    minutes_str = std::to_string(0) + minutes_str;
  }
  result += minutes_str;
  return result;
}

std::list<Action *> Adventure::getNPCsActions() {
  std::list<Action *> actions = std::list<Action *>();
  for(Character * npc : getCharacters()) {
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
    action = nullptr;
  }
}

void Adventure::actAllProjectiles() {
  for (auto pair : world->getMaps()) {
    pair.second->actAllProjectiles(this);
  }
}
Character * Adventure::spawnPlayer(std::string name, Attributes * attr, Way * race, Way * origin, Way * culture, Way * religion, Way * profession) {
  Spawn * spawn = spawns.front();
  spawns.remove(spawn);
  std::list<Way *> * titles = new std::list<Way *>();
  Character * player = new Character(
    database->getCharacter("TXT_PLAYER"),
    name,
    0,
    spawn->x,
    spawn->y,
    spawn->orientation,
    spawn->map_id,
    "party",
    new PlayerAI(),
    attr,
    (Attributes *) database->getAttributes("TXT_NO_ATTRIBUTES"),
    race,
    origin,
    culture,
    religion,
    profession,
    *titles
  );
  world->getMap(spawn->map_id)->addCharacter(player);
  delete spawn;
  delete titles;
  return player;
}

void Adventure::applyRoundIteration() {
  for(Character * c : getCharacters()) {
    c->gainLevel();
    c->applyEffects();
    // might have been killed by an effect
    if(c != nullptr) {
      c->applyTiredness();
      c->applyHunger();
      c->applySoulBurn();
      if(!c->isAlive()) {
        getWorld()->getMap(c->getCurrentMapId())->killCharacter(c, c);
      }
    }
  }
}
