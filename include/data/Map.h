#ifndef _MAP_H_
#define _MAP_H_

#include <list>
#include <vector>
#include <string>

#include "data/Character.h"
#include "data/Projectile.h"
#include "data/Item.h"
#include "data/Weapon.h"
#include "data/Tile.h"

#include "utils/FileOpener.h"

#include "communications/SpeechManager.h"

typedef struct Loot {
  int type;
  long x;
  long y;
  long gold;
  std::list<Weapon *> weapons;
  std::list<Item *> items;
  std::list<Ammunition *> ammunitions;
} Loot;

namespace map {
  static long id_cpt = 0;
}

class Map {
  public:
    const std::string name;
    const long id = ++map::id_cpt;
    const long sizeX;
    const long sizeY;
    const bool outside;
    Map(
      const std::string name,
      const long sizeX,
      const long sizeY,
      const bool outside
    ):
      name(name),
      sizeX(sizeX),
      sizeY(sizeY),
      outside(outside),
      tiles(sizeY),
      lights(sizeY)
    {
      for(long i = 0; i < sizeY; i++) {
        tiles[i] = std::vector<Tile *>(sizeX);
        lights[i] = std::vector<int>(sizeX);
      }
    }
    Map(const Map * map, std::string name):
      name(name),
      sizeX(map->sizeX),
      sizeY(map->sizeY),
      outside(map->outside),
      tiles(map->tiles),
      lights(map->lights)
    {}
    std::list<Character *> getCharacters();
    std::list<Projectile *> getProjectiles();
    std::list<Loot *> getLoots();
    Tile * getTile(long x, long y);
    void setTile(long x, long y, Tile * tile);
    int getLight(long x, long y);
    void calculateLights();
    void propagateLight(long x, long y);
    void applyDayLight(int light);
    void crumble(long x, long y);
    void addCharacter(Character * c);
    void addProjectile(Projectile * p);
    void addLoot(Loot * l);
    void removeCharacter(Character * c);
    void killCharacter(Character * victim, Character * killer);
    void removeProjectile(Projectile * p);
    void destroyProjectile(Projectile * p);
    void removeLoot(Loot * l);
    void destroyLoot(Loot * l);
    void takeLoot(Character * c);
    void tryHit(Projectile * p, Adventure * adventure);
    void move(Character *, int orientation);
  private:
    int light;
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Loot *> loots;
    std::vector<std::vector<Tile *>> tiles;
    std::vector<std::vector<int>> lights;
};

typedef struct MapLink {
    long x1;
    long y1;
    int orientation1;
    long x2;
    long y2;
    int orientation2;
    Map * map1;
    Map * map2;
} MapLink;

#endif // _MAP_H_
