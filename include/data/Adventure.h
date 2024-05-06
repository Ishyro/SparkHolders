#ifndef _ADVENTURE_H_
#define _ADVENTURE_H_

#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>

#include "data/Settings.h"

#include "util/MathUtil.h"

#include "Values.h"

typedef struct Spawn {
  int32_t x;
  int32_t y;
  int32_t z;
} Spawn;

typedef struct Time {
  int32_t year;
  int32_t month;
  int32_t week;
  int32_t day;
  int32_t hour;
  int32_t minutes;
  int32_t seconds;
  std::string to_string_clock() {
    int32_t charHoursSize = std::to_string(Settings::getDayDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
    int32_t charMinutesSize = std::to_string(Settings::getHourDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
    int32_t charSecondsSize = std::to_string(Settings::getMinuteDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
    std::string hour_str = std::to_string(hour);
    while(hour_str.length() - charHoursSize > 0) {
      hour_str = std::to_string(0) + hour_str;
    }
    std::string minutes_str = std::to_string(minutes);
    while(minutes_str.length() - charMinutesSize > 0) {
      minutes_str = std::to_string(0) + minutes_str;
    }
    std::string seconds_str = std::to_string(seconds);
    while(seconds_str.length() - charSecondsSize > 0) {
      seconds_str = std::to_string(0) + seconds_str;
    }
    std::stringstream ss = std::stringstream();
    ss << hour_str << ":" <<minutes_str << ":" << seconds_str;
    std::string result = ss.str();
    return result;
  }
  std::string to_string_day() {
    std::stringstream ss = std::stringstream();
    int32_t indice_day = (year * Settings::getYearDuration() + month * Settings::getMonthDuration() + day) % Settings::getWeekDuration() - 1;
    int32_t indice_month = month - 1;
    ss << Settings::getDayName(indice_day) << ", " << day << " of " << Settings::getMonthName(indice_month) << ", " << year;
    std::string result = ss.str();
    return result;
  }
  std::string to_string_day_short() {
    std::stringstream ss = std::stringstream();
    int32_t charDaySize = std::to_string(Settings::getYearDuration() / Settings::getMonthDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
    int32_t charMonthSize = std::to_string(Settings::getMonthDuration() - 1).length(); // -1 because if size is for example 100, we never reach 100
    std::string day_str = std::to_string(day);
    while(day_str.length() - charDaySize > 0) {
      day_str = std::to_string(0) + day_str;
    }
    std::string month_str = std::to_string(month);
    while(month_str.length() - charMonthSize > 0) {
      month_str = std::to_string(0) + month_str;
    }
    ss << day_str << "/" << month_str << "/" << year;
    std::string result = ss.str();
    return result;
  }
} Time;

typedef struct StateDisplay {
  std::list<CharacterDisplay *> characters;
  std::list<ProjectileDisplay *> projectiles;
  std::list<Loot *> loots;
  std::list<Speech *> speeches;
  std::list<Furniture *> changed_furnitures;
  bool need_to_send_actions;
} StateDisplay;

class Adventure {
  public:
    const std::string name;
    const std::string filePath;
    const int32_t maxPlayers;
    Adventure(
      std::string name,
      std::string filePath,
      int32_t maxPlayers,
      Database * database,
      World * world,
      std::list<Quest *> quests,
      std::list<Event *> events,
      std::list<Spawn *> spawns,
      std::list<Attributes *> startingAttributes,
      std::list<Way *> startingWays
    ):
      name(name),
      filePath(filePath),
      maxPlayers(maxPlayers),
      database(database),
      world(world),
      quests(quests),
      events(events),
      spawns(spawns),
      startingAttributes(startingAttributes),
      startingWays(startingWays),
      party(std::list<Character *>()),
      preserved_players(std::list<Character *>()),
      actions(std::list<Action *>())
    {
      tick = 0;
      round = 0L;
    }
    /*
    Adventure(Save * save) {
      // TODO INI_READER
    }
    */
    Save * save();
    void softMoveCharacterToMap(Character * character, MathUtil::Vector3 coord);
    void hardMoveCharacterToMap(Character * character, MathUtil::Vector3 coord);
    void addPlayer(Character * player);
    void removePlayer(Character * player);
    bool isWiped();
    std::list<Character *> getParty();
    std::list<Character *> getPreservedPlayers();
    void resurrect(Character * player, MathUtil::Vector3 coord);
    int64_t getRound();
    float getTick();
    void incrTick();
    void event();
    World * getWorld();
    std::list<Attributes *> getStartingAttributes();
    std::list<Way *> getStartingWays();
    Database * getDatabase();
    void addQuest(Quest * quest);
    void removeQuest(Quest * quest);
    std::list<Character *> getCharacters();
    Character * getCharacter(int64_t id);
    Furniture * getFurniture(int64_t id);
    std::list<Projectile *> getProjectiles();
    void getNPCsActions();
    void mergeActions(std::list<Action *> to_add);
    void executeActions();
    void actAllProjectiles();
    Character * spawnPlayer(std::string name, Attributes * attr, Race * race, Way * origin, Way * culture, Way * religion, Way * profession);
    void applyIteration();
    Time getTime();
    int32_t getLight(MathUtil::Vector3i coord);
    int32_t getLight(MathUtil::Vector3 coord);
    std::string state_to_string(std::map<const int64_t, Character *> players);
    StateDisplay * update_state(std::string to_read);
  private:
    World * world;
    Database * database;
    float tick;
    int64_t round;
    std::list<Character *> party;
    std::list<Character *> preserved_players;
    std::list<Quest *> quests;
    std::list<Event *> events;
    std::list<Spawn *> spawns;
    std::list<Attributes *> startingAttributes;
    std::list<Way *> startingWays;
    std::list<Action *> actions;
};

#endif // _ADVENTURE_H_
