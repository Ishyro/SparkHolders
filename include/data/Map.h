#ifndef _MAP_H_
#define _MAP_H_

#include <algorithm>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <string>

#include "data/Character.h"
#include "data/Database.h"
#include "data/Projectile.h"
#include "data/Block.h"
#include "data/World.h"

#include "data/furnitures/Furniture.h"
#include "data/furnitures/BasicFurniture.h"
#include "data/furnitures/ContainerFurniture.h"
#include "data/furnitures/CraftingFurniture.h"
#include "data/furnitures/LinkedFurniture.h"
#include "data/furnitures/SkillFurniture.h"
#include "data/furnitures/SwitchFurniture.h"

#include "util/MathUtil.h"

typedef struct Loot {
  int32_t type;
  float x;
  float y;
  float size;
  std::list<Item *> items;
} Loot;

namespace map {
  static int64_t id_cpt = 0;
}

class Map {
  public:
    const std::string name;
    const std::string baseName;
    const int64_t id;
    const int32_t offsetX;
    const int32_t offsetY;
    const int32_t offsetZ;
    const int32_t sizeX;
    const int32_t sizeY;
    const int32_t sizeZ;
    const int32_t lightening;
    Map(
      const std::string name,
      const int32_t sizeX,
      const int32_t sizeY,
      const int32_t sizeZ,
      const int32_t lightening
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
      lightening(lightening),
      blocks(std::map<const MathUtil::Vector3i, Block *>()),
      light(0),
      rotation(0)
    {
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      furnitures = std::list<Furniture *>();
      loots = std::list<Loot *>();
    }

    Map(Map * map, std::string name, int32_t offsetX, int32_t offsetY, int32_t offsetZ, int32_t rotation, int32_t lightening, Database * database):
      name(name),
      baseName(map->name),
      id(++map::id_cpt),
      offsetX(offsetX),
      offsetY(offsetY),
      offsetZ(offsetZ),
      sizeX(rotation == 0 || rotation == 180 ? map->sizeX : map->sizeY),
      sizeY(rotation == 0 || rotation == 180 ? map->sizeY : map->sizeX),
      sizeZ(map->sizeZ),
      lightening(lightening),
      blocks(std::map<const MathUtil::Vector3i, Block *>()),
      lights(this->sizeY),
      light(map->light),
      rotation(rotation)
    {
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      loots = std::list<Loot *>();
      for(auto pair : map->blocks) {
        MathUtil::Vector3i coord = pair.first;
        MathUtil::Vector3i new_coord;
        Block * block = pair.second;
        switch(rotation) {
          case 0:
            new_coord.x = coord.x + offsetX;
            new_coord.y = coord.y + offsetY;
            new_coord.z = coord.z + offsetZ;
            break;
          case 90:
            new_coord.x = sizeY - 1 - coord.y + offsetX;
            new_coord.y = coord.x + offsetY;
            new_coord.z = coord.z + offsetZ;
            break;
          case 180:
            new_coord.x = sizeX - 1 - coord.x + offsetX;
            new_coord.y = sizeY - 1 - coord.y + offsetY;
            new_coord.z = coord.z + offsetZ;
            break;
          case 270:
            new_coord.x = coord.y + offsetX;
            new_coord.y = sizeX - 1 - coord.x + offsetY;
            new_coord.z = coord.z + offsetZ;
            break;
          default:;
        }
        // oriented block
        if(block->name.find('#') != std::string::npos) {
          int32_t orientation = rotation + block->orientation;
          orientation = orientation >= 360.F ? orientation - 360.F : orientation;
          std::string the_name = std::string(block->name);
          switch(orientation) {
            case 0:
              setBlock(new_coord, (Block *) database->getBlock(block->name.substr(0, block->name.find('#')) + "#EAST"));
              break;
            case 90:
              setBlock(new_coord, (Block *) database->getBlock(block->name.substr(0, block->name.find('#')) + "#NORTH"));
              break;
            case 180:
              setBlock(new_coord, (Block *) database->getBlock(block->name.substr(0, block->name.find('#')) + "#WEST"));
              break;
            case 270:
              setBlock(new_coord, (Block *) database->getBlock(block->name.substr(0, block->name.find('#')) + "#SOUTH"));
              break;
          }
        }
        else {
          setBlock(new_coord, block);
        }
      }
      furnitures = std::list<Furniture *>();
      for(Furniture * furniture : map->furnitures) {
        switch(furniture->type) {
          case FURNITURE_BASIC:
            furnitures.push_back(new BasicFurniture( (BasicFurniture *) furniture, this));
            break;
          case FURNITURE_CONTAINER:
            furnitures.push_back(new ContainerFurniture( (ContainerFurniture *) furniture, this));
            break;
          case FURNITURE_CRAFTING:
            furnitures.push_back(new CraftingFurniture( (CraftingFurniture *) furniture, this));
            break;
          case FURNITURE_LINKED:
            furnitures.push_back(new LinkedFurniture( (LinkedFurniture *) furniture, this));
            break;
          case FURNITURE_SKILL:
            furnitures.push_back(new SkillFurniture( (SkillFurniture *) furniture, this));
            break;
          case FURNITURE_SWITCH:
            furnitures.push_back(new SwitchFurniture( (SwitchFurniture *) furniture, this));
            break;
        }
      }
    }

/*
    Map(Map * map, Character * player, Database * database, World * world):
      name(map->name),
      baseName(map->name),
      id(map->id),
      offsetX( (int32_t) std::floor(player->getX()) - std::max(player->getVisionRange(), player->getDetectionRange())),
      offsetY( (int32_t) std::floor(player->getY()) - std::max(player->getVisionRange(), player->getDetectionRange())),
      offsetZ(map->offsetZ),
      sizeX( (int32_t) std::floor(player->getX()) + std::max(player->getVisionRange(), player->getDetectionRange()) + 1 - offsetX),
      sizeY( (int32_t) std::floor(player->getY()) + std::max(player->getVisionRange(), player->getDetectionRange()) + 1 - offsetY),
      sizeZ(map->sizeZ),
      outside(map->outside),
      lights(sizeY),
      tiles(std::map<Vector3, Block *>()),
      light(map->light),
      rotation(0)
    {
      std::set<int64_t> ids = std::set<int64_t>();
      ids.insert(id);
      for(int32_t y = 0; y < sizeY; y++) {
        for(int32_t x = 0; x < sizeX; x++) {
          Block * block = map->getBlock(x + offsetX, y + offsetY); 
          if(block != nullptr && block->name != "TXT_VOID") {
            tiles[y][x] = block;
            lights[y][x] = map->getLight(x + offsetX, y + offsetY);
          }
          else {
            tiles[y][x] = world->getBlock(x + offsetX, y + offsetY, offsetZ);
            if(tiles[y][x] == nullptr) {
              tiles[y][x] = (Block *) database->getBlock("TXT_VOID");
              lights[y][x] = 0;
            }
            else {
              lights[y][x] = world->getLight(x + offsetX, y + offsetY, offsetZ);
              ids.insert(world->getMap(x + offsetX, y + offsetY, offsetZ)->id);
            }
          }
        }
      }
      characters = std::list<Character *>();
      projectiles = std::list<Projectile *>();
      furnitures = std::list<Furniture *>();
      for(int32_t used_map_id : ids) {
        Map * used_map = world->getMap(used_map_id);
        for(Furniture * f : used_map->furnitures) {
          if(f->getX() - offsetX >= 0 && f->getX() - offsetX < sizeX && f->getY() - offsetY >= 0 && f->getY() - offsetY < sizeY) {
            furnitures.push_back(f);
          }
        }
      }
      loots = std::list<Loot *>();
      mist_nb = canSee(player, (Block *) database->getBlock("TXT_MIST"));
      for(Furniture * f : std::list<Furniture *>(furnitures)) {
        if(getBlock(f->getX(), f->getY())->name == "TXT_MIST") {
          furnitures.remove(f);
        }
      }
      for(int32_t used_map_id : ids) {
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
*/

/*
    Map(Map * map, Projectile * projectile, Database * database, World * world):
      name(map->name),
      baseName(map->name),
      id(map->id),
      offsetX( (int32_t) std::floor(projectile->getX()) - projectile->getSpeed()),
      offsetY( (int32_t) std::floor(projectile->getY()) - projectile->getSpeed()),
      offsetZ(map->offsetZ),
      sizeX( (int32_t) std::floor(projectile->getX()) + projectile->getSpeed() + 1 - offsetX),
      sizeY( (int32_t) std::floor(projectile->getY()) + projectile->getSpeed() + 1 - offsetY),
      sizeZ(map->sizeZ),
      outside(map->outside),
      lights(sizeY),
      tiles(sizeY),
      light(map->light),
      rotation(0)
    {
      std::set<int32_t> ids = std::set<int32_t>();
      ids.insert(id);
      for(int32_t i = 0; i < sizeY; i++) {
        tiles[i] = std::vector<Block *>(sizeX);
      }
      for(int32_t y = 0; y < sizeY; y++) {
        for(int32_t x = 0; x < sizeX; x++) {
          Block * block = map->getBlock(x + offsetX, y + offsetY); 
          if(block != nullptr && block->name != "TXT_VOID") {
            tiles[y][x] = block;
          }
          else {
            tiles[y][x] = world->getBlock(x + offsetX, y + offsetY, offsetZ);
            if(tiles[y][x] == nullptr) {
              tiles[y][x] = (Block *) database->getBlock("TXT_VOID");
            }
            else {
              ids.insert(world->getMap(x + offsetX, y + offsetY, offsetZ)->id);
            }
          }
        }
      }
      characters = std::list<Character *>();
      for(int32_t used_map_id : ids) {
        Map * used_map = world->getMap(used_map_id);
        for(Character * c : used_map->characters) {
          if(c->getX() - offsetX >= 0 && c->getX() - offsetX < sizeX && c->getY() - offsetY >= 0 && c->getY() - offsetY < sizeY) {
            characters.push_back(c);
          }
        }
      }
    }
    */
    std::list<Character *> getCharacters();
    std::list<Projectile *> getProjectiles();
    std::list<Furniture *> getFurnitures();
    std::list<Loot *> getLoots();
    bool allowedCoords(int64_t x, int64_t y);
    bool allowedCoords(float x, float y);
    Block * getBlock(const MathUtil::Vector3i coord);
    int64_t getLight(int64_t x, int64_t y);
    int64_t getLight(float x, float y);
    Furniture * getFurniture(int64_t x, int64_t y);
    Furniture * getFurniture(float x, float y);
    bool isOpaque(int32_t x, int32_t y);
    bool isOpaque(float x, float y);
    bool isSolid(int32_t x, int32_t y);
    bool isSolid(float x, float y);
    bool isUnwalkable(int32_t x, int32_t y);
    bool isUnwalkable(float x, float y);
    void calculateLights();
    void propagateLight(int32_t x, int32_t y);
    void applyDayLight(int32_t light);
    bool canSee(Character * watcher, Character * target);
    int32_t canSee(Character * watcher, Block * mist);
    int32_t getMistNb();
    int32_t getRotation();
    void setBlock(const MathUtil::Vector3i coord, Block * block);
    void crumble(int32_t x, int32_t y);
    void addCharacter(Character * c);
    void addProjectile(Projectile * p);
    void addFurniture(Furniture * f);
    void addLoot(Loot * l);
    void removeCharacter(Character * c);
    void killCharacter(Character * killer, Character * victim);
    void removeProjectile(Projectile * p);
    void destroyProjectile(Projectile * p);
    void removeLoot(Loot * l);
    void destroyLoot(Loot * l);
    void takeLoot(Character * c, int32_t mode);
    float getMoveCost(Character * c, float ori_x, float ori_y, float x, float y);
    bool tryMove(Character * c, float destX, float destY);
    float move(Character * c, float y, float x, World * world);
    float move(Character * c, float orientation, float destX, float destY, float ap, World * world);
    float actProjectile(Projectile * p, Adventure * adventure, float speed);
    void clearProjectiles();
    std::string block_to_string(int32_t x, int32_t y);
    bool operator == (const Map& m) const { return id == m.id; }
    bool operator != (const Map& m) const { return !operator==(m); }
  private:
    int32_t light;
    int32_t mist_nb = 0;
    int32_t rotation;
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Furniture *> furnitures;
    std::list<Loot *> loots;
    std::map<const MathUtil::Vector3i, Block *> blocks;
    std::vector<std::vector<int32_t>> lights;
};

#endif // _MAP_H_
