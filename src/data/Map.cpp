#include <sstream>

#include "data/Adventure.h"
#include "data/Effect.h"
#include "data/Item.h"
#include "data/Weapon.h"

#include "communication/SpeechManager.h"

#include "data/Map.h"

#include "utils/String.h"
#include "utils/MapUtil.h"

std::list<Character *> Map::getCharacters() { return characters; }
std::list<Projectile *> Map::getProjectiles() { return projectiles; }
std::list<Loot *> Map::getLoots() { return loots; }
// TODO : return right tile
Tile * Map::getTile(int y, int x) { return tiles[y][x]; }
int Map::getLight(int y, int x) { return std::max(lights[y][x], light); }
void Map::applyDayLight(int light) { this->light = light; }

void Map::calculateLights() {
  // initialize
  int lightX[sizeX*sizeY];
  int lightY[sizeX*sizeY];
  int cpt = 0;
  for(int y = 0; y < sizeY; y++) {
    for(int x = 0; x < sizeX; x++) {
      lights[y][x] = tiles[y][x]->light;
      if (lights[y][x] > 0) {
        lightX[cpt] = x;
        lightY[cpt++] = y;
      }
    }
  }
  for(int i = 0; i < cpt; i++) {
    propagateLight(lightY[i],lightX[i]);
  }
}

void Map::propagateLight(int y, int x) {
  int light = lights[y][x] - 1;
  if(x > 1 && y > 1 && light > lights[y - 1][x - 1]) {
    lights[y - 1][x - 1] = light;
    propagateLight(y - 1, x - 1);
  }
  if(x > 1 && light > lights[y][x - 1]) {
    lights[y][x - 1] = light;
    propagateLight(y, x - 1);
  }
  if(x > 1 && y < sizeY - 1 && light > lights[y + 1][x - 1]) {
    lights[y + 1][x - 1] = light;
    propagateLight(y + 1, x - 1);
  }
  if(y > 1 && light > lights[y - 1][x]) {
    lights[y-1][x] = light;
    propagateLight(y - 1, x);
  }
  if(y < sizeY - 1 && light > lights[y + 1][x]) {
    lights[y + 1][x] = light;
    propagateLight(y + 1, x);
  }
  if(x < sizeX - 1 && y > 1 && light > lights[y - 1][x + 1]) {
    lights[y - 1][x + 1] = light;
    propagateLight(y - 1, x + 1);
  }
  if(x < sizeX - 1 && light > lights[y][x + 1]) {
    lights[y][x + 1] = light;
    propagateLight(y, x + 1);
  }
  if(x < sizeX - 1 && y < sizeY - 1 && light > lights[y + 1][x + 1]) {
    lights[y + 1][x + 1] = light;
    propagateLight(y + 1, x + 1);
  }
}

// map initialized with mist tiles
bool Map::canSee(Character * watcher, Character * target) {
  if(target->isInvisible() || target->isEtheral()) {
    return false;
  }
  int range = MapUtil::distance(watcher->getX(), watcher->getY(), target->getX(), target->getY());
  if(range <= watcher->getDetectionRange()) {
    return true;
  }
  if(getTile(target->getY(), target->getX())->name == "mist" || range > watcher->getVisionRange() || (watcher->getVisionPower() - target->cloakPower()) < 10 - getLight(target->getY(), target->getX())) {
    return false;
  }
  return true;
}

