#ifndef _MAP_H_
#define _MAP_H_

#include <list>
#include <string>

#include "src/data/Character.h"
#include "src/data/Projectile.h"
#include "src/data/Item.h"
#include "src/data/Loot.h"

class Map {
  public:
    const std::string name;
    const long sizeX;
    const long sizeY;
    Map(std::string name);
  private:
    std::list<Character *> mobs;
    std::list<Projectile *> projectiles;
    std::list<Loot *> loots;
    Tile ** tiles;
};

typedef struct MapLink {
    int x1;
    int x2;
    int y1;
    int y2;
    Map * map1;
    Map * map2;
} MapLink;

#endif // _MAP_H_
