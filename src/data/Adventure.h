#ifndef _ADVENTURE_H_
#define _ADVENTURE_H_

#include <list>
#include <string>

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
    Adventure(Save * save) {
      // TODO XML_READER
    }
    Save * save();
    void softMoveCharacterToMap(Character * character, long mapId, long x, long y);
    void hardMoveCharacterToMap(Character * character, long mapId, long x, long y);
    void addPlayer(Character * player);
    void removePlayer(Character * player);
    bool isWiped();
    std::list<Character *> getParty();
    int getTick();
    void event();
    World * getWorld();
    void addQuest(Quest * quest);
    void removeQuest(Quest * quest);
  private:
    World * world;
    std::list<Character *> party;
    std::list<Quest *> quests;
    std::list<Event *> events;
};

#endif // _ADVENTURE_H_