std::vector<std::vector<Tile *>> Map::canSee(Map * map, Character * watcher, Database * database) {
  int range = std::max(watcher->getVisionRange(), watcher->getDetectionRange());
  int originX = std::max(0, watcher->getX() - range);
  int endX = std::min(map->sizeX - 1, watcher->getX() + range);
  int originY = std::max(0, watcher->getY() - range);
  int endY = std::min(map->sizeY - 1, watcher->getY() + range);
  std::vector<std::vector<Tile *>> result = std::vector<std::vector<Tile *>>(endY - originY + 1);
  for(int index = 0; index <= endY - originY; index++) {
    result[index] = std::vector<Tile *>(endX - originX + 1);
  }
  for(int y = 0; y <= endY - originY; y++) {
    for(int x = 0; x <= endX - originX; x++) {
      result[y][x] = (Tile *) database->getTile("mist");
    }
  }
  for(int y = 0; y <= range; y++) {
    float a = (float) y / (float) range;
    std::list<MapUtil::Pair> path1 = MapUtil::getPathFromCartesianEquationFloor(a, range);
    std::list<MapUtil::Pair> path2 = MapUtil::getPathFromCartesianEquationCeil(a, range);
    // 8 times the same action with inverted coordinates
    for(MapUtil::Pair pair : path1) {
      int i = pair.x + watcher->getX() - originX;
      int j = pair.y + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(pair.y + watcher->getY(), pair.x + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(pair.y + watcher->getY(), pair.x + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path1) {
      int i = pair.x + watcher->getX() - originX;
      int j = -pair.y + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(-pair.y + watcher->getY(), pair.x + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(-pair.y + watcher->getY(), pair.x + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path1) {
      int i = -pair.x + watcher->getX() - originX;
      int j = pair.y + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(pair.y + watcher->getY(), -pair.x + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(pair.y + watcher->getY(), -pair.x + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path1) {
      int i = -pair.x + watcher->getX() - originX;
      int j = -pair.y + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(-pair.y + watcher->getY(), -pair.x + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(-pair.y + watcher->getY(), -pair.x + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path1) {
      int i = pair.y + watcher->getX() - originX;
      int j = pair.x + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(pair.x + watcher->getY(), pair.y + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(pair.x + watcher->getY(), pair.y + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path1) {
      int i = -pair.y + watcher->getX() - originX;
      int j = pair.x + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(pair.x + watcher->getY(), -pair.y + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(pair.x + watcher->getY(), -pair.y + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path1) {
      int i = pair.y + watcher->getX() - originX;
      int j = -pair.x + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(-pair.x + watcher->getY(), pair.y + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(-pair.x + watcher->getY(), pair.y + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path1) {
      int i = -pair.y + watcher->getX() - originX;
      int j = -pair.x + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(-pair.x + watcher->getY(), -pair.y + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(-pair.x + watcher->getY(), -pair.y + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    // 8 times the same action with inverted coordinates
    for(MapUtil::Pair pair : path2) {
      int i = pair.x + watcher->getX() - originX;
      int j = pair.y + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(pair.y + watcher->getY(), pair.x + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(pair.y + watcher->getY(), pair.x + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path2) {
      int i = pair.x + watcher->getX() - originX;
      int j = -pair.y + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(-pair.y + watcher->getY(), pair.x + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(-pair.y + watcher->getY(), pair.x + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path2) {
      int i = -pair.x + watcher->getX() - originX;
      int j = pair.y + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(pair.y + watcher->getY(), -pair.x + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(pair.y + watcher->getY(), -pair.x + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path2) {
      int i = -pair.x + watcher->getX() - originX;
      int j = -pair.y + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(-pair.y + watcher->getY(), -pair.x + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(-pair.y + watcher->getY(), -pair.x + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path2) {
      int i = pair.y + watcher->getX() - originX;
      int j = pair.x + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(pair.x + watcher->getY(), pair.y + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(pair.x + watcher->getY(), pair.y + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path2) {
      int i = -pair.y + watcher->getX() - originX;
      int j = pair.x + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(pair.x + watcher->getY(), -pair.y + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(pair.x + watcher->getY(), -pair.y + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path2) {
      int i = pair.y + watcher->getX() - originX;
      int j = -pair.x + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(-pair.x + watcher->getY(), pair.y + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(-pair.x + watcher->getY(), pair.y + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
    for(MapUtil::Pair pair : path2) {
      int i = -pair.y + watcher->getX() - originX;
      int j = -pair.x + watcher->getY() - originY;
      if(i < 0 || i > endX - originX || j < 0 || j > endY - originY) {
        break;
      }
      Tile * tile = map->getTile(-pair.x + watcher->getY(), -pair.y + watcher->getX());
      if(MapUtil::distance(pair.x, pair.y, 0, 0) <= watcher->getDetectionRange()) {
        result[j][i] = tile;
      }
      if(map->getLight(-pair.x + watcher->getY(), -pair.y + watcher->getX()) >= 10 - watcher->getVisionPower()) {
        result[j][i] = tile;
      }
      if(tile->opaque) {
        break;
      }
    }
  }
  return result;
}

void Map::setTile(int y, int x, Tile * tile) { tiles[y][x] = tile; }

void Map::crumble(int y, int x) {
  for(auto character : characters) {
    if(character->getX() == x && character->getY() == y) {
      if(character->type == WALL) {
        removeCharacter(character);
        delete character;
        character = nullptr;
      }
      else break;
    }
  }
}

void Map::addCharacter(Character * c) { characters.push_back(c); }
void Map::addProjectile(Projectile * p) { projectiles.push_back(p); }
void Map::addLoot(Loot * l) { loots.push_back(l); }

void Map::removeCharacter(Character * c) { characters.remove(c); }
void Map::killCharacter(Character * killer, Character * victim) {
  characters.remove(victim);
  for(Character * character : characters) {
    if(character->player_character) {
      SpeechManager::add(victim->getDeathSpeech());
      break;
    }
  }
  Loot * loot = new Loot();
  loot->weapons = std::list<Weapon *>();
  loot->items = std::list<Item *>();
  loot->ammunition = std::list<Ammunition *>();
  loot->x = victim->getX();
  loot->y = victim->getY();
  loot->gold = victim->getGold();
  for(Weapon * w : victim->getWeapons()) {
    if(w->droppable) {
      loot->weapons.push_back(w);
    }
  }
  for(Item * i : victim->getItems()) {
    if(i->droppable) {
      loot->items.push_back(i);
    }
  }
  for(Ammunition * a : victim->getAmmunitions()) {
    loot->ammunition.push_back(a);
  }
  for(Item * i : victim->getLoot()) {
    if(i->droppable) {
      loot->items.push_back(i);
    }
  }
  loot->type = CORPSE;
  if(killer != victim) {
    killer->gainXP(victim->getXP() / 2);
  } else {
    int damages[DAMAGE_TYPE_NUMBER] = {0};
    float damage_reductions[DAMAGE_TYPE_NUMBER] = {0.F};
    int xp = victim->getXP();
    Effect * effect = new Effect("TXT_GAIN_XP", 0, "", EXPERIENCE, INSTANT, 0, xp, damages, damage_reductions);
    std::list<Effect *> * effects = new std::list<Effect *>();
    effects->push_back(effect);
    loot->items.push_back(new Item("TXT_PERL_OF_WISDOM", false, true, UNEQUIPABLE, true, 0.F, xp * 10, *effects, damage_reductions));
    delete effects;
  }
  if(loot->gold == 0 && loot->weapons.empty() && loot->items.empty() && loot->ammunition.empty()) {
    delete loot;
  } else {
    loots.push_back(loot);
  }
  delete victim;
  victim = nullptr;
}

void Map::removeProjectile(Projectile * p) { projectiles.remove(p); }
void Map::destroyProjectile(Projectile * p) {
  projectiles.remove(p);
  delete p;
  p = nullptr;
}

void Map::removeLoot(Loot * l) { loots.remove(l); }

void Map::destroyLoot(Loot * l) {
  loots.remove(l);
  delete l;
  l = nullptr;
}

void Map::takeLoot(Character * c, int mode) {
  std::list<Loot *> to_delete = std::list<Loot *>();
  switch(mode) {
    case FOOD: {
      for(Loot * l : loots) {
        if(l != nullptr && l->x == c->getX() && l->y == c->getY()) {
          for(Item * i : l->items) {
            if(i->isFood()) {
              c->addItem(i);
              l->items.remove(i);
              break;
            }
          }
          if(l->items.size() == 0 && l->weapons.size() == 0 && l->ammunition.size() == 0 && l->gold == 0) {
            to_delete.push_back(l);
          }
        }
      }
      break;
    }
    case ALL: // 0
    default: {
      for(Loot * l : loots) {
        if(l != nullptr && l->x == c->getX() && l->y == c->getY()) {
          for(Item * i : l->items) {
            c->addItem(i);
          }
          for(Weapon * w : l->weapons) {
            c->addWeapon(w);
          }
          for(Ammunition * a : l->ammunition) {
            c->addAmmunition(a);
          }
          c->gainGold(l->gold);
          to_delete.push_back(l);
        }
      }
    }
  }
  for(Loot * l : to_delete) {
    destroyLoot(l);
  }
  to_delete.clear();
}

void Map::move(Character *c, int orientation, Adventure * adventure) {
  bool is_legal = false;
  int destX;
  int destY;
  switch(orientation) {
    case NORTH:
      if(c->getY() < sizeY - 1 && !tiles[c->getY() + 1][c->getX()]->untraversable) {
        is_legal = true;
        destX = c->getX();
        destY = c->getY() + 1;
      }
      break;
    case NORTH_EAST:
      if(c->getY() < sizeY - 1 && c->getX() < sizeX - 1 && !tiles[c->getY() + 1][c->getX() + 1]->untraversable) {
        is_legal = true;
        destX = c->getX() + 1;
        destY = c->getY() + 1;
      }
      break;
    case EAST:
      if(c->getX() < sizeX - 1 && !tiles[c->getY()][c->getX() + 1]->untraversable) {
        is_legal = true;
        destX = c->getX() + 1;
        destY = c->getY();
      }
      break;
    case SOUTH_EAST:
      if(c->getY() > 0 && c->getX() < sizeX - 1 && !tiles[c->getY() - 1][c->getX() + 1]->untraversable) {
        is_legal = true;
        destX = c->getX() + 1;
        destY = c->getY() - 1;
      }
      break;
    case SOUTH:
      if(c->getY() > 0 && !tiles[c->getY() - 1][c->getX()]->untraversable) {
        is_legal = true;
        destX = c->getX();
        destY = c->getY() - 1;
      }
      break;
    case SOUTH_WEST:
      if(c->getY() > 0 && c->getX() > 0 && !tiles[c->getY() - 1][c->getX() - 1]->untraversable) {
        is_legal = true;
        destX = c->getX() - 1;
        destY = c->getY() - 1;
      }
      break;
    case WEST:
      if(c->getX() > 0 && !tiles[c->getY()][c->getX() - 1]->untraversable) {
        is_legal = true;
        destX = c->getX() - 1;
        destY = c->getY();
      }
      break;
    case NORTH_WEST:
      if(c->getY() < sizeY - 1 && c->getX() > 0 && !tiles[c->getY() + 1][c->getX() - 1]->untraversable) {
        is_legal = true;
        destX = c->getX() - 1;
        destY = c->getY() + 1;
      }
      break;
  }
  if(is_legal) {
    Character * to_kill = nullptr;
    for(Character * other : characters) {
      if (other != nullptr && other->getX() == destX && other->getY() == destY && !other->isEtheral()) {
        if(adventure->getDatabase()->getRelation(c->getTeam(), other->getTeam()) == ENEMY) {
          c->setOrientation(orientation);
          c->attack(other);
          if(!other->isAlive()) {
            to_kill = other;
          } else {
            return;
          }
        } else {
          return; // there is a character, can't move on this position
        }
      }
    }
    if(to_kill != nullptr) {
      characters.remove(to_kill);
      killCharacter(c, to_kill);
      return;
    }
    c->move(orientation);
  } else {
    c->setOrientation(orientation);
  }
}

bool Map::actProjectile(Projectile * p, Adventure * adventure) {
  for(int i = 0; i < p->getSpeed(); i++) {
    if(p->isAtDest()) {
      p->setLost(true);
      if(p->getArea() > 1) {
        p->attack_multiple_targets(characters, adventure);
        for(Character * c : characters) {
          if(c != nullptr && !c->isAlive()) {
            killCharacter(p->getOwner(), c);
          }
        }
        p->move();
        continue;
      }
    }
    for(Character * c : characters) {
      if(c != nullptr && c->getX() == p->getX() && c->getY() == p->getY() && !c->isEtheral()) {
        if(p->getArea() > 1) {
          p->attack_multiple_targets(characters, adventure);
          for(Character * c : characters) {
            if(!c->isAlive()) {
              killCharacter(p->getOwner(), c);
            }
          }
          break;
        } else {
          p->attack_single_target(c, adventure);
          if(!c->isAlive()) {
            killCharacter(p->getOwner(), c);
          }
        }
        p->move();
        break;
      }
    }
    if(p->noDamage()) {
      return true;
    }
    p->move();
  }
  return false;
}

void Map::actAllProjectiles(Adventure * adventure) {
  std::list<Projectile *> new_projectiles;
  for(Projectile * projectile : projectiles) {
    if(projectile != nullptr) {
      if(actProjectile(projectile, adventure)) {
        delete projectile;
      } else {
        new_projectiles.push_back(projectile);
      }
    }
  }
  projectiles.clear();
  projectiles = new_projectiles;
}

std::string Map::to_string(Character * player, Adventure * adventure) {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, adventure->getTime());
  String::insert(ss, name);
  String::insert_int(ss, offsetX);
  String::insert_int(ss, offsetY);
  String::insert_int(ss, sizeX);
  String::insert_int(ss, sizeY);
  String::insert_bool(ss, outside);
  for(int y = 0; y < sizeY; y++) {
    for(int x = 0; x < sizeX; x++) {
      String::insert_int(ss, x);
      String::insert_int(ss, y);
      String::insert(ss, getTile(y, x)->name);
      String::insert_bool(ss, getTile(y, x)->untraversable);
      String::insert_bool(ss, getTile(y, x)->opaque);
      String::insert_int(ss, getLight(y, x));
    }
  }
  std::stringstream * ss_characters = new std::stringstream();
  for(Character * character : characters) {
    String::insert(ss_characters, character->to_string(offsetY, offsetX));
    String::insert_int(ss_characters, adventure->getDatabase()->getRelation(character->getTeam(), player->getTeam()));
  }
  String::insert(ss, ss_characters->str());
  delete ss_characters;
  std::stringstream * ss_projectiles = new std::stringstream();
  for(Projectile * projectile : projectiles) {
    String::insert(ss_projectiles, projectile->to_string(offsetY, offsetX));
  }
  String::insert(ss, ss_projectiles->str());
  delete ss_projectiles;
  std::stringstream * ss_loots = new std::stringstream();
  for(Loot * loot : loots) {
    String::insert_int(ss_loots, loot->type);
    String::insert_int(ss_loots, loot->x - offsetX);
    String::insert_int(ss_loots, loot->y - offsetY);
  }
  String::insert(ss, ss_loots->str());
  delete ss_loots;
  std::stringstream * ss_speeches = new std::stringstream();
  for(Speech * speech : SpeechManager::get()) {
    String::insert(ss_speeches, speech->to_string());
  }
  String::insert(ss, ss_speeches->str());
  delete ss_speeches;
  std::string result = ss->str();
  delete ss;
  return result;
}

MapDisplay * Map::from_string(std::string to_read) {
  MapDisplay * display = new MapDisplay();
  std::stringstream * ss = new std::stringstream(to_read);
  display->time = String::extract(ss);
  display->name = String::extract(ss);
  display->offsetX = String::extract_int(ss);
  display->offsetY = String::extract_int(ss);
  display->sizeX = String::extract_int(ss);
  display->sizeY = String::extract_int(ss);
  display->outside = String::extract_bool(ss);
  display->tiles = std::vector<std::vector<Tile *>>(display->sizeY);
  for(int i = 0; i < display->sizeY; i++) {
    display->tiles[i] = std::vector<Tile *>(display->sizeX);
  }
  for(int y = 0; y < display->sizeY; y++) {
    for(int x = 0; x < display->sizeX; x++) {
      int i = String::extract_int(ss);
      int j = String::extract_int(ss);
      std::string name = String::extract(ss);
      bool untraversable = String::extract_bool(ss);
      bool opaque = String::extract_bool(ss);
      int light = String::extract_int(ss);
      display->tiles[j][i] = new Tile(name, untraversable, opaque, light);
    }
  }
  std::stringstream * ss_characters = new std::stringstream(String::extract(ss));
  while(ss_characters->rdbuf()->in_avail() != 0) {
    CharacterDisplay * characterDisplay = Character::from_string(String::extract(ss_characters));
    characterDisplay->teamRelation = String::extract_int(ss_characters);
    display->characters.push_back(characterDisplay);
  }
  delete ss_characters;
  std::stringstream * ss_projectiles = new std::stringstream(String::extract(ss));
  while(ss_projectiles->rdbuf()->in_avail() != 0) {
    display->projectiles.push_back(Projectile::from_string(String::extract(ss_projectiles)));
  }
  delete ss_projectiles;
  std::stringstream * ss_loots = new std::stringstream(String::extract(ss));
  while(ss_loots->rdbuf()->in_avail() != 0) {
    Loot * loot = new Loot();
    loot->type = String::extract_int(ss_loots);
    loot->x = String::extract_int(ss_loots);
    loot->y = String::extract_int(ss_loots);
    display->loots.push_back(loot);
  }
  delete ss_loots;
  std::stringstream * ss_speeches = new std::stringstream(String::extract(ss));
  while(ss_speeches->rdbuf()->in_avail() != 0) {
    display->speeches.push_back(Speech::from_string(String::extract(ss_speeches)));
  }
  delete ss_speeches;
  delete ss;
  return display;
}
