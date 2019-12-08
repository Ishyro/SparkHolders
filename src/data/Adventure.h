#ifndef _ADVENTURE_H_
#define _ADVENTURE_H_

#include <list>
#include <string>

#include "src/data/World.h"
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
    void addPlayer(Character * player);
    void removePlayer(Character * player);
    bool isWiped();
    int getTick();
    void event();
  private:
    World * world;
    std::list<Character *> party;
    std::list<Quest *> quests;
    std::list<Event *> events;
};

#endif // _ADVENTURE_H_
