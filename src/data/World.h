#include <set>

#include "src/loader/WorldLoader.h"
#include "src/data/Map.h"

class World {
  public:
    const string name;
    World(String name);
    boolean addMap(Map map);
    boolean changeMap(string name);
    long getTick();
    void incrTick();
  private:
    Map current;
    long tick;
    std::set<Map> maps;
};
