#include <set>

#include "src/loader/WorldLoader.h"
#include "src/data/Map.h"

class World {
  public:
    const string name;
    World(String name);
    boolean addMap(Map map);
    boolean changeMap(String name);
  private:
    Map current;
    std::set<Map> maps;
};
