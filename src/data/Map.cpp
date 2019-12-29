#include "src/data/Map.h"

std::list<Character *> Map::getCharacters() {return characters;}
std::list<Projectile *> Map::getProjectiles() {return projectiles;}
std::list<Loot *> Map::getLoots() {return loots;}
// TODO : return right tile
Tile * Map::getTile(long x, long y) {return tiles[x][y];}
bool Map::isTileLighted(long x, long y) {return lighted[x][y];}

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
  Loot * loot = malloc(sizeof(Loot));
  loot->x = c->getX();
  loot->y = c->getY();
  loot->gold = c->getGold();
  for (auto w : c->getWeapons()) {
    loot->weapons.push_front(w);
  }
  for (auto i : c->getStuff()) {
    loot->items.push_front(i);
  }
  loot->type = CORPSE;
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

void Map::tryHit(Projectile * p, World * world) {
  int p_x = p->getX();
  int p_y = p->getY();
  for (auto c : characters) {
    if (c->getX() == p_x && c->getY() == p_y) {
      switch(p->target_type) {
        case TILE:
          p->attack_single_target(c, world);
          p->setLost(true);
          break;
        case CHARACTER:
          p->attack_single_target(c, world);
          break;
        case MULTIPLE_TILES:
        case MULTIPLE_CHARACTERS:
          p->attack_multiple_targets(characters, world);
          break;
        default:

      }
      break;
    }
  }
}
