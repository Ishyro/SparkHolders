#ifndef _ADVENTURE_H_
#define _ADVENTURE_H_

#include <list>
#include <set>
#include <string>

#include "src/data/World.h"
#include "src/data/Character.h"
#include "src/data/Quest.h"

class Adventure {
  public:
    const int maxPlayers;
    const std::string name;
    Adventure(std::string name);
    Adventure(Save save);
    bool addPlayer(Character newPlayer);
    bool isWiped();
  private:
    World world;
    std::set<Character *> active_party;
    std::set<Character *> known_players;
    std::list<Quest *> quests;
    std::list<Event *> events;
};

#endif // _ADVENTURE_H_
