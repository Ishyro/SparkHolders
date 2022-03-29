#include "data/Map.h"

std::list<Character *> Map::getCharacters() { return characters; }
std::list<Projectile *> Map::getProjectiles() { return projectiles; }
std::list<Loot *> Map::getLoots() { return loots; }
// TODO : return right tile
Tile * Map::getTile(long x, long y) { return tiles[x][y]; }
bool Map::isTileLighted(long x, long y) { return lighted[x][y]; }

void Map::setTile(long x, long y, Tile * tile) { tiles[x][y] = tile; }

void Map::crumble(long x, long y) {
  for (auto character : characters) {
    if (character->getX() == x && character->getY() == y) {
      if (character->type == WALL) {
        removeCharacter(character);
        delete character;
      }
      else break;
    }
  }
}

void Map::addCharacter(Character * c) { characters.push_front(c); }
void Map::addProjectile(Projectile * p) { projectiles.push_front(p); }
void Map::addLoot(Loot * l) { loots.push_front(l); }

void Map::removeCharacter(Character * c) { characters.remove(c); }
void Map::killCharacter(Character * victim, Character * killer) {
  characters.remove(victim);
  SpeechManager::add(victim->death_speech);
  Loot * loot = (Loot *) malloc(sizeof(Loot));
  loot->x = victim->getX();
  loot->y = victim->getY();
  loot->gold = victim->getGold();
  for (auto w : victim->getWeapons()) {
    loot->weapons.push_front(w);
  }
  for (auto i : victim->getStuff()) {
    loot->items.push_front(i);
  }
  loot->type = CORPSE;
  killer->gainXP(victim->getXP() / 2);
  delete victim;
}

void Map::removeProjectile(Projectile * p) { projectiles.remove(p); }
void Map::destroyProjectile(Projectile * p) {
  projectiles.remove(p);
  delete p;
}

void Map::removeLoot(Loot * l) { loots.remove(l); }

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

void Map::tryHit(Projectile * p, Adventure * adventure) {
  int p_x = p->getX();
  int p_y = p->getY();
  for (auto c : characters) {
    if (c->getX() == p_x && c->getY() == p_y) {
      switch(p->target_type) {
        case SINGLE_TILE:
          p->attack_single_target(c, adventure);
          p->setLost(true);
          break;
        case SINGLE_CHARACTER:
          p->attack_single_target(c, adventure);
          break;
        case MULTIPLE_TILES:
        case MULTIPLE_CHARACTERS:
          p->attack_multiple_targets(characters, adventure);
          break;
        default:
          break;
      }
      break;
    }
  }
}
