#include <list>
#include <set>

#include "src/loader/AdventureLoader.h"
#include "src/data/World.h"
#include "src/data/Character.h"
#include "src/data/Quest.h"

class Adventure {
  public:
    const int maxPlayers;
    const string name;
    Adventure(String name);
    Adventure(Save save);
    boolean addPlayer(Character newPlayer);
    boolean isWiped();
  private:
    World world;
    std::set<Character> active_party;
    std::set<Character> known_players;
    std::list<Quests> quests;
};
