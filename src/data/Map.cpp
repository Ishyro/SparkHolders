#include <sstream>

#include "data/Adventure.h"
#include "data/Effect.h"
#include "data/Way.h"
#include "data/World.h"

#include "data/items/Item.h"
#include "data/items/WeaponItem.h"
#include "data/items/SerialItem.h"

#include "communication/SpeechManager.h"

#include "data/Map.h"

#include "util/String.h"
#include "util/MapUtil.h"

std::list<Character *> Map::getCharacters() { return characters; }
std::list<Projectile *> Map::getProjectiles() { return projectiles; }
std::list<Loot *> Map::getLoots() { return loots; }
// TODO : return right tile
Tile * Map::getTile(int y, int x) { return tiles[y][x]; }
Tile * Map::getTile(float y, float x) { return tiles[(int) std::floor(y)][(int) std::floor(x)]; }
int Map::getLight(int y, int x) { return std::max(lights[y][x], light); }
int Map::getLight(float y, float x) { return std::max(lights[(int) std::floor(y)][(int) std::floor(x)], light); }
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
  for(Character * character : characters) {
    int light = character->getLight();
    int x = character->getX();
    int y = character->getY();
    if(light > lights[y][x]) {
      lights[y][x] = light;
      lightX[cpt] = x;
      lightY[cpt++] = y;
    }
  }
  for(Projectile * projectile : projectiles) {
    int light = projectile->getLight();
    int x = projectile->getX();
    int y = projectile->getY();
    if(light > lights[y][x]) {
      lights[y][x] = light;
      lightX[cpt] = x;
      lightY[cpt++] = y;
    }
  }
  for(int i = 0; i < cpt; i++) {
    propagateLight(lightY[i],lightX[i]);
  }
  //new_light = false;
}

void Map::propagateLight(int y, int x) {
  int light = lights[y][x] - 1;
  if(x > 1 && y > 1 && light > lights[y - 1][x - 1]) {
    lights[y - 1][x - 1] = light;
    if(!getTile(y - 1, x - 1)->opaque) {
      propagateLight(y - 1, x - 1);
    }
  }
  if(x > 1 && light > lights[y][x - 1]) {
    lights[y][x - 1] = light;
    if(!getTile(y, x - 1)->opaque) {
      propagateLight(y, x - 1);
    }
  }
  if(x > 1 && y < sizeY - 1 && light > lights[y + 1][x - 1]) {
    lights[y + 1][x - 1] = light;
    if(!getTile(y + 1, x - 1)->opaque) {
      propagateLight(y + 1, x - 1);
    }
  }
  if(y > 1 && light > lights[y - 1][x]) {
    lights[y - 1][x] = light;
    if(!getTile(y - 1, x)->opaque) {
      propagateLight(y - 1, x);
    }
  }
  if(y < sizeY - 1 && light > lights[y + 1][x]) {
    lights[y + 1][x] = light;
    if(!getTile(y + 1, x)->opaque) {
      propagateLight(y + 1, x);
    }
  }
  if(x < sizeX - 1 && y > 1 && light > lights[y - 1][x + 1]) {
    lights[y - 1][x + 1] = light;
    if(!getTile(y - 1, x + 1)->opaque) {
      propagateLight(y - 1, x + 1);
    }
  }
  if(x < sizeX - 1 && light > lights[y][x + 1]) {
    lights[y][x + 1] = light;
    if(!getTile(y, x + 1)->opaque) {
      propagateLight(y, x + 1);
    }
  }
  if(x < sizeX - 1 && y < sizeY - 1 && light > lights[y + 1][x + 1]) {
    lights[y + 1][x + 1] = light;
    if(!getTile(y + 1, x + 1)->opaque) {
      propagateLight(y + 1, x + 1);
    }
  }
}

// map initialized with mist tiles
bool Map::canSee(Character * watcher, Character * target) {
  if(target->isInvisible() || target->isEtheral()) {
    return false;
  }
  float range = MapUtil::distance(watcher->getX(), watcher->getY(), target->getX(), target->getY());
  if(range <= watcher->getDetectionRange()) {
    return true;
  }
  if(getTile(target->getY(), target->getX())->name == "TXT_MIST" || range > watcher->getVisionRange() || (watcher->getVisionPower() - target->cloakPower()) < 10 - getLight(target->getY(), target->getX())) {
    return false;
  }
  return true;
}

