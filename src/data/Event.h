#include <list>

#include "src/loader/EventLoader.h"
#include "src/data/Character.h"
#include "src/data/Map.h"
#include "src/data/Speech.h"

// type

#define CALAMITY 1
#define BLESSING 2

class Event {
  public:
    const string name;
    const int type;
    const int tick;
    Event(string name);
    void activate(World world);
  private:
    std::list<Character> characters;
    std::list<Map> maps;
    std::list<Speech> speechs;
    std::list<Effect> effects;
};
