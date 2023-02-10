#include <sstream>

#include "data/Adventure.h"
#include "data/Effect.h"
#include "data/Item.h"
#include "data/Way.h"
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
  float range = MapUtil::distance(watcher->getX(), watcher->getY(), watcher->getDX(), watcher->getDY(), target->getX(), target->getY(), target->getDX(), target->getDY());
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
      result[y][x] = (Tile *) database->getTile("TXT_MIST");
    }
  }
  for(int y = 0; y <= range; y++) {
    float a = (float) y / (float) range;
    std::list<MapUtil::Pair> path = MapUtil::getPathFromCartesianEquation(a, range);
    // 8 times the same action with inverted coordinates
    for(MapUtil::Pair pair : path) {
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
    for(MapUtil::Pair pair : path) {
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
    for(MapUtil::Pair pair : path) {
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
    for(MapUtil::Pair pair : path) {
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
    for(MapUtil::Pair pair : path) {
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
    for(MapUtil::Pair pair : path) {
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
    for(MapUtil::Pair pair : path) {
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
    for(MapUtil::Pair pair : path) {
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
  for(Item * i : victim->getRace()->getLoot()) {
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
    Effect * effect = new Effect("TXT_GAIN_XP", victim->getLevel(), "", EXPERIENCE, INSTANT, xp, 0, damages, damage_reductions);
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

float Map::getMoveCost(Character * c, int y, int x, float dy, float dx, float orientation) {
  if(c->isFlying()) {
    return MapUtil::distance(c->getX(), c->getY(), c->getDX(), c->getDY(), x, y, dx, dy) * 10.F;
  }
  float ap_cost = 0.F;
  int current_x = c->getX();
  int current_y = c->getY();
  float current_dx = c->getDX();
  float current_dy = c->getDY();
  int next_x = current_x;
  int next_y = current_y;
  float next_dx = current_dx;
  float next_dy = current_dy;
  int x_direction = 1;
  int y_direction = 1;
  if(orientation > 180.F) {
    y_direction = -1;
  }
  if(orientation > 90.F && orientation < 270.F) {
    x_direction = -1;
  }
  if( (float) c->getY() + c->getDY() == (float) y + dy) {
    while(current_x !=  x) {
      next_x = current_x + x_direction;
      next_dx = 0.F;
      ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, next_x, next_y, next_dx, next_dy);
      current_x = next_x;
      current_dx = next_dx;
    }
  }
  else if( (float) c->getX() + c->getDX() == (float) x + dx) {
    while(current_y !=  y) {
      next_y = current_y + y_direction;
      next_dy = 0.F;
      ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, next_x, next_y, next_dx, next_dy);
      current_y = next_y;
      current_dy = next_dy;
    }
  }
  else {
    float tan = std::tan(orientation * 3.141593F / 180.F);
    while(current_x != x && current_y != y) {
      next_x = current_x + x_direction;
      next_dx = 0.F;
      next_dy = (float) current_y + current_dy + (1.F - current_dy) * tan;
      next_y = floor(next_dy);
      next_dy -= (float) next_y;
      if(next_y != current_y) {
        next_y = current_y + y_direction;
        next_dy = 0.F;
        next_dx = (float) current_x + current_dx + (1.F - current_dx) / tan;
        next_x = floor(next_dx);
        next_dx -= (float) next_x;
      }
      ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, next_x, next_y, next_dx, next_dy);
      current_x = next_x;
      current_y = next_y;
      current_dx = next_dx;
      current_dy = next_dy;
    }
  }
  ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, x, y, dx, dy);
  return ap_cost;
}

float Map::move(Character * c, int destY, int destX, float destDY, float destDX) {
  int x = destX;
  int y = destY;
  float dx = destDX;
  float dy = destDY;
  float orientation = MapUtil::getOrientationToTarget(c->getX(), c->getY(), c->getDX(), c->getDY(), x, y, dx, dy);
  if(orientation == 360.F) {
    return 0.F;
  }
  float range = MapUtil::distance(c->getX(), c->getY(), c->getDX(), c->getDY(), x, y, dx, dy);
  std::list<MapUtil::Pair> path = MapUtil::getPathFromOrientation(c->getX(), c->getY(), c->getDX(), c->getDY(), orientation, range);
  for(MapUtil::Pair pair : path) {
    if(pair.x < 0 || pair.x >= sizeX || pair.y < 0 || pair.y >= sizeY || getTile(pair.y, pair.x)->solid || (!c->isFlying() && getTile(pair.y, pair.x)->untraversable)) {
      break;
    }
    x = pair.x;
    y = pair.y;
    dx = pair.dx;
    dy = pair.dy;
  }
  x = x > destX ? destX : x;
  dx = dx > destDX ? destDX : dx;
  y = y > destY ? destY : y;
  dy = dy > destDY ? destDY : dy;
  float ap_cost = getMoveCost(c, y, x, dy, dx, orientation);
  c->move(y, x, dy, dx, orientation);
  return ap_cost;
}

void Map::move(Character * c, float orientation) {
  int lim_x = c->getX();
  float lim_dx = c->getDX();
  int lim_y = c->getY();
  float lim_dy = c->getDY();
  float ap_cost = 0.F;
  float current_cost = 0.F;
  int current_x = c->getX();
  int current_y = c->getY();
  float current_dx = c->getDX();
  float current_dy = c->getDY();
  int next_x = current_x;
  int next_y = current_y;
  float next_dx = current_dx;
  float next_dy = current_dy;
  std::list<MapUtil::Pair> path = MapUtil::getPathFromOrientation(c->getX(), c->getY(), c->getDX(), c->getDY(), orientation, 1.F);
  for(MapUtil::Pair pair : path) {
    if(pair.x < 0 || pair.x >= sizeX || pair.y < 0 || pair.y >= sizeY || getTile(pair.y, pair.x)->solid || (!c->isFlying() && getTile(pair.y, pair.x)->untraversable)) {
      break;
    }
    lim_x = pair.x;
    lim_y = pair.y;
    lim_dx = pair.dx;
    lim_dy = pair.dy;
  }
  int x_direction = 1;
  int y_direction = 1;
  if(orientation > 180.F) {
    y_direction = -1;
  }
  if(orientation > 90.F && orientation < 270.F) {
    x_direction = -1;
  }
  if(c->isFlying()) {
    next_dx = std::cos(orientation * 3.141593F / 180.F) * 0.1F + (float) c->getX() + c->getDX();
    next_x = floor(next_dx);
    next_dx -= (float) next_x;
    next_dy = std::sin(orientation * 3.141593F / 180.F) * 0.1F + (float) c->getY() + c->getDY();
    next_y = floor(next_dy);
    next_dy -= (float) next_y;
    if(next_x == lim_x + x_direction || next_y == lim_y + y_direction) {
      next_x = lim_x;
      next_dx = lim_dx;
      next_y = lim_y;
      next_dy = lim_y;
    }
  }
  else {
    if(orientation == 0.F || orientation == 180.F) {
      while(ap_cost < 1.F) {
        current_cost = ap_cost;
        current_x = next_x;
        current_dx = next_dx;
        next_x = current_x + x_direction;
        next_dx = 0.F;
        if(next_x == lim_x + x_direction) {
          next_x = lim_x;
          next_dx = lim_dx;
          ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, next_x, next_y, next_dx, next_dy);
          break;
        }
        ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, next_x, next_y, next_dx, next_dy);
      }
      if(ap_cost > 1.F) {
        next_dx = (float) current_x + current_dx + (float) x_direction * (1.F - current_cost) / getTile(current_y, current_x)->ap_cost;
        next_x = floor(next_dx);
        next_dx -= (float) next_x;
      }
    }
    else if(orientation == 90.F || orientation == 270.F) {
      while(ap_cost < 1.F) {
        current_cost = ap_cost;
        current_y = next_y;
        current_dy = next_dy;
        next_y = current_y + y_direction;
        next_dy = 0.F;
        if(next_y == lim_y + y_direction) {
          next_y = lim_y;
          next_dy = lim_dy;
          ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, next_x, next_y, next_dx, next_dy);
          break;
        }
        ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, next_x, next_y, next_dx, next_dy);
      }
      if(ap_cost > 1.F) {
        next_dy = (float) current_y + current_dy + (float) y_direction * (1.F - current_cost) / getTile(current_y, current_x)->ap_cost;
        next_y = floor(next_dy);
        next_dy -= (float) next_y;
      }
    }
    else {
      float tan = std::tan(orientation * 3.141593F / 180.F);
      while(ap_cost < 1.F) {
        current_cost = ap_cost;
        current_x = next_x;
        current_dx = next_dx;
        current_y = next_y;
        current_dy = next_dy;
        next_x = current_x + x_direction;
        next_dx = 0.F;
        next_dy += (float) current_y + current_dy + (1.F - current_dx) * tan;
        next_y = floor(next_dy);
        next_dy -= (float) next_y;
        if(next_y != current_y) {
          next_y = current_y + y_direction;
          next_dy = 0.F;
          next_dx += (float) current_x + current_dx + (1.F - current_dy) / tan;
          next_x = floor(next_dx);
          next_dx -= (float) next_x;
        }
        if(next_x == lim_x + x_direction || next_y == lim_y + y_direction) {
          next_x = lim_x;
          next_dx = lim_dx;
          next_y = lim_y;
          next_dy = lim_dy;
          ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, next_x, next_y, next_dx, next_dy);
          break;
        }
        ap_cost += getTile(current_y, current_x)->ap_cost * MapUtil::distance(current_x, current_y, current_dx, current_dy, next_x, next_y, next_dx, next_dy);
      }
      if(ap_cost > 1.F) {
        float cos = std::cos(orientation * 3.141593F / 180.F);
        float sin = std::sin(orientation * 3.141593F / 180.F);
        next_dx = (float) current_x + current_dx + ((1.F - current_cost) / getTile(current_y, current_x)->ap_cost) * cos;
        next_x = floor(next_dx);
        next_dx -= (float) next_x;
        next_dy = (float) current_y + current_dy + ((1.F - current_cost) / getTile(current_y, current_x)->ap_cost) * sin;
        next_y = floor(next_dy);
        next_dy -= (float) next_y;
      }
    }
  }
  current_cost = getMoveCost(c, next_y, next_x, next_dy, next_dx, orientation);
  next_dx = MapUtil::round(next_dx);
  next_dy = MapUtil::round(next_dy);
  if(current_cost <= 0.33 || next_x < 0 || next_x >= sizeX || next_y < 0 || next_y >= sizeY || getTile(next_y, next_x)->solid || (!c->isFlying() && getTile(next_y, next_x)->untraversable)) {
    return;
  }
  c->move(next_y, next_x, next_dy, next_dx, orientation);
}

bool Map::actProjectile(Projectile * p, Adventure * adventure) {
  bool to_destroy = false;
  if(!p->isLost() && p->getTarget() !=nullptr) {
    p->setOrientation(MapUtil::getOrientationToTarget(p->getX(), p->getY(), p->getDX(), p->getDY(), p->getTarget()->getX(), p->getTarget()->getY(), p->getTarget()->getDX(), p->getTarget()->getDY()));
  }
  float dx = std::cos(p->getOrientation() * 3.141593F / 180.F) * p->getSpeed() + (float) p->getX() + p->getDX();
  int x = floor(dx);
  dx = MapUtil::round(dx - (float) x);
  float dy = std::sin(p->getOrientation() * 3.141593F / 180.F) * p->getSpeed() + (float) p->getY() + p->getDY();
  int y = floor(dy);
  dy = MapUtil::round(dy - (float) y);
  int max_x;
  float max_dx;
  int max_y;
  float max_dy;
  std::list<MapUtil::Pair> path = MapUtil::getPathFromOrientation(p->getX(), p->getY(), p->getDX(), p->getDY(), p->getOrientation(), p->getSpeed());
  for(MapUtil::Pair pair : path) {
    if(pair.x < 0 || pair.x >= sizeX || pair.y < 0 || pair.y >= sizeY || getTile(pair.y, pair.x)->solid) {
      to_destroy = true;
      break;
    }
    max_x = pair.x;
    max_y = pair.y;
    max_dx = pair.dx;
    max_dy = pair.dy;
  }
  // use these coordinates if the projectile is destroyed before meeting the target
  if(to_destroy) {
    x = max_x;
    dx = max_dx;
    y = max_y;
    dy = max_dy;
  }
  std::list<Character *> targets = std::list<Character *>();
  if( (float) p->getY() + p->getDY() == (float) y + dy) {
    for(Character * c : characters) {
      float c_y = (float) c->getY() + c->getDY();
      if(c_y <= (float) y + dy + c->getSize() + p->getSize() && c_y >= (float) y + dy - c->getSize() - p->getSize()) {
        float c_x = (float) c->getX() + c->getDX();
        if((float) x + dx > (float) p->getX() + p->getDX()) {
          if(c_x <= (float) x + dx + c->getSize() + p->getSize() && c_x >= (float) p->getX() + p->getDX() - c->getSize() - p->getSize()) {
            targets.push_back(c);
          }
        }
        else {
          if(c_x <= (float) p->getX() + p->getDX() + c->getSize() + p->getSize() && c_x >= (float) x + dx - c->getSize() - p->getSize()) {
            targets.push_back(c);
          }
        }
      }
    }
  }
  else if( (float) p->getX() + p->getDX() == (float) x + dx) {
    for(Character * c : characters) {
      float c_x = (float) c->getX() + c->getDX();
      if(c_x <= (float) x + dx + c->getSize() + p->getSize() && c_x >= (float) x + dx - c->getSize() - p->getSize()) {
        float c_y = (float) c->getY() + c->getDY();
        if((float) y + dy > (float) p->getY() + p->getDY()) {
          if(c_y <= (float) y + dy + c->getSize() + p->getSize() && c_y >= (float) p->getY() + p->getDY() - c->getSize() - p->getSize()) {
            targets.push_back(c);
          }
        }
        else {
          if(c_y <= (float) p->getY() + p->getDY() + c->getSize() + p->getSize() && c_y >= (float) y + dy - c->getSize() - p->getSize()) {
            targets.push_back(c);
          }
        }
      }
    }
  }
  else {
    float tan = std::tan(p->getOrientation() * 3.141593F / 180.F);
    for(Character * c : characters) {
      float c_x = (float) c->getX() + c->getDX();
      if( (c_x <= (float) x + dx + c->getSize() + p->getSize() && c_x >= (float) p->getX() + p->getDX() - c->getSize() - p->getSize()) ||
        (c_x <= (float) p->getX() + p->getDX() + c->getSize() + p->getSize() && c_x >= (float) x + dx - c->getSize() - p->getSize()) ) {
        float c_y = (float) c->getY() + c->getDY();
        float p_y = tan * (c_x - (float) p->getX() - p->getDX()) + (float) p->getY() + p->getDY();
        if( (c_y <= p_y + c->getSize() + p->getSize() && c_y >= p_y - c->getSize() - p->getSize()) ||
          (c_y <= p_y - c->getSize() - p->getSize() && c_y >= p_y + c->getSize() + p->getSize()) ) {
          targets.push_back(c);
        }
      }
    }
  }
  // attack targets
  auto rangeSort = [&](Character * a, Character * b) -> bool {
    return MapUtil::distance(p->getX(), p->getY(), p->getDX(), p->getDY(), a->getX(), a->getY(), a->getDX(), a->getDY()) <
      MapUtil::distance(p->getX(), p->getY(), p->getDX(), p->getDY(), b->getX(), b->getY(), b->getDX(), b->getDY());
  };
  targets.sort(rangeSort);
  for(Character * character : targets) {
    if(!p->isLost() && p->getArea() > 0 && p->getTarget() == nullptr &&
      MapUtil::distance(p->getX(), p->getY(), p->getDX(), p->getDY(), character->getX(), character->getY(), character->getDX(), character->getDY())
      >= MapUtil::distance(p->getX(), p->getY(), p->getDX(), p->getDY(), p->getDestX(), p->getDestY(), p->getDestDX(), p->getDestDY()) ) {
        // exploding on targeted zone when other targets where found
        p->move(p->getDestY(), p->getDestX(), p->getDestDY(), p->getDestDX());
        p->attack(nullptr, characters, adventure);
      }
    p->attack(character, characters, adventure);
    if(p->noDamage()) {
      break;
    }
  }
  if(targets.empty() && !p->isLost() && p->getArea() > 0 && p->getTarget() == nullptr &&
    MapUtil::distance(p->getX(), p->getY(), p->getDX(), p->getDY(), x, y, dx, dy)
    >= MapUtil::distance(p->getX(), p->getY(), p->getDX(), p->getDY(), p->getDestX(), p->getDestY(), p->getDestDX(), p->getDestDY()) ) {
      // exploding on targeted zone when no other target where found
      p->move(p->getDestY(), p->getDestX(), p->getDestDY(), p->getDestDX());
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
    return true;
  }
  p->clearOldTargets();
  p->move(y, x, dy, dx);
  p->reduceDamageTick();
  return p->noDamage();
}

void Map::actAllProjectiles(Adventure * adventure) {
  std::list<Projectile *> new_projectiles = std::list<Projectile *>();
  for(Projectile * projectile : projectiles) {
    if(projectile != nullptr) {
      if(actProjectile(projectile, adventure)) {
        delete projectile;
      }
      else {
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
      Tile * temp = new Tile(getTile(y, x)->name, getTile(y, x)->untraversable, getTile(y, x)->opaque, getTile(y, x)->solid, getTile(y, x)->ap_cost, getLight(y, x));
      String::insert(ss, temp->to_string());
      delete temp;
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
      display->tiles[j][i] = Tile::from_string(String::extract(ss));
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
