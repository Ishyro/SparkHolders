#include <list>

#include "src/loader/ItemLoader.h"
#include "src/data/Effect.h"

// type

#define NONE 0
#define HEAD 1
#define LEFT_ARM 2
#define RIGHT_ARM 3
#define BOTH_ARMS 4
#define BODY 5
#define LEGS 6
#define FULL_BODY 7
#define RING 8
#define AMULET 9

class Item {
  public:
    const string name;
    const long id;
    const boolean equipable;
    const int type;
    Item(string name);
  private:
    static long id_cpt = 0;
    std::list<Effect> effects;
};
