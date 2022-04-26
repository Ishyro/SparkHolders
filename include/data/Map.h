#ifndef _MAP_H_
#define _MAP_H_

#include <algorithm>
#include <list>
#include <vector>
#include <string>

#include "data/Character.h"
#include "data/Database.h"
#include "data/Projectile.h"
#include "data/Tile.h"

typedef struct Loot {
  int type;
  long x;
  long y;
  long gold;
  std::list<Weapon *> weapons;
  std::list<Item *> items;
  std::list<Ammunition *> ammunitions;
} Loot;

typedef struct MapDisplay {
  std::string name;
  long offsetX;
  long offsetY;
  long sizeX;
  long sizeY;
  bool outside;
  std::list<CharacterDisplay *> characters;
  std::list<ProjectileDisplay *> projectiles;
  std::list<Loot *> loots;
  std::vector<std::vector<Tile *>> tiles;
} MapDisplay;

namespace map {
  static long id_cpt = 0;
}

class Map {
  public:
    const std::string name;
    const long id = ++map::id_cpt;
    const long offsetX;
    const long offsetY;
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
      offsetX(0),
      offsetY(0),
      sizeX(sizeX),
      sizeY(sizeY),
      outside(outside),
      tiles(sizeY),
      lights(sizeY),
      light(0)
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
      offsetX(0),
      offsetY(0),
      sizeX(map->sizeX),
      sizeY(map->sizeY),
      outside(map->outside),
      tiles(map->tiles),
      lights(map->lights),
      light(map->light)
    {}

    // use this only for players, too much work for every characters
    Map(Map * map, Character * player, Database * database):
      name(map->name),
      offsetX(std::max(0, player->getX() - std::max(player->getVisionRange(), player->getDetectionRange()))),
      offsetY(std::max(0, player->getY() - std::max(player->getVisionRange(), player->getDetectionRange()))),
      sizeX(std::min(map->sizeX, (long) player->getX() + (long) std::max(player->getVisionRange(), player->getDetectionRange())) - offsetX),
      sizeY(std::min(map->sizeY, (long) player->getY() + (long) std::max(player->getVisionRange(), player->getDetectionRange())) - offsetY),
      outside(map->outside),
      tiles(sizeY),
      lights(sizeY),
      light(map->light)
    {
      for(long i = 0; i < sizeY; i++) {
        tiles[i] = std::vector<Tile *>(sizeX);
        lights[i] = std::vector<int>(sizeX);
      }
      for(long y = 0; y < sizeY; y++) {
        for(long x = 0; x < sizeX; x++) {
          int distance = std::max(abs(player->getX() - (x + offsetX)), abs(player->getY() - (y + offsetY)));
          if ( (distance <= player->getVisionRange() && map->getLight(y + offsetY, x + offsetX) >= 10 - player->getVisionPower()) || distance <= player->getDetectionRange() ) {
            tiles[y][x] = map->tiles[y + offsetY][x + offsetX];
            lights[y][x] = map->lights[y + offsetY][x + offsetX];
          }
          else {
            tiles[y][x] = (Tile *) database->getTile("mist");
          }
        }
      }
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      loots = std::list<Loot *>();
      for(Character * c : map->characters) {
        if(c->getX() - offsetX >= 0 && c->getX() - offsetX < sizeX && c->getY() - offsetY >= 0 && c->getY() - offsetY < sizeY &&
          tiles[c->getY() - offsetY][c->getX() - offsetX]->name != "mist") {
          characters.push_back(c);
        }
      }
      for(Projectile * p : map->projectiles) {
        if(p->getX() - offsetX >= 0 && p->getX() - offsetX < sizeX && p->getY() - offsetY >= 0 && p->getY() - offsetY < sizeY &&
          tiles[p->getY() - offsetY][p->getX() - offsetX]->name != "mist") {
          projectiles.push_back(p);
        }
      }
      for(Loot * l : map->loots) {
        if(l->x - offsetX >= 0 && l->x - offsetX < sizeX && l->y - offsetY >= 0 && l->y - offsetY < sizeY &&
          tiles[l->y - offsetY][l->x - offsetX]->name != "mist") {
          loots.push_back(l);
        }
      }
    }
    std::list<Character *> getCharacters();
    std::list<Projectile *> getProjectiles();
    std::list<Loot *> getLoots();
    Tile * getTile(long y, long x);
    void setTile(long y, long x, Tile * tile);
    int getLight(long y, long x);
    void calculateLights();
    void propagateLight(long y, long x);
    void applyDayLight(int light);
    void crumble(long y, long x);
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
    void move(Character *, int orientation, Adventure * adventure);
    void actProjectile(Projectile * p, Adventure * adventure);
    void actAllProjectiles(Adventure * adventure);
    std::string to_string(Character * player, Adventure * adventure);
    static MapDisplay * from_string(std::string to_read);
  private:
    int light;
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Loot *> loots;
    std::vector<std::vector<Tile *>> tiles;
    std::vector<std::vector<int>> lights;
};

#endif // _MAP_H_