std::vector<std::vector<Tile *>> Map::canSee(Map * map, Character * watcher, Database * database) {
  int range = std::max(watcher->getVisionRange(), watcher->getDetectionRange());
  int originX = std::max(0, (int) std::floor(watcher->getX()) - range);
  int endX = std::min(map->sizeX - 1, (int) std::floor(watcher->getX()) + range);
  int originY = std::max(0, (int) std::floor(watcher->getY()) - range);
  int endY = std::min(map->sizeY - 1, (int) std::floor(watcher->getY()) + range);
  std::vector<std::vector<Tile *>> result = std::vector<std::vector<Tile *>>(endY - originY + 1);
  for(int index = 0; index <= endY - originY; index++) {
    result[index] = std::vector<Tile *>(endX - originX + 1);
  }
  for(int y = 0; y <= endY - originY; y++) {
    for(int x = 0; x <= endX - originX; x++) {
      result[y][x] = (Tile *) database->getTile("TXT_MIST");
    }
  }
  for(int y = 0; y <= range; y++) {
    float a = (float) y / (float) range;
    std::list<MapUtil::Pair> path = MapUtil::getPathFromCartesianEquation(a, range);
    // 8 times the same action with inverted coordinates
    for(MapUtil::Pair pair : path) {
      int i = std::floor(pair.x) + std::floor(watcher->getX()) - originX;
      int j = std::floor(pair.y) + std::floor(watcher->getY()) - originY;
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
    for(MapUtil::Pair pair : path) {
      int i = std::floor(pair.x) + std::floor(watcher->getX()) - originX;
      int j = -std::floor(pair.y) + std::floor(watcher->getY()) - originY;
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
    for(MapUtil::Pair pair : path) {
      int i = -std::floor(pair.x) + std::floor(watcher->getX()) - originX;
      int j = std::floor(pair.y) + std::floor(watcher->getY()) - originY;
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
    for(MapUtil::Pair pair : path) {
      int i = -std::floor(pair.x) + std::floor(watcher->getX()) - originX;
      int j = -std::floor(pair.y) + std::floor(watcher->getY()) - originY;
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
    for(MapUtil::Pair pair : path) {
      int i = std::floor(pair.y) + std::floor(watcher->getX()) - originX;
      int j = std::floor(pair.x) + std::floor(watcher->getY()) - originY;
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
    for(MapUtil::Pair pair : path) {
      int i = -std::floor(pair.y) + std::floor(watcher->getX()) - originX;
      int j = std::floor(pair.x) + std::floor(watcher->getY()) - originY;
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
    for(MapUtil::Pair pair : path) {
      int i = std::floor(pair.y) + std::floor(watcher->getX()) - originX;
      int j = -std::floor(pair.x) + std::floor(watcher->getY()) - originY;
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
    for(MapUtil::Pair pair : path) {
      int i = -std::floor(pair.y) + std::floor(watcher->getX()) - originX;
      int j = -std::floor(pair.x) + std::floor(watcher->getY()) - originY;
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
  // fix artifacts
  for(int y = watcher->getY() - originY + 1; y <= endY - originY; y++) {
    for(int x = watcher->getX() - originX + 1; x <= endX - originX; x++) {
      if(result[y][x]->name == "TXT_MIST" && map->getTile(y + originY, x + originX)->opaque && !(result[y-1][x]->opaque && result[y][x-1]->opaque)) {
        result[y][x] = map->getTile(y + originY, x + originX);
      }
    }
  }
  for(int y = watcher->getY() - originY + 1; y <= endY - originY; y++) {
    for(int x = watcher->getX() - originX - 1; x >= 0; x--) {
      if(result[y][x]->name == "TXT_MIST" && map->getTile(y + originY, x + originX)->opaque && !(result[y-1][x]->opaque && result[y][x+1]->opaque)) {
        result[y][x] = map->getTile(y + originY, x + originX);
      }
    }
  }
  for(int y = watcher->getY() - originY - 1; y >= 0; y--) {
    for(int x = watcher->getX() - originX + 1; x <= endX - originX; x++) {
      if(result[y][x]->name == "TXT_MIST" && map->getTile(y + originY, x + originX)->opaque && !(result[y+1][x]->opaque && result[y][x-1]->opaque)) {
        result[y][x] = map->getTile(y + originY, x + originX);
      }
    }
  }
  for(int y = watcher->getY() - originY - 1; y >= 0; y--) {
    for(int x = watcher->getX() - originX - 1; x >= 0; x--) {
      if(result[y][x]->name == "TXT_MIST" && map->getTile(y + originY, x + originX)->opaque && !(result[y+1][x]->opaque && result[y][x+1]->opaque)) {
        result[y][x] = map->getTile(y + originY, x + originX);
      }
    }
  }
  return result;
}

void Map::setTile(int y, int x, Tile * tile) { tiles[y][x] = tile; }
void Map::setTile(float y, float x, Tile * tile) { tiles[(int) std::floor(y)][(int) std::floor(x)] = tile; }

void Map::crumble(int y, int x) {
  for(Character * character : characters) {
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
  loot->items = std::list<Item *>();
  loot->x = victim->getX();
  loot->y = victim->getY();
  loot->gold = victim->getGold();
  for(Item * i : victim->getItems()) {
    if(i->droppable) {
      loot->items.push_back(i);
    }
  }
  for(Item * i : victim->getRace()->getLoot()) {
    if(i->droppable) {
      loot->items.push_back(i);
    }
  }
  loot->type = CORPSE;
  if(killer != victim) {
    killer->gainXP(victim->getXP() / 2);
  }
  else {
    int damages[DAMAGE_TYPE_NUMBER] = {0};
    float damage_reductions[DAMAGE_TYPE_NUMBER] = {0.F};
    int xp = victim->getXP();
    Effect * effect = new Effect("TXT_GAIN_XP", ++effect::id_cpt, victim->getLevel(), "", EXPERIENCE, INSTANT_DURATION, xp, 0, damages, damage_reductions);
    std::list<Effect *> * effects = new std::list<Effect *>();
    effects->push_back(effect);
    loot->items.push_back(new SerialItem("TXT_PERL_OF_WISDOM", ++item::id_cpt, ITEM_CONSUMABLE, ITEM_POTION, xp, xp, 1.F, xp * 10, true, true, true, 5, *effects, 1));
    delete effects;
  }
  if(loot->gold == 0 && loot->items.empty()) {
    delete loot;
  }
  else {
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
            if(i->type == ITEM_CONSUMABLE && ( (SerialItem *) i)->isFood()) {
              c->addItem(i);
              l->items.remove(i);
              break;
            }
          }
          if(l->items.size() == 0 && l->gold == 0) {
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

float Map::getMoveCost(Character * c, float y, float x) {
  if(c->isFlying()) {
    return MapUtil::distance(c->getX(), c->getY(), x, y) * 10.F;
  }
  float orientation = MapUtil::getOrientationToTarget(c->getX(), c->getY(), x, y);
  float ap_cost = 0.F;
  int current_x = c->getX();
  int current_y = c->getY();
  int next_x = current_x;
  int next_y = current_y;
  int x_direction = 1;
  int y_direction = 1;
  if(orientation > 180.F) {
    y_direction = -1;
  }
  if(orientation > 90.F && orientation < 270.F) {
    x_direction = -1;
  }
  if(c->getY() == y) {
    if(current_x !=  x && x_direction == -1) {
      next_x = std::floor(next_x);
      ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, next_x, next_y);
      current_x = next_x;
    }
    while(current_x !=  x) {
      next_x = std::floor(current_x + x_direction);
      if(x_direction == -1) {
        ap_cost += getTile(current_y, next_x)->ap_cost * MapUtil::distance(current_x, current_y, next_x, next_y);
      }
      else {
        ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, next_x, next_y);
      }
      current_x = next_x;
    }
  }
  else if(c->getX() == x) {
    if(current_y !=  y && y_direction == -1) {
      next_y = std::floor(next_y);
      ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, next_x, next_y);
      current_y = next_y;
    }
    while(current_y !=  y) {
      next_y = std::floor(current_y + y_direction);
      if(y_direction == -1) {
        ap_cost += getTile(next_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, next_x, next_y);
      }
      else {
        ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, next_x, next_y);
      }
      current_y = next_y;
    }
  }
  else {
    float cos = std::cos(orientation * 3.141593F / 180.F);
    float sin = std::sin(orientation * 3.141593F / 180.F);
    float current_dx = current_x - std::floor(current_x);
    float current_dy = current_y - std::floor(current_y);
    float range_x = std::abs(-current_dx / cos);
    float range_y = std::abs(-current_dy / sin);
    float range;
    float next_dx;
    float next_dy;
    float x_x = (float) current_x + current_dx + cos * range_x;
    float x_y = (float) current_y + current_dy + sin * range_x;
    float y_x = (float) current_x + current_dx + cos * range_y;
    float y_y = (float) current_y + current_dy + sin * range_y;
    if(MapUtil::distanceSquare(x, y, x_x, x_y) < MapUtil::distanceSquare(x, y, x_x, x_y)) {
      next_x = x_x;
      next_dx = x_x - std::floor(current_x);
      next_y = x_y;
      next_dy = x_y - std::floor(current_y);
      range = range_x;
    }
    else {
      next_x = y_x;
      next_dx = y_x - std::floor(current_x);
      next_y = y_y;
      next_dy = y_y - std::floor(current_y);
      range = range_y;
    }
    ap_cost = getTile(current_y, current_x)->ap_cost * range;
    current_dx = next_dx;
    current_dy = next_dy;
    while(MapUtil::distanceSquare(x, y, current_x, current_y) > 2) {
    //while(current_x != x || current_y != y) {
      range_x = std::abs((x_direction - current_dx) / cos);
      range_y = std::abs((y_direction - current_dy) / sin);
      x_x = (float) current_x + current_dx + cos * range_x;
      x_y = (float) current_y + current_dy + sin * range_x;
      y_x = (float) current_x + current_dx + cos * range_y;
      y_y = (float) current_y + current_dy + sin * range_y;
      if(MapUtil::distanceSquare(x, y, x_x, x_y) < MapUtil::distanceSquare(x, y, x_x, x_y)) {
        next_x = std::floor(x_x);
        next_dx = x_x - (float) next_x;
        next_y = std::floor(x_y);
        next_dy = x_y - (float) next_y;
        range = range_x;
      }
      else {
        next_x = std::floor(y_x);
        next_dx = y_x - (float) next_x;
        next_y = std::floor(y_y);
        next_dy = y_y - (float) next_y;
        range = range_y;
      }
      if(x_direction == -1 || y_direction == -1) {
        ap_cost += getTile(next_y, next_x)->ap_cost * range;
      }
      else {
        ap_cost += getTile(current_y, current_x)->ap_cost * range;
      }
      current_x = next_x;
      current_dx = next_dx;
      current_y = next_y;
      current_dy = next_dy;
    }
  }
  if(x_direction == -1 || y_direction == -1) {
    ap_cost -= getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, x, y);
  }
  else {
    ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, x, y);
  }
  return ap_cost / c->getMovementTimeModifier();
}

bool Map::tryMove(Character * c, float destY, float destX) {
  if(c->isEtheral()) {
    return true;
  }
  for(Character * other : characters) {
    if(c != other && !other->isEtheral() && MapUtil::distance(destX, destY, other->getX(), other->getY()) <= c->getSize() + other->getSize()) {
      return false;
    } 
  }
  return true;
}

float Map::move(Character * c, float destY, float destX) {
  int x = destX;
  int y = destY;
  float orientation = MapUtil::getOrientationToTarget(c->getX(), c->getY(), x, y);
  if(orientation == 360.F) {
    return 0.F;
  }
  float range = MapUtil::distance(c->getX(), c->getY(), x, y);
  std::list<MapUtil::Pair> path = MapUtil::getPathFromOrientation(c->getX(), c->getY(), orientation, c->getSize(), range);
  for(MapUtil::Pair pair : path) {
    if(pair.x < 0 || std::floor(pair.x) >= sizeX || pair.y < 0 || std::floor(pair.y) >= sizeY || getTile(pair.y, pair.x)->solid || (!c->isFlying() && getTile(pair.y, pair.x)->untraversable)) {
      break;
    }
    x = pair.x;
    y = pair.y;
  }
  if(MapUtil::distance(c->getX(), c->getY(), x, y) > range) {
    x = destX;
    y = destY;
  }
  float ap_cost = getMoveCost(c, y, x);
  if(tryMove(c, y, x)) {
    c->move(y, x, orientation, id);
    return ap_cost;
  }
  else {
    return -1;
  }
}

float Map::move(Character * c, float orientation, float destY, float destX, float ap, World * world) {
  int lim_x = c->getX();
  int lim_y = c->getY();
  float ap_cost = 0.F;
  float current_cost = 0.F;
  float current_x = c->getX();
  float current_y = c->getY();
  float next_x = current_x;
  float next_y = current_y;
  int x_direction = 1;
  int y_direction = 1;
  if(orientation > 180.F) {
    y_direction = -1;
  }
  if(orientation > 90.F && orientation < 270.F) {
    x_direction = -1;
  }
  MapLink * link = nullptr;
  std::list<MapUtil::Pair> path = MapUtil::getPathFromOrientation(c->getX(), c->getY(), orientation, c->getSize(), std::max(3.F, ap / 10.F));
  for(MapUtil::Pair pair : path) {
    if(pair.x < 0 || std::floor(pair.x) >= sizeX || pair.y < 0 || std::floor(pair.y) >= sizeY || getTile(pair.y, pair.x)->solid || (!c->isFlying() && getTile(pair.y, pair.x)->untraversable)) {
      break;
    }
    link = world->getMapLink(pair.y, pair.x, id);
    if(link != nullptr) {
      break;
    }
    lim_x = pair.x;
    lim_y = pair.y;
  }
  if(c->isFlying()) {
    next_x = std::cos(orientation * 3.141593F / 180.F) * c->getMovementTimeModifier() * ap / 10.F + c->getX();
    next_y = std::sin(orientation * 3.141593F / 180.F) * c->getMovementTimeModifier() * ap / 10.F + c->getY();
    if(std::floor(next_x) == std::floor(lim_x) + x_direction || std::floor(next_y) == std::floor(lim_y) + y_direction) {
      next_x = lim_x;
      next_y = lim_y;
    }
    current_cost = getMoveCost(c, next_y, next_x);
  }
  else {
    if(orientation == 0.F || orientation == 180.F) {
      if(x_direction == -1) {
        next_x = std::floor(next_x);
        ap_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
        if(ap_cost <= ap) {
          current_x = next_x;
        }
      }
      while(ap_cost <= ap) {
        current_cost = ap_cost;
        current_x = next_x;
        next_x = std::floor(current_x + x_direction);
        if(next_x == std::floor(lim_x + x_direction)) {
          next_x = lim_x;
          ap_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
          break;
        }
        if(x_direction == -1) {
          ap_cost += getTile(current_y, next_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
        }
        else {
          ap_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
        }
      }
      if(ap_cost > ap) {
        next_x = current_x + (float) x_direction * (ap - current_cost) / getTile(current_y, current_x)->ap_cost;
        current_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
      }
      else {
        current_cost = ap_cost;
      }
    }
    else if(orientation == 90.F || orientation == 270.F) {
      if(y_direction == -1) {
        next_y = std::floor(next_y);
        ap_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
        if(ap_cost <= ap) {
          current_y = next_y;
        }
      }
      while(ap_cost <= ap) {
        current_cost = ap_cost;
        current_y = next_y;
        next_y = std::floor(current_y + y_direction);
        if(next_y == std::floor(lim_y + y_direction)) {
          next_y = lim_y;
          ap_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
          break;
        }
        if(y_direction == -1) {
          ap_cost += getTile(next_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
        }
        else {
          ap_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
        }
      }
      if(ap_cost > ap) {
        next_y = current_y + (float) y_direction * (ap - current_cost) / getTile(current_y, current_x)->ap_cost;
        current_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
      }
      else {
        current_cost = ap_cost;
      }
    }
    else {
      float cos = std::cos(orientation * 3.141593F / 180.F);
      float sin = std::sin(orientation * 3.141593F / 180.F);
      float range_x = std::abs(-(current_x - std::floor(current_x)) / cos);
      float range_y = std::abs(-(current_y - std::floor(current_y)) / sin);
      float range;
      float x_x = current_x + cos * range_x;
      float x_y = current_y + sin * range_x;
      float y_x = current_x + cos * range_y;
      float y_y = current_y + sin * range_y;
      if(MapUtil::distanceSquare(lim_x, lim_y, x_x, x_y) < MapUtil::distanceSquare(lim_x, lim_y, x_x, x_y)) {
        next_x = x_x;
        next_y = x_y;
        range = range_x;
      }
      else {
        next_x = y_x;
        next_y = y_y;
        range = range_y;
      }
      ap_cost = getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * range;
      while(ap_cost <= ap) {
        current_cost = ap_cost;
        current_x = next_x;
        current_y = next_y;
        range_x = std::abs((x_direction - (current_x - std::floor(current_x))) / cos);
        range_y = std::abs((y_direction - (current_y - std::floor(current_y))) / sin);
        x_x = current_x + cos * range_x;
        x_y = current_y + sin * range_x;
        y_x = current_x + cos * range_y;
        y_y = current_y + sin * range_y;
        if(MapUtil::distanceSquare(lim_x, lim_y, x_x, x_y) < MapUtil::distanceSquare(lim_x, lim_y, x_x, x_y)) {
          next_x = x_x;
          next_y = x_y;
          range = range_x;
        }
        else {
          next_x = y_x;
          next_y = y_y;
          range = range_y;
        }
        if(MapUtil::distanceSquare(lim_x, lim_y, current_x, current_y) <= 2) {
          next_x = lim_x;
          next_y = lim_y;
          ap_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
          break;
        }
        ap_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * range;
      }
      if(ap_cost > ap) {
        next_x = current_x + ((ap - current_cost) / getTile(current_y, current_x)->ap_cost) * cos;
        next_y = current_y + ((ap - current_cost) / getTile(current_y, current_x)->ap_cost) * sin;
        current_cost += getTile(current_y, current_x)->ap_cost / c->getMovementTimeModifier() * MapUtil::distance(current_x, current_y, next_x, next_y);
      }
      else {
        current_cost = ap_cost;
      }
    }
  }
  next_x = MapUtil::round(next_x);
  next_y = MapUtil::round(next_y);
  if(next_x < 0 || std::floor(next_x) >= sizeX || next_y < 0 || std::floor(next_y) >= sizeY || getTile(next_y, next_x)->solid || (!c->isFlying() && getTile(next_y, next_x)->untraversable)) {
    c->move(lim_y, lim_x, orientation, id);
    return -1;
  }
  if(link != nullptr && next_x == lim_x && next_y == lim_y) {
    Map * next_map;
    float dest_orientation = orientation;
    float diff = 0.F;
    float next_dx = MapUtil::round(next_x - std::floor(next_x));
    float next_dy = MapUtil::round(next_y - std::floor(next_y));
    next_x = std::floor(next_x);
    next_y = std::floor(next_y);
    float high_limit = MapUtil::round(0.999F - c->getSize());
    float low_limit = MapUtil::round(c->getSize());
    switch(link->type) {
      case THROUGH:
        if((next_dx == low_limit || next_dx == high_limit) && link->map1 == this && link->x1 == next_x + x_direction && link->y1 == next_y) {
          next_map = link->map2;
          removeCharacter(c);
          link->map2->addCharacter(c);
          next_x = link->x2 + x_direction;
          next_y = link->y2;
          next_dx = (next_dx == low_limit ? high_limit : low_limit);
        }
        else if((next_dx == low_limit || next_dx == high_limit) && link->map2 == this && link->x2 == next_x + x_direction && link->y2 == next_y) {
          next_map = link->map1;
          removeCharacter(c);
          link->map1->addCharacter(c);
          next_x = link->x1 + x_direction;
          next_y = link->y1;
          next_dx = (next_dx == low_limit ? high_limit : low_limit);
        }
        else if((next_dy == low_limit || next_dy == high_limit) && link->map1 == this && link->y1 == next_y + y_direction && link->x1 == next_x) {
          next_map = link->map2;
          removeCharacter(c);
          link->map2->addCharacter(c);
          next_x = link->x2;
          next_y = link->y2 + y_direction;
          next_dy = (next_dy == low_limit ? high_limit : low_limit);
        }
        else if((next_dy == low_limit || next_dy == high_limit) && link->map2 == this && link->y2 == next_y + y_direction && link->x2 == next_x) {
          next_map = link->map1;
          removeCharacter(c);
          link->map1->addCharacter(c);
          next_x = link->x1;
          next_y = link->y1 + y_direction;
          next_dy = (next_dy == low_limit ? high_limit : low_limit);
        }
        else if((next_dx == low_limit || next_dx == high_limit) && (next_dy == low_limit || next_dy == high_limit) && link->map1 == this && link->x1 == next_x + x_direction && link->y1 == next_y + y_direction) {
          next_map = link->map2;
          removeCharacter(c);
          link->map2->addCharacter(c);
          next_x = link->x2 + x_direction;
          next_y = link->y2 + y_direction;
          next_dx = (next_dx == low_limit ? high_limit : low_limit);
          next_dy = (next_dy == low_limit ? high_limit : low_limit);
        }
        else if((next_dx == low_limit || next_dx == high_limit) && (next_dy == low_limit || next_dy == high_limit) && link->map2 == this && link->x2 == next_x + x_direction && link->y2 == next_y + y_direction) {
          next_map = link->map1;
          removeCharacter(c);
          link->map1->addCharacter(c);
          next_x = link->x1 + x_direction;
          next_y = link->y1 + y_direction;
          next_dx = (next_dx == low_limit ? high_limit : low_limit);
          next_dy = (next_dy == low_limit ? high_limit : low_limit);
        }
        if(next_x < 0 || std::floor(next_x) >= next_map->sizeX || next_y < 0 || std::floor(next_y) >= next_map->sizeY || next_map->getTile(next_y, next_x)->solid || (!c->isFlying() && next_map->getTile(next_y, next_x)->untraversable)) {
          if(tryMove(c, lim_y, lim_x)) {
            if(MapUtil::distance(c->getX(), c->getY(), lim_x, lim_y) > MapUtil::distance(c->getX(), c->getY(), destX, destY)) {
              c->move(destY, destX, orientation, id);
              return -1.F;
            }
            c->move(lim_y, lim_x, orientation, id);
            return 0.F;
          }
          else {
            return -1.F;
          }
        }
        else {
          if(next_map->tryMove(c, next_y, next_x)) {
            c->move(next_y, next_x, dest_orientation, next_map->id);
          }
          else {
            return -1.F;
          }
        }
        break;
      case BOUNCE:
      case BACK:
        if((next_dx == low_limit || next_dx == high_limit) && link->map1 == this && link->x1 == next_x + x_direction && link->y1 == next_y) {
          next_map = link->map2;
          removeCharacter(c);
          link->map2->addCharacter(c);
          next_x = link->x2 - x_direction;
          next_y = link->y2;
          // BOUNCE orientation
          diff = (orientation > 270 || orientation < 90 ? 180 - orientation : orientation);
        }
        else if((next_dx == low_limit || next_dx == high_limit) && link->map2 == this && link->x2 == next_x + x_direction && link->y2 == next_y) {
          next_map = link->map1;
          removeCharacter(c);
          link->map1->addCharacter(c);
          next_x = link->x1 - x_direction;
          next_y = link->y1;
          // BOUNCE orientation
          diff = (orientation > 270 || orientation < 90 ? 180 - orientation : orientation);
        }
        else if((next_dy == low_limit || next_dy == high_limit) && link->map1 == this && link->y1 == next_y + y_direction && link->x1 == next_x) {
          next_map = link->map2;
          removeCharacter(c);
          link->map2->addCharacter(c);
          next_x = link->x2;
          next_y = link->y2 - y_direction;
          // BOUNCE orientation
          diff = (orientation > 180 ? 270 - orientation : 90 - orientation);
        }
        else if((next_dy == low_limit || next_dy == high_limit) && link->map2 == this && link->y2 == next_y + y_direction && link->x2 == next_x) {
          next_map = link->map1;
          removeCharacter(c);
          link->map1->addCharacter(c);
          next_x = link->x1;
          next_y = link->y1 - y_direction;
          // BOUNCE orientation
          diff = (orientation > 180 ? 270 - orientation : 90 - orientation);
        }
        else if((next_dx == low_limit || next_dx == high_limit) && (next_dy == low_limit || next_dy == high_limit) && link->map1 == this && link->x1 == next_x + x_direction && link->y1 == next_y + y_direction) {
          next_map = link->map2;
          removeCharacter(c);
          link->map2->addCharacter(c);
          next_x = link->x2 - x_direction;
          next_y = link->y2 - y_direction;
        }
        else if((next_dx == low_limit || next_dx == high_limit) && (next_dy == low_limit || next_dy == high_limit) && link->map2 == this && link->x2 == next_x + x_direction && link->y2 == next_y + y_direction) {
          next_map = link->map1;
          removeCharacter(c);
          link->map1->addCharacter(c);
          next_x = link->x1 - x_direction;
          next_y = link->y1 - y_direction;
        }
        if(link->type == BOUNCE) {
          dest_orientation = 180 + orientation + 2.F * diff;
        }
        else if(link->type == BACK) {
          dest_orientation = orientation + 180.F;
        }
        while(dest_orientation >= 360.F) {
          dest_orientation -= 360.F;
        }
        if(next_x < 0 || std::floor(next_x) >= next_map->sizeX || next_y < 0 || std::floor(next_y) >= next_map->sizeY || next_map->getTile(next_y, next_x)->solid || (!c->isFlying() && next_map->getTile(next_y, next_x)->untraversable)) {
          if(tryMove(c, lim_y, lim_x)) {
            if(MapUtil::distance(c->getX(), c->getY(), lim_x, lim_y) > MapUtil::distance(c->getX(), c->getY(), destX, destY)) {
              c->move(destY, destX, orientation, id);
              return -1.F;
            }
            c->move(lim_y, lim_x, orientation, id);
            return low_limit;
          }
          else {
            return -1.F;
          }
        }
        else {
          if(next_map->tryMove(c, next_y, next_x)) {
            c->move(next_y, next_x, dest_orientation, next_map->id);
          }
          else {
            return -1.F;
          }
        }
        break;
    }
    return ap - current_cost;
  }
  else {
    if(tryMove(c, next_y, next_x)) {
      if(MapUtil::distance(c->getX(), c->getY(), next_x, next_y) > MapUtil::distance(c->getX(), c->getY(), destX, destY)) {
        c->move(destY, destX, orientation, id);
        return -1.F;
      }
      if(next_x == c->getX() && next_y == c->getY()) {
        return -1.F;
      }
      c->move(next_y, next_x, orientation, id);
      return 0.F;
    }
    else {
      return -1.F;
    }
  }
}

float Map::actProjectile(Projectile * p, Adventure * adventure, float speed) {
  bool to_destroy = false;
  if(!p->isLost() && p->getTarget()->type == CHARACTER && adventure->getCharacter(p->getTarget()->id)->getCurrentMapId() == id) {
    p->setOrientation(MapUtil::getOrientationToTarget(p->getX(), p->getY(), p->getTarget()->x, p->getTarget()->y));
  }
  float x = MapUtil::round(std::cos(p->getOrientation() * 3.141593F / 180.F) * speed + p->getX());
  float y = MapUtil::round(std::sin(p->getOrientation() * 3.141593F / 180.F) * speed + p->getY());
  float max_x;
  float max_y;
  MapLink * link = nullptr;
  std::list<MapUtil::Pair> path = MapUtil::getPathFromOrientation(p->getX(), p->getY(), p->getOrientation(), p->size , speed);
  for(MapUtil::Pair pair : path) {
    if(pair.x < 0 || pair.x >= sizeX || pair.y < 0 || pair.y >= sizeY || getTile(pair.y, pair.x)->solid) {
      to_destroy = true;
      break;
    }
    link = adventure->getWorld()->getMapLink(pair.y, pair.x, id);
    if(link != nullptr) {
      break;
    }
    max_x = pair.x;
    max_y = pair.y;
  }
  // use these coordinates if the projectile is destroyed before meeting the target
  if(to_destroy || link != nullptr) {
    x = max_x;
    y = max_y;
  }
  std::list<Character *> targets = std::list<Character *>();
  if(p->getY() == y) {
    for(Character * c : characters) {
      if(c->getY() <=  y + c->getSize() + p->getSize() && c->getY() >= y - c->getSize() - p->getSize()) {
        if( x > p->getX()) {
          if(c->getX() <= x + c->getSize() + p->getSize() && c->getX()>= p->getX() - c->getSize() - p->getSize()) {
            targets.push_back(c);
          }
        }
        else {
          if(c->getX() <= p->getX() + c->getSize() + p->getSize() && c->getX() >= x - c->getSize() - p->getSize()) {
            targets.push_back(c);
          }
        }
      }
    }
  }
  else if(p->getX() == x) {
    for(Character * c : characters) {
      if(c->getX() <= x + c->getSize() + p->getSize() && c->getX() >= x - c->getSize() - p->getSize()) {
        if(y > p->getY()) {
          if(c->getY() <= y + c->getSize() + p->getSize() && c->getY() >= p->getY() - c->getSize() - p->getSize()) {
            targets.push_back(c);
          }
        }
        else {
          if(c->getY() <= p->getY() + c->getSize() + p->getSize() && c->getY() >= y - c->getSize() - p->getSize()) {
            targets.push_back(c);
          }
        }
      }
    }
  }
  else {
    float tan = std::tan(p->getOrientation() * 3.141593F / 180.F);
    for(Character * c : characters) {
      if( (c->getX() <= x + c->getSize() + p->getSize() && c->getX() >= p->getX() - c->getSize() - p->getSize()) ||
        (c->getX() <= p->getX() + c->getSize() + p->getSize() && c->getX() >= x - c->getSize() - p->getSize()) ) {
        float p_y = tan * c->getX() + p->getY();
        if( (c->getY() <= p_y + c->getSize() + p->getSize() && c->getY() >= p_y - c->getSize() - p->getSize()) ||
          (c->getY() <= p_y - c->getSize() - p->getSize() && c->getY() >= p_y + c->getSize() + p->getSize()) ) {
          targets.push_back(c);
        }
      }
    }
  }
  // attack targets
  auto rangeSort = [&](Character * a, Character * b) -> bool {
    return MapUtil::distance(p->getX(), p->getY(), a->getX(), a->getY()) <
      MapUtil::distance(p->getX(), p->getY(), b->getX(), b->getY());
  };
  targets.sort(rangeSort);
  for(Character * character : targets) {
    if(!p->isLost() && p->getArea() > 0 && p->getTarget() == nullptr &&
      MapUtil::distance(p->getX(), p->getY(), character->getX(), character->getY())
      >= MapUtil::distance(p->getX(), p->getY(), p->getDestX(), p->getDestY()) ) {
        // exploding on targeted zone when other targets where found
        p->move(p->getDestY(), p->getDestX(), p->getOrientation(), id);
        p->attack(nullptr, characters, adventure);
      }
    p->attack(character, characters, adventure);
    if(p->noDamage()) {
      break;
    }
  }
  if(targets.empty() && !p->isLost() && p->getArea() > 0 && p->getTarget() == nullptr &&
    MapUtil::distance(p->getX(), p->getY(), x, y)
    >= MapUtil::distance(p->getX(), p->getY(), p->getDestX(), p->getDestY()) ) {
      // exploding on targeted zone when no other target where found
      p->move(p->getDestY(), p->getDestX(), p->getOrientation(), id);
      p->attack(nullptr, characters, adventure);
  }
  std::list<Character *> tokill = std::list<Character *>();
  for(Character * c : characters) {
    if(c != nullptr && !c->isAlive()) {
      tokill.push_back(c);
    }
  }
  for(Character * c : tokill) {
    killCharacter(p->getOwner(), c);
  }
  tokill.clear();
  // destroy the projectile after dealing damage
  if(to_destroy) {
    p->markDestroyed();
    return 0.F;
  }
  p->clearOldTargets();
  if(link == nullptr) {
    p->reduceDamageTick();
  }
  else {
    float old_x = x;
    float old_y = y;
    float range = MapUtil::distance(x, y, p->getX(), p->getY());
    Map * next_map;
    float orientation = p->getOrientation();
    float dest_orientation = orientation;
    float diff = 0.F;
    int x_direction = 1;
    int y_direction = 1;
    if(orientation > 180.F) {
      y_direction = -1;
    }
    if(orientation > 90.F && orientation < 270.F) {
      x_direction = -1;
    }
    float dx = MapUtil::round(x - std::floor(x));
    float dy = MapUtil::round(y - std::floor(y));
    x = std::floor(x);
    y = std::floor(y);
    float high_limit = MapUtil::round(0.999F - p->getSize());
    float low_limit = MapUtil::round(p->getSize());
    switch(link->type) {
      case THROUGH:
        if((dx == low_limit || dx == high_limit) && link->map1 == this && link->x1 == x + x_direction && link->y1 == y) {
          next_map = link->map2;
          x = link->x2 + x_direction;
          y = link->y2;
          dx = (dx == low_limit ? high_limit : low_limit);
        }
        else if((dx == low_limit || dx == high_limit) && link->map2 == this && link->x2 == x + x_direction && link->y2 == y) {
          next_map = link->map1;
          x = link->x1 + x_direction;
          y = link->y1;
          dx = (dx == low_limit ? high_limit : low_limit);
        }
        else if((dy == low_limit || dy == high_limit) && link->map1 == this && link->y1 == y + y_direction && link->x1 == x) {
          next_map = link->map2;
          x = link->x2;
          y = link->y2 + y_direction;
          dy = (dy == low_limit ? high_limit : low_limit);
        }
        else if((dy == low_limit || dy == high_limit) && link->map2 == this && link->y2 == y + y_direction && link->x2 == x) {
          next_map = link->map1;
          x = link->x1;
          y = link->y1 + y_direction;
          dy = (dy == low_limit ? high_limit : low_limit);
        }
        else if((dx == low_limit || dx == high_limit) && (dy == low_limit || dy == high_limit) && link->map1 == this && link->x1 == x + x_direction && link->y1 == y + y_direction) {
          next_map = link->map2;
          x = link->x2 + x_direction;
          y = link->y2 + y_direction;
          dx = (dx == low_limit ? high_limit : low_limit);
          dy = (dy == low_limit ? high_limit : low_limit);
        }
        else if((dx == low_limit || dx == high_limit) && (dy == low_limit || dy == high_limit) && link->map2 == this && link->x2 == x + x_direction && link->y2 == y + y_direction) {
          next_map = link->map1;
          x = link->x1 + x_direction;
          y = link->y1 + y_direction;
          dx = (dx == low_limit ? high_limit : low_limit);
          dy = (dy == low_limit ? high_limit : low_limit);
        }
        if(x < 0 || std::floor(x) >= next_map->sizeX || y < 0 || std::floor(y) >= next_map->sizeY || next_map->getTile(y, x)->solid) {
          p->move(old_y, old_x, orientation, id);
          return 0.F;
        }
        else {
          p->move(y, x, dest_orientation, next_map->id);
        }
        break;
      case BOUNCE:
      case BACK:
        if((dx == low_limit || dx == high_limit) && link->map1 == this && link->x1 == x + x_direction && link->y1 == y) {
          next_map = link->map2;
          x = link->x2 - x_direction;
          y = link->y2;
          // BOUNCE orientation
          diff = (orientation > 270 || orientation < 90 ? 180 - orientation : orientation);
        }
        else if((dx == low_limit || dx == high_limit) && link->map2 == this && link->x2 == x + x_direction && link->y2 == y) {
          next_map = link->map1;
          x = link->x1 - x_direction;
          y = link->y1;
          // BOUNCE orientation
          diff = (orientation > 270 || orientation < 90 ? 180 - orientation : orientation);
        }
        else if((dy == low_limit || dy == high_limit) && link->map1 == this && link->y1 == y + y_direction && link->x1 == x) {
          next_map = link->map2;
          x = link->x2;
          y = link->y2 - y_direction;
          // BOUNCE orientation
          diff = (orientation > 180 ? 270 - orientation : 90 - orientation);
        }
        else if((dy == low_limit || dy == high_limit) && link->map2 == this && link->y2 == y + y_direction && link->x2 == x) {
          next_map = link->map1;
          x = link->x1;
          y = link->y1 - y_direction;
          // BOUNCE orientation
          diff = (orientation > 180 ? 270 - orientation : 90 - orientation);
        }
        else if((dx == low_limit || dx == high_limit) && (dy == low_limit || dy == high_limit) && link->map1 == this && link->x1 == x + x_direction && link->y1 == y + y_direction) {
          next_map = link->map2;
          x = link->x2 - x_direction;
          y = link->y2 - y_direction;
        }
        else if((dx == low_limit || dx == high_limit) && (dy == low_limit || dy == high_limit) && link->map2 == this && link->x2 == x + x_direction && link->y2 == y + y_direction) {
          next_map = link->map1;
          x = link->x1 - x_direction;
          y = link->y1 - y_direction;
        }
        if(link->type == BOUNCE) {
          dest_orientation = 180 + orientation + 2.F * diff;
        }
        else if(link->type == BACK) {
          dest_orientation = orientation + 180.F;
        }
        while(dest_orientation >= 360.F) {
          dest_orientation -= 360.F;
        }
        if(x < 0 || std::floor(x) >= next_map->sizeX || y < 0 || std::floor(y) >= next_map->sizeY || next_map->getTile(y, x)->solid) {
          p->move(old_y, old_x, orientation, id);
          return 0.F;
        }
        else {
          p->move(y + dy, x + dx, dest_orientation, next_map->id);
        }
        break;
    }
    return speed - range;
  }
  p->move(y, x, p->getOrientation(), id);
  return 0.F;
}

void Map::clearProjectiles() {
  projectiles.clear();
}

std::string Map::tile_to_string(int y, int x) {
  std::stringstream * ss = new std::stringstream();
  String::insert_long(ss, id);
  String::insert_int(ss, y);
  String::insert_int(ss, x);
  String::insert(ss, getTile(y, x)->name);
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Map::target_to_string(Target * target) {
  std::stringstream * ss = new std::stringstream();
  String::insert_int(ss, target->type);
  if(target->type != NO_TARGET && target->type != SELF) {
    String::insert_long(ss, target->id);
    if(target->type != CHARACTER) {
      String::insert_float(ss, target->x);
      String::insert_float(ss, target->y);
    }
  }
  if(target->next != nullptr) {
    String::insert(ss, target_to_string(target->next));
  }
  else if(target->type != NO_TARGET) {
    String::insert(ss, "END");
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

Target * Map::target_from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  Target * target = new Target();
  target->type = String::extract_int(ss);
  if(target->type != NO_TARGET && target->type != SELF) {
    target->id = String::extract_long(ss);
    if(target->type != CHARACTER) {
      target->x = String::extract_float(ss);
      target->y = String::extract_float(ss);
    }
  }
  if(target->type != NO_TARGET) {
    std::string next = String::extract(ss);
    if(next != "END") {
      target->next = target_from_string(next);
    }
    else {
      target->next = nullptr;
    }
  }
  delete ss;
  return target;
}
