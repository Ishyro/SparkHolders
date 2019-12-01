#include <list>

#include "src/loader/MapLoader.h"
#include "src/data/Character.h"
#include "src/data/Projectile.h"
#include "src/data/Item.h"

typedef struct MapLink {
    int x1;
    int x2;
    int y1;
    int y2;
    Map map1;
    Map map2;
} MapLink;

class Map {
  public:
    const string name;
    const long sizeX;
    const long sizeY;
    Map(string name);
  private:
    std::list<Character> mobs;
    std::list<Projectile> projectiles;
    std::list<Item> loot;
    Tile[][] tiles;
    std::list<MapLink> links;
};
