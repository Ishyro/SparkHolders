#ifndef _MAP_H_
#define _MAP_H_

#include <algorithm>
#include <list>
#include <set>
#include <vector>
#include <string>

#include "data/Character.h"
#include "data/Database.h"
#include "data/Projectile.h"
#include "data/Tile.h"
#include "data/World.h"

typedef struct Loot {
  int type;
  float x;
  float y;
  float size;
  long gold;
  std::list<Item *> items;
} Loot;

typedef struct Target {
  int type;
  long id;
  float x;
  float y;
  float z;
  Target * next;
} Target;

namespace map {
  static long id_cpt = 0;
}

class Map {
  public:
    const std::string name;
    const std::string baseName;
    const long id;
    const int offsetX;
    const int offsetY;
    const int offsetZ;
    const int sizeX;
    const int sizeY;
    const int sizeZ;
    const bool outside;
    Map(
      const std::string name,
      const int sizeX,
      const int sizeY,
      const int sizeZ,
      const bool outside
    ):
      name(name),
      baseName(name),
      id(0),
      offsetX(0),
      offsetY(0),
      offsetZ(0),
      sizeX(sizeX),
      sizeY(sizeY),
      sizeZ(sizeZ),
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
    Map(const Map * map, std::string name, int offsetX, int offsetY, int offsetZ):
      name(name),
      baseName(map->name),
      id(++map::id_cpt),
      offsetX(offsetX),
      offsetY(offsetY),
      offsetZ(offsetZ),
      sizeX(map->sizeX),
      sizeY(map->sizeY),
      sizeZ(map->sizeZ),
      outside(map->outside),
      tiles(map->tiles),
      lights(map->lights),
      light(map->light)
    {
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      loots = std::list<Loot *>();
    }

    Map(Map * map, Character * player, Database * database, World * world):
      name(map->name),
      baseName(map->name),
      id(map->id),
      offsetX( (int) std::floor(player->getX()) - std::max(player->getVisionRange(), player->getDetectionRange())),
      offsetY( (int) std::floor(player->getY()) - std::max(player->getVisionRange(), player->getDetectionRange())),
      offsetZ(map->offsetZ),
      sizeX( (int) std::floor(player->getX()) + std::max(player->getVisionRange(), player->getDetectionRange()) + 1 - offsetX),
      sizeY( (int) std::floor(player->getY()) + std::max(player->getVisionRange(), player->getDetectionRange()) + 1 - offsetY),
      sizeZ(map->sizeZ),
      outside(map->outside),
      lights(sizeY),
      tiles(sizeY),
      light(map->light)
    {
      std::set<long> ids = std::set<long>();
      ids.insert(id);
      for(int i = 0; i < sizeY; i++) {
        lights[i] = std::vector<int>(sizeX);
        tiles[i] = std::vector<Tile *>(sizeX);
      }
      for(int y = 0; y < sizeY; y++) {
        for(int x = 0; x < sizeX; x++) {
          Tile * tile = map->getTile(x + offsetX, y + offsetY); 
          if(tile != nullptr && tile->name != "TXT_VOID") {
            tiles[y][x] = tile;
            lights[y][x] = map->getLight(x + offsetX, y + offsetY);
          }
          else {
            tiles[y][x] = world->getTile(x + offsetX, y + offsetY, offsetZ);
            if(tiles[y][x] == nullptr) {
              tiles[y][x] = (Tile *) database->getTile("TXT_VOID");
              lights[y][x] = 0;
            }
            else {
              lights[y][x] = world->getLight(x + offsetX, y + offsetY, offsetZ);
              ids.insert(world->getMap(x + offsetX, y + offsetY, offsetZ)->id);
            }
          }
        }
      }
      mist_nb = canSee(player, (Tile *) database->getTile("TXT_MIST"), offsetX, offsetY, offsetZ, sizeX, sizeY, tiles, lights);
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      loots = std::list<Loot *>();
      for(int used_map_id : ids) {
        Map * used_map = world->getMap(used_map_id);
        for(Character * c : used_map->characters) {
          if(c->getX() - offsetX >= 0 && c->getX() - offsetX < sizeX && c->getY() - offsetY >= 0 && c->getY() - offsetY < sizeY &&
            tiles[c->getY() - offsetY][c->getX() - offsetX]->name != "TXT_MIST" &&
            !c->isInvisible() &&
            !c->isEtheral()) {
            characters.push_back(c);
          }
        }
        for(Projectile * p : used_map->projectiles) {
          if(p->getX() - offsetX >= 0 && p->getX() - offsetX < sizeX && p->getY() - offsetY >= 0 && p->getY() - offsetY < sizeY &&
            tiles[p->getY() - offsetY][p->getX() - offsetX]->name != "TXT_MIST") {
            projectiles.push_back(p);
          }
        }
        for(Loot * l : used_map->loots) {
          if(l->x - offsetX >= 0 && l->x - offsetX < sizeX && l->y - offsetY >= 0 && l->y - offsetY < sizeY &&
            tiles[l->y - offsetY][l->x - offsetX]->name != "TXT_MIST") {
            loots.push_back(l);
          }
        }
      }
    }

