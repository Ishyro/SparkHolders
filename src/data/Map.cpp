#include "src/data/Map.h"

std::list<Character *> Map::getCharacters() {return characters;}
std::list<Projectile *> Map::getProjectiles() {return projectiles;}
std::list<Loot *> Map::getLoots() {return loots;}
// TODO : return right tile
Tile * Map::getTile(long x, long y) {return tiles[x][y];}

void Map::crumble(long x, long y) {
  tiles[x][y] = tiles[x][y]->crumble();
}

void Map::addCharacter(Character * c) {characters.push_front(c);}
void Map::addProjectile(Projectile * p) {projectiles.push_front(p);}
void Map::addLoot(Loot * l) {loots.push_front(l);}

void Map::removeCharacter(Character * c) {characters.remove(c);}
void Map::killCharacter(Character * c) {
  characters.remove(c);
  speech_manager->add(c->death_speech);
  delete c;
}

void Map::removeProjectile(Projectile * p) {projectiles.remove(p);}
void Map::destroyProjectile(Projectile * p) {
  projectiles.remove(p);
  delete p;
}

void Map::removeLoot(Loot * l) {loots.remove(l);}

void Map::destroyLoot(Loot * l) {
  loots.remove(l);
  //delete l;
}

void Map::takeLoot(Character * c, Loot * l) {
  loots.remove(l);
  for (auto i : l->items) {
    c->addItem(i);
  }
  for (auto w : l->weapons) {
    c->addWeapon(w);
  }
  //delete l;
}
