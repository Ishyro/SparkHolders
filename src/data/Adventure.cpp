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

#include "utils/String.h"

Save * Adventure::save() {
  return new Save(this);
}

void Adventure::softMoveCharacterToMap(Character * character, int map_id, int y, int x) {
  Map * map = world->getMap(map_id);
  world->getMap(character->getCurrentMapId())->removeCharacter(character);
  for(Projectile * p : world->getMap(character->getCurrentMapId())->getProjectiles()) {
    if(p->getTarget() == character && !p->homing) {
      p->setLost(true);
    }
  }
  map->addCharacter(character);
  character->move(y, x, 0.5F, 0.5F, character->getOrientation(), map_id);
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
  character->move(y, x, 0.5F, 0.5F, character->getOrientation(), map_id);
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
  std::list<Character *> characters = std::list<Character *>();
  for (Map * map : world->getMaps()) {
    for (Character * character : map->getCharacters()) {
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

void Adventure::getNPCsActions() {
  for(Character * npc : getCharacters()) {
    if(!npc->player_character && npc->getNeedToUpdateActions()) {
      Action * action = npc->getAI()->getActions(this, npc);
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
    90.F,
    spawn->map_id,
    "party",
    new PlayerAI(),
    attr,
    (Attributes *) database->getAttributes("TXT_NO_ATTRIBUTES"),
    (Gear *) database->getGear("TXT_LIGHT_STARTING_GEAR"),
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
        getWorld()->getMap(c->getCurrentMapId())->killCharacter(c, c);
      }
    }
  }
}
