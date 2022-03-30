#include "data/World.h"

void World::addMap(Map * map) {
  maps.insert(std::pair<long, Map *>(map->id, map));
}

void World::addMapLink(MapLink * link) {
  links.push_front(link);
}

Map * World::getMap(long map_id) { return maps.at(map_id); }

Tile * World::getTile(std::string name) {
  for(auto it = this->tiles.begin(); it != this->tiles.end(); it++) {
    if((*it)->name == name) {
      return *it;
    }
  }
  return nullptr;
}
