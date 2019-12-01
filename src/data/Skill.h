#include <list>

#include "src/loader/SkillLoader.h"
#include "src/data/World.h"
#include "src/data/Effect.h"

// target

#define SELF 1
#define OTHER 2

// type

#define ACTIVE 1
#define PASSIVE 2
#define TOGGLE 3

// special

#define TIMESTOP 1
#define TILESWAP 2
#define TELEPORT 3
#define MAPLINKER 4

// cost_type

#define NONE 0
#define INCREMENTAL 1
#define EXPONENTIAL 2


class Skill {
  public:
    const string name;
    const int type;
    const int mana_cost;
    const int cost_type;
    const std::list<Effect> effects;
    const std::list<int> specials;
    Skill(string name);
    void activate(World world, Character owner);
    void desactivate();
    int drain();
  private:
    int tick;
    int current_mana_cost;
};
