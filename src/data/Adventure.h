#ifndef _ADVENTURE_H_
#define _ADVENTURE_H_

#include <list>
#include <string>
#include <algorithm>

#include "src/data/World.h"
#include "src/data/Map.h"
#include "src/data/Tile.h"
#include "src/data/Character.h"
#include "src/data/Quest.h"
#include "src/data/Event.h"
#include "src/data/Save.h"

class Adventure {
  public:
    const int maxPlayers;
    const std::string name;
    Adventure(std::string name) {
      // TODO XML_READER
    }
    /*
    Adventure(Save * save) {
      // TODO XML_READER
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
    long tick;
    std::list<Character *> party;
    std::list<Character *> preserved_players;
    std::list<Quest *> quests;
    std::list<Event *> events;
};

#endif // _ADVENTURE_H_
