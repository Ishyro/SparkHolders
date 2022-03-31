#ifndef _ADVENTURE_H_
#define _ADVENTURE_H_

#include <list>
#include <string>
#include <algorithm>

#include "data/World.h"
#include "data/Map.h"
#include "data/Tile.h"
#include "data/Character.h"
#include "data/Quest.h"
#include "data/Event.h"
#include "data/Save.h"
#include "data/Database.h"

#include "utils/FileOpener.h"

class Adventure {
  public:
    const std::string name;
    const int maxPlayers;
    Adventure(
      std::string name,
      int maxPlayers,
      Database * database,
      World * world,
      std::list<Quest *> quests,
      std::list<Event *> events
    ):
      name(name),
      maxPlayers(maxPlayers),
      database(database),
      world(world),
      quests(quests),
      events(events)
    {
      party = std::list<Character *>();
      preserved_players = std::list<Character *>();
      tick = 0;
    }
    /*
    Adventure(Save * save) {
      // TODO INI_READER
    }
    */
    Save * save();
    void softMoveCharacterToMap(Character * character, long mapId, long x, long y);
    void hardMoveCharacterToMap(Character * character, long mapId, long x, long y);
    void addPlayer(Character * player);
    void removePlayer(Character * player);
    bool isWiped();
    std::list<Character *> getParty();
    std::list<Character *> getPreservedPlayers();
    void resurrect(Character * player, long map_id, long x, long y);
    long getTick();
    void incrTick();
    void event();
    World * getWorld();
    void addQuest(Quest * quest);
    void removeQuest(Quest * quest);
  private:
    World * world;
    Database * database;
    long tick;
    std::list<Character *> party;
    std::list<Character *> preserved_players;
    std::list<Quest *> quests;
    std::list<Event *> events;
};

#endif // _ADVENTURE_H_