    Map(Map * map, Projectile * projectile, Database * database, World * world):
      name(map->name),
      baseName(map->name),
      id(map->id),
      offsetX( (int) std::floor(projectile->getX()) - projectile->getSpeed()),
      offsetY( (int) std::floor(projectile->getY()) - projectile->getSpeed()),
      offsetZ(map->offsetZ),
      sizeX( (int) std::floor(projectile->getX()) + projectile->getSpeed() + 1 - offsetX),
      sizeY( (int) std::floor(projectile->getY()) + projectile->getSpeed() + 1 - offsetY),
      sizeZ(map->sizeZ),
      outside(map->outside),
      lights(sizeY),
      tiles(sizeY),
      light(map->light)
    {
      std::set<int> ids = std::set<int>();
      ids.insert(id);
      for(int i = 0; i < sizeY; i++) {
        tiles[i] = std::vector<Tile *>(sizeX);
      }
      for(int y = 0; y < sizeY; y++) {
        for(int x = 0; x < sizeX; x++) {
          Tile * tile = map->getTile(x + offsetX, y + offsetY); 
          if(tile != nullptr && tile->name != "TXT_VOID") {
            tiles[y][x] = tile;
          }
          else {
            tiles[y][x] = world->getTile(x + offsetX, y + offsetY, offsetZ);
            if(tiles[y][x] == nullptr) {
              tiles[y][x] = (Tile *) database->getTile("TXT_VOID");
            }
            else {
              ids.insert(world->getMap(x + offsetX, y + offsetY, offsetZ)->id);
            }
          }
        }
      }
      characters = std::list<Character *>();
      for(int used_map_id : ids) {
        Map * used_map = world->getMap(used_map_id);
        for(Character * c : used_map->characters) {
          if(c->getX() - offsetX >= 0 && c->getX() - offsetX < sizeX && c->getY() - offsetY >= 0 && c->getY() - offsetY < sizeY) {
            characters.push_back(c);
          }
        }
      }
    }
    std::list<Character *> getCharacters();
    std::list<Projectile *> getProjectiles();
    std::list<Loot *> getLoots();
    bool allowedCoords(int x, int y);
    bool allowedCoords(float x, float y);
    Tile * getTile(int x, int y);
    Tile * getTile(float x, float y);
    int getLight(int x, int y);
    int getLight(float x, float y);
    void calculateLights();
    void propagateLight(int x, int y);
    void applyDayLight(int light);
    bool canSee(Character * watcher, Character * target);
    int getMistNb();
    void setTile(int x, int y, Tile * tile);
    void setTile(float x, float y, Tile * tile);
    void crumble(int x, int y);
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
    float getMoveCost(Character * c, float ori_x, float ori_y, float x, float y);
    bool tryMove(Character * c, float destX, float destY);
    float move(Character * c, float y, float x, World * world);
    float move(Character * c, float orientation, float destX, float destY, float ap, World * world);
    float actProjectile(Projectile * p, Adventure * adventure, float speed);
    void clearProjectiles();
    std::string tile_to_string(int x, int y);
    static std::string target_to_string(Target * target);
    static Target * target_from_string(std::string to_read);
    static int canSee(
      Character * watcher,
      Tile * mist,
      int offsetX,
      int offsetY,
      int offsetZ,
      int sizeX,
      int sizeY,
      std::vector<std::vector<Tile *>>& tiles,
      std::vector<std::vector<int>>& lights
    );
    bool operator == (const Map& m) const { return id == m.id; }
    bool operator != (const Map& m) const { return !operator==(m); }
  private:
    int light;
    int mist_nb = 0;
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Loot *> loots;
    std::vector<std::vector<Tile *>> tiles;
    std::vector<std::vector<int>> lights;
};

#endif // _MAP_H_
