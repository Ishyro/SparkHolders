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
  float x;
  float y;
  float size;
  long gold;
  std::list<Weapon *> weapons;
  std::list<Item *> items;
  std::list<Ammunition *> ammunition;
} Loot;

typedef struct Target {
  int type;
  long id;
  float x;
  float y;
  Target * next;
} Target;

typedef struct MapDisplay {
  std::string time;
  std::string name;
  int id;
  int offsetX;
  int offsetY;
  int sizeX;
  int sizeY;
  bool outside;
  std::list<CharacterDisplay *> characters;
  std::list<ProjectileDisplay *> projectiles;
  std::list<Loot *> loots;
  std::vector<std::vector<Tile *>> tiles;
  std::list<Speech *> speeches;
} MapDisplay;

namespace map {
  static long id_cpt = 0;
}

class Map {
  public:
    const std::string name;
    const long id;
    const int offsetX;
    const int offsetY;
    const int sizeX;
    const int sizeY;
    const bool outside;
    Map(
      const std::string name,
      const int sizeX,
      const int sizeY,
      const bool outside
    ):
      name(name),
      id(0),
      offsetX(0),
      offsetY(0),
      sizeX(sizeX),
      sizeY(sizeY),
      outside(outside),
      tiles(sizeY),
      lights(sizeY),
      light(0)
    {
      for(int i = 0; i < sizeY; i++) {
        tiles[i] = std::vector<Tile *>(sizeX);
        lights[i] = std::vector<int>(sizeX);
      }
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      loots = std::list<Loot *>();
    }
    Map(const Map * map, std::string name):
      name(name),
      id(++map::id_cpt),
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
      id(map->id),
      offsetX(std::max(0, (int) std::floor(player->getX()) - std::max(player->getVisionRange(), player->getDetectionRange()))),
      offsetY(std::max(0, (int) std::floor(player->getY()) - std::max(player->getVisionRange(), player->getDetectionRange()))),
      sizeX(std::min(map->sizeX, (int) std::floor(player->getX()) + std::max(player->getVisionRange(), player->getDetectionRange()) + 1) - offsetX),
      sizeY(std::min(map->sizeY, (int) std::floor(player->getY()) + std::max(player->getVisionRange(), player->getDetectionRange()) + 1) - offsetY),
      outside(map->outside),
      lights(sizeY),
      light(map->light)
    {
      tiles = canSee(map, player, database);
      for(int i = 0; i < sizeY; i++) {
        lights[i] = std::vector<int>(sizeX);
      }
      for(int y = 0; y < sizeY; y++) {
        for(int x = 0; x < sizeX; x++) {
          if (map->getTile(y + offsetY, x + offsetX)->name != "TXT_MIST") {
            lights[y][x] = map->getLight(y + offsetY, x + offsetX);
          }
          else {
            lights[y][x] = 0;
          }
        }
      }
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      loots = std::list<Loot *>();
      for(Character * c : map->characters) {
        if(c->getX() - offsetX >= 0 && c->getX() - offsetX < sizeX && c->getY() - offsetY >= 0 && c->getY() - offsetY < sizeY &&
          tiles[c->getY() - offsetY][c->getX() - offsetX]->name != "TXT_MIST" &&
          !c->isInvisible() &&
          !c->isEtheral()) {
          characters.push_back(c);
        }
      }
      for(Projectile * p : map->projectiles) {
        if(p->getX() - offsetX >= 0 && p->getX() - offsetX < sizeX && p->getY() - offsetY >= 0 && p->getY() - offsetY < sizeY &&
          tiles[p->getY() - offsetY][p->getX() - offsetX]->name != "TXT_MIST") {
          projectiles.push_back(p);
        }
      }
      for(Loot * l : map->loots) {
        if(l->x - offsetX >= 0 && l->x - offsetX < sizeX && l->y - offsetY >= 0 && l->y - offsetY < sizeY &&
          tiles[l->y - offsetY][l->x - offsetX]->name != "TXT_MIST") {
          loots.push_back(l);
        }
      }
    }
    std::list<Character *> getCharacters();
    std::list<Projectile *> getProjectiles();
    std::list<Loot *> getLoots();
    Tile * getTile(int y, int x);
    Tile * getTile(float y, float x);
    int getLight(int y, int x);
    int getLight(float y, float x);
    void calculateLights();
    void propagateLight(int y, int x);
    void applyDayLight(int light);
    bool canSee(Character * watcher, Character * target);
    void setTile(int y, int x, Tile * tile);
    void setTile(float y, float x, Tile * tile);
    void crumble(int y, int x);
    void addCharacter(Character * c);
    void addProjectile(Projectile * p);
    void addLoot(Loot * l);
    void removeCharacter(Character * c);
    void killCharacter(Character * killer, Character * victim);
    void removeProjectile(Projectile * p);
    void destroyProjectile(Projectile * p);
    void removeLoot(Loot * l);
    void destroyLoot(Loot * l);
    void takeLoot(Character * c, int mode);
    float getMoveCost(Character * c, float y, float x);
    bool tryMove(Character * c, float destY, float destX);
    float move(Character * c, float y, float x);
    float move(Character * c, float orientation, float destY, float destX, float ap, World * world);
    float actProjectile(Projectile * p, Adventure * adventure, float speed);
    void clearProjectiles();
    static std::string target_to_string(Target * target);
    static Target * target_from_string(std::string to_read);
    std::string to_string(Character * player, Adventure * adventure);
    static MapDisplay * from_string(std::string to_read);
    static std::vector<std::vector<Tile *>> canSee(Map * map, Character * watcher, Database * database);
    bool operator == (const Map& m) const { return id == m.id; }
    bool operator != (const Map& m) const { return !operator==(m); }
  private:
    int light;
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Loot *> loots;
    std::vector<std::vector<Tile *>> tiles;
    std::vector<std::vector<int>> lights;
};

#endif // _MAP_H_
