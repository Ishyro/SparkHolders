#ifndef _MAP_H_
#define _MAP_H_

#include <list>
#include <vector>
#include <string>
#include <cmath>

#include "data/Character.h"
#include "data/Projectile.h"

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
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      loots = std::list<Loot *>();
    }
    Map(const Map * map, std::string name):
      name(name),
      sizeX(map->sizeX),
      sizeY(map->sizeY),
      outside(map->outside),
      tiles(map->tiles),
      lights(map->lights)
    {}
    // use this only for players, too much work for every characters
    Map(const Map * map, Character * player):
      name(name),
      sizeX(map->sizeX),
      sizeY(map->sizeY),
      outside(map->outside),
      tiles(map->sizeY),
      lights(map->sizeY),
      light(map->light)
    {
      tiles = std::vector<std::vector<Tile *>>();
      lights = std::vector<std::vector<int>>();
      for(long i = 0; i < sizeY; i++) {
        tiles[i] = std::vector<Tile *>(sizeX);
        lights[i] = std::vector<int>(sizeX);
      }
      for(long x = 0; x < sizeX; x++) {
        for(long y = 0; y < sizeY; y++) {
          int distance = std::max(abs(player->getX() - x), abs(player->getY() -y));
          if ( (distance <= player->getVisionRange() && getLight(x, y) >= 10 - player->getVisionPower()) || distance <= player->getDetectionRange() ) {
            tiles[x][y] = map->tiles[x][y];
            lights[x][y] = map->lights[x][y];
          }
          else {
            tiles[x][y] = nullptr;
            lights[x][y] = -light;
          }
        }
      }
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      loots = std::list<Loot *>();
      for(Character * c : map->characters) {
        if(tiles[c->getX()][c->getY()] != nullptr) {
          characters.push_back(c);
        }
      }
      for(Projectile * p : map->projectiles) {
        if(tiles[p->getX()][p->getY()] != nullptr) {
          projectiles.push_back(p);
        }
      }
      for(Loot * l : map->loots) {
        if(tiles[l->x][l->y] != nullptr) {
          loots.push_back(l);
        }
      }
    }
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
    void killCharacter(Character * killer, Character * victim);
    void removeProjectile(Projectile * p);
    void destroyProjectile(Projectile * p);
    void removeLoot(Loot * l);
    void destroyLoot(Loot * l);
    void takeLoot(Character * c);
    void move(Character *, int orientation);
    void actProjectile(Projectile * p, Adventure * adventure);
    void actAllProjectiles(Adventure * adventure);
  private:
    int light;
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Loot *> loots;
    std::vector<std::vector<Tile *>> tiles;
    std::vector<std::vector<int>> lights;
};

#endif // _MAP_H_
