#include "data/Character.h"
#include "data/Projectile.h"
#include "data/Tile.h"
#include "data/Map.h"

#include "data/items/Item.h"

#include "util/MapUtil.h"

#include "data/World.h"

void World::addMap(Map * map) {
  maps.insert(std::pair<int, Map *>(map->id, map));
}

void World::addMapLink(MapLink * link) {
  links.push_back(link);
}

Map * World::getMap(int map_id) { return maps.at(map_id); }

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

MapLink * World::getMapLink(int x, int y, int mapId) {
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


float World::setPathToTarget(int map_id, float x, float y, Target * target) {
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

float World::distance(int map_id, float x, float y, Target * target) {
  return 0.F;
}
