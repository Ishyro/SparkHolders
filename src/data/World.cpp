#include "data/Character.h"
#include "data/Projectile.h"
#include "data/Tile.h"
#include "data/Map.h"

#include "data/items/Item.h"

#include "util/MapUtil.h"

#include "data/World.h"

void World::addMap(Map * map) {
  std::set<long> region = std::set<long>();
  std::set<long> neighbours_group = std::set<long>();
  region.insert(map->id);
  regions.insert(std::make_pair(map->id, region));
  neighbours_group.insert(map->id);
  neighbours.insert(std::make_pair(map->id, neighbours_group));
  for(int y = map->offsetY; y < map->offsetY + map->sizeY; y++) {
    for(int x = map->offsetX; x < map->offsetX + map->sizeX; x++) {
      if(map->getTile(x, y)->name == "TXT_VOID") {
        Map * to_add = getMap(x, y, map->offsetZ);
        if(to_add != nullptr) {
          regions.at(map->id).insert(to_add->id);
        }
      }
      // Border
      if(y == map->offsetY || y == map->offsetY + map->sizeY - 1 || x == map->offsetX || x == map->offsetX + map->sizeX - 1) {
        if(!(map->getTile(x, y)->opaque && map->getTile(x, y)->solid)) {
          if(y == map->offsetY) {
            Map * to_add = getMap(x, y - 1, map->offsetZ);
            if(to_add != nullptr) {
              neighbours.at(map->id).insert(to_add->id);
              neighbours.at(to_add->id).insert(map->id);
            }
          }
          if(y == map->offsetY + map->sizeY - 1) {
            Map * to_add = getMap(x, y + 1, map->offsetZ);
            if(to_add != nullptr) {
              neighbours.at(map->id).insert(to_add->id);
              neighbours.at(to_add->id).insert(map->id);
            }
          }
          if(x == map->offsetX) {
            Map * to_add = getMap(x - 1, y, map->offsetZ);
            if(to_add != nullptr) {
              neighbours.at(map->id).insert(to_add->id);
              neighbours.at(to_add->id).insert(map->id);
            }
          }
          if(x == map->offsetX + map->sizeX - 1) {
            Map * to_add = getMap(x + 1, y, map->offsetZ);
            if(to_add != nullptr) {
              neighbours.at(map->id).insert(to_add->id);
              neighbours.at(to_add->id).insert(map->id);
            }
          }
        }
      }
    }
  }
  for(long id1 : regions.at(map->id)) {
    for(long id2 : regions.at(map->id)) {
      regions.at(id1).insert(id2);
      regions.at(id2).insert(id1);
    }
  }
  maps.insert(std::pair<int, Map *>(map->id, map));
}

void World::addMapLink(MapLink * link) {
  links.push_back(link);
}

std::set<long> World::getRegion(long map_id) {
  return regions.at(map_id);
}

std::set<long> World::getNeighbours(long map_id) {
  return neighbours.at(map_id);
}

Map * World::getMap(long map_id) { return maps.at(map_id); }

Map * World::getMap(std::string name) {
    for (auto pair = maps.begin(); pair != maps.end(); pair++) {
      if(pair->second->name == name) {
        return pair->second;
      }
    }
    return nullptr;
}

std::list<Map *> World::getMaps() {
  std::list<Map *> to_return = std::list<Map *>();
  for(auto pair : maps) {
    to_return.push_back(pair.second);
  }
  return to_return;
}

MapLink * World::getMapLink(int x, int y, long mapId) {
  for(MapLink * link : links) {
    if((link->map1->id == mapId && link->x1 == x && link->y1 == y) ||
      (link->map2->id == mapId && link->x2 == x && link->y2 == y)) {
        return link;
    }
  }
  return nullptr;
}

std::list<Character *> World::getCharacters() {
  std::list<Character *> characters = std::list<Character *>();
  for (auto pair : maps) {
    for (Character * character : pair.second->getCharacters()) {
      // no check on player_character, because we want mind controlled players to act as npc
      // this imply that the player AI needs to send nullptr when asked for an Action
      // otherwise players will have 2 Actions per round
      characters.push_back(character);
    }
  }
  return characters;
}

Character * World::getCharacter(long id) {
  if(id != 0) {
    for(Character * c : getCharacters()) {
      if(id == c->id) {
        return c;
      }
    }
  }
  return nullptr;
}

Map * World::getMap(int x, int y, int z) {
  for (auto pair : maps) {
    if(pair.second->offsetZ == z) {
      Tile * tile = pair.second->getTile(x, y);
      if(tile != nullptr && tile->name != "TXT_VOID") {
        return pair.second;
      }
    }
  }
  return nullptr;
}

Map * World::getMap(float x, float y, float z) {
  for (auto pair : maps) {
    if(pair.second->offsetZ == z) {
      Tile * tile = pair.second->getTile(x, y);
      if(tile != nullptr && tile->name != "TXT_VOID") {
        return pair.second;
      }
    }
  }
  return nullptr;
}

Tile * World::getTile(int x, int y, int z) {
  for (auto pair : maps) {
    if(pair.second->offsetZ == z) {
      Tile * tile = pair.second->getTile(x, y);
      if(tile != nullptr && tile->name != "TXT_VOID") {
        return tile;
      }
    }
  }
  return nullptr;
}

Tile * World::getTile(float x, float y, float z) {
  for (auto pair : maps) {
    if(pair.second->offsetZ == z) {
      Tile * tile = pair.second->getTile(x, y);
      if(tile != nullptr && tile->name != "TXT_VOID") {
        return tile;
      }
    }
  }
  return nullptr;
}
int World::getLight(int x, int y, int z) {
  for (auto pair : maps) {
    if(pair.second->offsetZ == z) {
      Tile * tile = pair.second->getTile(x, y);
      if(tile != nullptr && tile->name != "TXT_VOID") {
        return pair.second->getLight(x, y);
      }
    }
  }
  return 0;
}
int World::getLight(float x, float y, float z) {
  for (auto pair : maps) {
    if(pair.second->offsetZ == z) {
      Tile * tile = pair.second->getTile(x, y);
      if(tile != nullptr && tile->name != "TXT_VOID") {
        return pair.second->getLight(x, y);
      }
    }
  }
  return 0;
}


float World::setPathToTarget(long map_id, float x, float y, Target * target) {
  float target_x;
  float target_y;
  switch(target->type) {
    case TARGET_TILE:
      target_x = target->x + 0.5F;
      target_y = target->y + 0.5F;
      break;
    case TARGET_COORDINATES:
      target_x = target->x;
      target_y = target->y;
      break;
    case TARGET_CHARACTER: {
      Character * other = getCharacter(target->id);
      target_x = other->getX();
      target_y = other->getY();
      break;
    }
    default: ;
  }
  return MapUtil::getOrientationToTarget(x, y, target_x, target_y);
}

float World::distance(long map_id, float x, float y, Target * target) {
  return 0.F;
}
