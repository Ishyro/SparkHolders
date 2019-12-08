#include "src/data/World.h"

void World::addMap(Map * map) {
  maps.insert(std::pair<long, Map *>(map->id, map));
}

void World::addMapLink(MapLink * link) {
  links.push_front(link);
}

Map * World::getMap(int map_id) {return maps.at(map_id);}
long World::getTick() {return tick;}
void World::incrTick() {tick++;}
