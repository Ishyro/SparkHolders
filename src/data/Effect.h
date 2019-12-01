#include <list>

#include "src/loader/EffectLoader.h"
#include "src/data/World.h"

// stats

#define HP 1
#define MANA 2
#define DEFENSE 3
#define SOULBURNTRESHOLD 4
#define FLOW 5
#define VISION 6
#define DAMAGE 7
#define SPEED 8


class Effect {
  public:
    const string name;
    const int type;
    const int mana_cost;
    const int cost_type;
    const std::list<int> stats;
    const std::list<int> specials;
    Effect(string name);
};
