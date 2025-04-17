#include <sstream>

#include "data/Adventure.h"
#include "data/Effect.h"
#include "data/World.h"

#include "data/items/Item.h"
#include "data/items/BasicItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/SerialItem.h"
#include "data/items/Gear.h"

#include "data/ways/Way.h"
#include "data/ways/Race.h"

#include "communication/SpeechManager.h"

#include "data/Map.h"

#include "util/String.h"
#include "util/MathUtil.h"

std::list<Character *> Map::getCharacters() { return characters; }
std::list<Projectile *> Map::getProjectiles() { return projectiles; }
std::list<Furniture *> Map::getFurnitures() { return furnitures; }
std::list<Loot *> Map::getLoots() { return loots; }
bool Map::allowedCoords(int64_t x, int64_t y) { return y >= offsetY && y < sizeY + offsetY && x >= offsetX && x < sizeX + offsetX; }
bool Map::allowedCoords(float x, float y) {
  int64_t local_y = (int64_t) std::floor(y) - offsetY;
  int64_t local_x = (int64_t) std::floor(x) - offsetX;
  return local_y >= 0 && local_y < sizeY && local_x >= 0 && local_x < sizeX;
}

Block * Map::getBlock(const MathUtil::Vector3i coord) {
  std::map<const MathUtil::Vector3i, Block *>::iterator it = blocks.find(coord);
  if(it != blocks.end()) {
    return it->second;
  }
  else {
    return nullptr;
  }
}

int64_t Map::getLight(int64_t x, int64_t y) { return allowedCoords(x, y) ? std::max(lights[y - offsetY][x - offsetX], light) : -1; }
int64_t Map::getLight(float x, float y) { return allowedCoords(x, y) ? std::max(lights[(int64_t) std::floor(y) - offsetY][(int64_t) std::floor(x) - offsetX], light) : -1; }

Furniture * Map::getFurniture(int64_t x, int64_t y) {
  if(allowedCoords(x, y)) {
    for(Furniture * furniture : furnitures) {
      if(furniture->getCoord().x == x && furniture->getCoord().y == y) {
        return furniture;
      }
    }
  }
  return nullptr;
}
Furniture * Map::getFurniture(float x, float y) {
  if(allowedCoords(x, y)) {
    int64_t int_x = (int64_t) std::floor(x);
    int64_t int_y = (int64_t) std::floor(y);
    for(Furniture * furniture : furnitures) {
      if(furniture->getCoord().x == int_x && furniture->getCoord().y == int_y) {
        return furniture;
      }
    }
  }
  return nullptr;
}

/*
bool Map::isOpaque(int32_t x, int32_t y) {
  Block * block = getBlock(x, y);
  Furniture * furniture = getFurniture(x, y);
  return (block != nullptr && block->opaque) || (furniture != nullptr && furniture->getOpaque());
}

bool Map::isOpaque(float x, float y) {
  Block * block = getBlock(x, y);
  Furniture * furniture = getFurniture(x, y);
  return (block != nullptr && block->opaque) || (furniture != nullptr && furniture->getOpaque());
}

bool Map::isSolid(int32_t x, int32_t y) {
  Block * block = getBlock(x, y);
  Furniture * furniture = getFurniture(x, y);
  return (block != nullptr && block->solid) || (furniture != nullptr && furniture->getSolid());
}

bool Map::isSolid(float x, float y) {
  Block * block = getBlock(x, y);
  Furniture * furniture = getFurniture(x, y);
  return (block != nullptr && block->solid) || (furniture != nullptr && furniture->getSolid());
}

bool Map::isUnwalkable(int32_t x, int32_t y) {
  Block * block = getBlock(x, y);
  Furniture * furniture = getFurniture(x, y);
  return (block != nullptr && block->unwalkable) || (furniture != nullptr && furniture->getUnwalkable());
}

bool Map::isUnwalkable(float x, float y) {
  Block * block = getBlock(x, y);
  Furniture * furniture = getFurniture(x, y);
  return (block != nullptr && block->unwalkable) || (furniture != nullptr && furniture->getUnwalkable());
}
*/
void Map::applyDayLight(int32_t light) { this->light = light; }

/*
void Map::calculateLights() {
  // initialize
  int32_t lightX[sizeX*sizeY];
  int32_t lightY[sizeX*sizeY];
  int32_t cpt = 0;
  for(int32_t y = 0; y < sizeY; y++) {
    for(int32_t x = 0; x < sizeX; x++) {
      lights[y][x] = tiles[y][x]->light;
      if (lights[y][x] > 0) {
        lightX[cpt] = x;
        lightY[cpt++] = y;
      }
    }
  }
  for(Furniture * furniture : furnitures) {
    int32_t light = furniture->getLight();
    int32_t x = furniture->getX() - offsetX;
    int32_t y = furniture->getY() - offsetY;
    if(light > lights[y][x]) {
      lights[y][x] = light;
      lightX[cpt] = x;
      lightY[cpt++] = y;
    }
  }
  for(Character * character : characters) {
    int32_t light = character->getLight();
    int32_t x = character->getX() - offsetX;
    int32_t y = character->getY() - offsetY;
    if(light > lights[y][x]) {
      lights[y][x] = light;
      lightX[cpt] = x;
      lightY[cpt++] = y;
    }
  }
  for(Projectile * projectile : projectiles) {
    int32_t light = projectile->getLight();
    int32_t x = projectile->getX() - offsetX;
    int32_t y = projectile->getY() - offsetY;
    if(light > lights[y][x]) {
      lights[y][x] = light;
      lightX[cpt] = x;
      lightY[cpt++] = y;
    }
  }
  for(int32_t i = 0; i < cpt; i++) {
    propagateLight(lightX[i],lightY[i]);
  }
  //new_light = false;
}

void Map::propagateLight(int32_t x, int32_t y) {
  int32_t light = lights[y][x] - 1;
  if(x > 1 && y > 1 && light > lights[y - 1][x - 1]) {
    lights[y - 1][x - 1] = light;
    if(!tiles[y - 1][x - 1]->opaque) {
      propagateLight(y - 1, x - 1);
    }
  }
  if(x > 1 && light > lights[y][x - 1]) {
    lights[y][x - 1] = light;
    if(!tiles[y][x - 1]->opaque) {
      propagateLight(y, x - 1);
    }
  }
  if(x > 1 && y < sizeY - 1 && light > lights[y + 1][x - 1]) {
    lights[y + 1][x - 1] = light;
    if(!tiles[y + 1][x - 1]->opaque) {
      propagateLight(y + 1, x - 1);
    }
  }
  if(y > 1 && light > lights[y - 1][x]) {
    lights[y - 1][x] = light;
    if(!tiles[y - 1][x]->opaque) {
      propagateLight(y - 1, x);
    }
  }
  if(y < sizeY - 1 && light > lights[y + 1][x]) {
    lights[y + 1][x] = light;
    if(!tiles[y + 1][x]->opaque) {
      propagateLight(y + 1, x);
    }
  }
  if(x < sizeX - 1 && y > 1 && light > lights[y - 1][x + 1]) {
    lights[y - 1][x + 1] = light;
    if(!tiles[y - 1][x + 1]->opaque) {
      propagateLight(y - 1, x + 1);
    }
  }
  if(x < sizeX - 1 && light > lights[y][x + 1]) {
    lights[y][x + 1] = light;
    if(!tiles[y][x + 1]->opaque) {
      propagateLight(y, x + 1);
    }
  }
  if(x < sizeX - 1 && y < sizeY - 1 && light > lights[y + 1][x + 1]) {
    lights[y + 1][x + 1] = light;
    if(!tiles[y + 1][x + 1]->opaque) {
      propagateLight(y + 1, x + 1);
    }
  }
}

bool Map::canSee(Character * watcher, Character * target) {
  if(target->isInvisible() || target->isEtheral()) {
    return false;
  }
  float range = MathUtil::distance(watcher->getX(), watcher->getY(), target->getX(), target->getY());
  if(range <= watcher->getDetectionRange()) {
    return true;
  }
  if(getBlock(target->getX(), target->getY())->name == "TXT_MIST" || range > watcher->getVisionRange() || (watcher->getVisionPower() - target->cloakPower()) < 0 - getLight(target->getX(), target->getY())) {
    return false;
  }
  return true;
}

int32_t Map::canSee(Character * watcher, Block * mist) {
  int32_t mist_nb = 0;
  std::vector<std::vector<Block *>> old_blocks = tiles;
  int32_t range = std::max(watcher->getVisionRange(), watcher->getDetectionRange());
  for(int32_t y = 0; y <= range; y++) {
    float a = (float) y / (float) range;
    std::list<MathUtil::Pair> path = MathUtil::getPathFromCartesianEquation(a, range);
    bool unseen = false;
    // 8 times the same action with inverted coordinates
    for(MathUtil::Pair pair : path) {
      int32_t i = std::floor(pair.x) + std::floor(watcher->getX()) - offsetX;
      int32_t j = std::floor(pair.y) + std::floor(watcher->getY()) - offsetY;
      if(i < 0 || i > sizeX || j < 0 || j > sizeY || tiles[j][i]->name == "TXT_VOID") {
        break;
      }
      if(unseen) {
        tiles[j][i] = mist;
        mist_nb++;
      }
      if(isOpaque(i + offsetX, j + offsetY)) {
        unseen = true;
      }
      if(lights[j][i] < 0 - watcher->getVisionPower()) {
        tiles[j][i] = mist;
        mist_nb++;
      }
    }
    unseen = false;
    for(MathUtil::Pair pair : path) {
      int32_t i = std::floor(pair.x) + std::floor(watcher->getX()) - offsetX;
      int32_t j = -std::floor(pair.y) + std::floor(watcher->getY()) - offsetY;
      if(i < 0 || i > sizeX || j < 0 || j > sizeY || tiles[j][i]->name == "TXT_VOID") {
        break;
      }
      if(unseen) {
        tiles[j][i] = mist;
        mist_nb++;
      }
      if(isOpaque(i + offsetX, j + offsetY)) {
        unseen = true;
      }
      if(lights[j][i] < 0 - watcher->getVisionPower()) {
        tiles[j][i] = mist;
        mist_nb++;
      }
    }
    unseen = false;
    for(MathUtil::Pair pair : path) {
      int32_t i = -std::floor(pair.x) + std::floor(watcher->getX()) - offsetX;
      int32_t j = std::floor(pair.y) + std::floor(watcher->getY()) - offsetY;
      if(i < 0 || i > sizeX || j < 0 || j > sizeY || tiles[j][i]->name == "TXT_VOID") {
        break;
      }
      if(unseen) {
        tiles[j][i] = mist;
        mist_nb++;
      }
      if(isOpaque(i + offsetX, j + offsetY)) {
        unseen = true;
      }
      if(lights[j][i] < 0 - watcher->getVisionPower()) {
        tiles[j][i] = mist;
        mist_nb++;
      }
    }
    unseen = false;
    for(MathUtil::Pair pair : path) {
      int32_t i = -std::floor(pair.x) + std::floor(watcher->getX()) - offsetX;
      int32_t j = -std::floor(pair.y) + std::floor(watcher->getY()) - offsetY;
      if(i < 0 || i > sizeX || j < 0 || j > sizeY || tiles[j][i]->name == "TXT_VOID") {
        break;
      }
      if(unseen) {
        tiles[j][i] = mist;
        mist_nb++;
      }
      if(isOpaque(i + offsetX, j + offsetY)) {
        unseen = true;
      }
      if(lights[j][i] < 0 - watcher->getVisionPower()) {
        tiles[j][i] = mist;
        mist_nb++;
      }
    }
    unseen = false;
    for(MathUtil::Pair pair : path) {
      int32_t i = std::floor(pair.y) + std::floor(watcher->getX()) - offsetX;
      int32_t j = std::floor(pair.x) + std::floor(watcher->getY()) - offsetY;
      if(i < 0 || i > sizeX || j < 0 || j > sizeY || tiles[j][i]->name == "TXT_VOID") {
        break;
      }
      if(unseen) {
        tiles[j][i] = mist;
        mist_nb++;
      }
      if(isOpaque(i + offsetX, j + offsetY)) {
        unseen = true;
      }
      if(lights[j][i] < 0 - watcher->getVisionPower()) {
        tiles[j][i] = mist;
        mist_nb++;
      }
    }
    unseen = false;
    for(MathUtil::Pair pair : path) {
      int32_t i = -std::floor(pair.y) + std::floor(watcher->getX()) - offsetX;
      int32_t j = std::floor(pair.x) + std::floor(watcher->getY()) - offsetY;
      if(i < 0 || i > sizeX || j < 0 || j > sizeY || tiles[j][i]->name == "TXT_VOID") {
        break;
      }
      if(unseen) {
        tiles[j][i] = mist;
        mist_nb++;
      }
      if(isOpaque(i + offsetX, j + offsetY)) {
        unseen = true;
      }
      if(lights[j][i] < 0 - watcher->getVisionPower()) {
        tiles[j][i] = mist;
        mist_nb++;
      }
    }
    unseen = false;
    for(MathUtil::Pair pair : path) {
      int32_t i = std::floor(pair.y) + std::floor(watcher->getX()) - offsetX;
      int32_t j = -std::floor(pair.x) + std::floor(watcher->getY()) - offsetY;
      if(i < 0 || i > sizeX || j < 0 || j > sizeY || tiles[j][i]->name == "TXT_VOID") {
        break;
      }
      if(unseen) {
        tiles[j][i] = mist;
        mist_nb++;
      }
      if(isOpaque(i + offsetX, j + offsetY)) {
        unseen = true;
      }
      if(lights[j][i] < 0 - watcher->getVisionPower()) {
        tiles[j][i] = mist;
        mist_nb++;
      }
    }
    unseen = false;
    for(MathUtil::Pair pair : path) {
      int32_t i = -std::floor(pair.y) + std::floor(watcher->getX()) - offsetX;
      int32_t j = -std::floor(pair.x) + std::floor(watcher->getY()) - offsetY;
      if(i < 0 || i > sizeX || j < 0 || j > sizeY || tiles[j][i]->name == "TXT_VOID") {
        break;
      }
      if(unseen) {
        tiles[j][i] = mist;
        mist_nb++;
      }
      if(isOpaque(i + offsetX, j + offsetY)) {
        unseen = true;
      }
      if(lights[j][i] < 0 - watcher->getVisionPower()) {
        tiles[j][i] = mist;
        mist_nb++;
      }
    }
  }
  // fix artifacts
  for(int32_t y = watcher->getY() - offsetY + 1; y < sizeY; y++) {
    for(int32_t x = watcher->getX() - offsetX + 1; x < sizeX; x++) {
      if(tiles[y][x]->name == "TXT_MIST" && old_blocks[y][x]->opaque && !(tiles[y-1][x]->opaque && tiles[y][x-1]->opaque)) {
        tiles[y][x] = old_blocks[y][x];
        mist_nb--;
      }
    }
  }
  for(int32_t y = watcher->getY() - offsetY + 1; y < sizeY; y++) {
    for(int32_t x = watcher->getX() - offsetX - 1; x >= 0; x--) {
      if(tiles[y][x]->name == "TXT_MIST" && old_blocks[y][x]->opaque && !(tiles[y-1][x]->opaque && tiles[y][x+1]->opaque)) {
        tiles[y][x] = old_blocks[y][x];
        mist_nb--;
      }
    }
  }
  for(int32_t y = watcher->getY() - offsetY - 1; y >= 0; y--) {
    for(int32_t x = watcher->getX() - offsetX + 1; x < sizeX; x++) {
      if(tiles[y][x]->name == "TXT_MIST" && old_blocks[y][x]->opaque && !(tiles[y+1][x]->opaque && tiles[y][x-1]->opaque)) {
        tiles[y][x] = old_blocks[y][x];
        mist_nb--;
      }
    }
  }
  for(int32_t y = watcher->getY() - offsetY - 1; y >= 0; y--) {
    for(int32_t x = watcher->getX() - offsetX - 1; x >= 0; x--) {
      if(tiles[y][x]->name == "TXT_MIST" && old_blocks[y][x]->opaque && !(tiles[y+1][x]->opaque && tiles[y][x+1]->opaque)) {
        tiles[y][x] = old_blocks[y][x];
        mist_nb--;
      }
    }
  }
  return mist_nb;
}

int32_t Map::getMistNb() { return mist_nb; }
*/

int32_t Map::getRotation() { return rotation; }

void Map::setBlock(const MathUtil::Vector3i coord, Block * block) {
  std::map<const MathUtil::Vector3i, Block *>::iterator it = blocks.find(coord);
  if(it != blocks.end()) {
    blocks.erase(it);
  }
  if(block != nullptr) {
    blocks.insert(std::make_pair(coord, block));
  }
}

void Map::addCharacter(Character * c) { characters.push_back(c); }
void Map::addProjectile(Projectile * p) { projectiles.push_back(p); }

void Map::addFurniture(Furniture * f) {
  for(Furniture * furniture : furnitures) {
    if(f->getCoord() == furniture->getCoord()) {
      return;
    }
  }
  furnitures.push_back(f);
}

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
  loot->x = victim->getCoord().x;
  loot->y = victim->getCoord().y;
  for(ItemSlot * slot : victim->getGear()->getItems()) {
    if(slot->item->droppable) {
      loot->items.push_back(slot->item);
    }
  }
  for(Item * i : victim->getLoot()) {
    if(i->droppable) {
      loot->items.push_back(i);
    }
  }
  loot->type = LOOT_CORPSE;
  if(killer != victim) {
    killer->gainXP(victim->getXP() / 2);
  }
  else {
    std::array<float, DAMAGE_TYPE_NUMBER> damages;
    int32_t xp = victim->getXP();
    Effect * effect = new Effect("TXT_GAIN_XP", ++effect::id_cpt, victim->getLevel(), "", EFFECT_EXPERIENCE, DURATION_INSTANT, xp, 0, damages);
    std::list<Effect *> * effects = new std::list<Effect *>();
    effects->push_back(effect);
    loot->items.push_back(new BasicItem("TXT_PERL_OF_WISDOM", ++item::id_cpt, ITEM_BASIC, ITEM_POTION, xp, xp, 1.F, 1, 1, xp * 10, true, true, true, 5, *effects));
    delete effects;
  }
  if(loot->items.empty()) {
    delete loot;
  }
  else {
    loots.push_back(loot);
  }
  victim->markDead(true);
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

void Map::takeLoot(Character * c, int32_t mode) {
  return;
  /*
  std::list<Loot *> to_delete = std::list<Loot *>();
  switch(mode) {
    case GRAB_FOOD: {
      for(Loot * l : loots) {
        if(l != nullptr && l->x == c->getX() && l->y == c->getY()) {
          for(Item * i : l->items) {
            if(i->consumable && ( (SerialItem *) i)->isFood()) {
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
    case GRAB_ALL: // 0
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
  */
}

/*

float Map::getMoveCost(Character * c, float ori_x, float ori_y, float x, float y) {
  if(!allowedCoords(x, y) || !allowedCoords(ori_x, ori_y)) {
    return 0.F;
  }
  if(ori_x == x && ori_y == y) {
    return 0.F;
  }
  if(c->isFlying()) {
    return MathUtil::distance(ori_x, ori_y, x, y) * 10.F / c->getMovementTimeModifier();
  }
  float orientation_z = MathUtil::getOrientationToTarget(ori_x, ori_y, x, y);
  float ap_cost = 0.F;
  float current_x = ori_x;
  float current_y = ori_y;
  float next_x = current_x;
  float next_y = current_y;
  int32_t x_direction = 1;
  int32_t y_direction = 1;
  if(orientation_z > 180.F) {
    y_direction = -1;
  }
  if(orientation_z > 90.F && orientation_z < 270.F) {
    x_direction = -1;
  }
  if(ori_y == y) {
    if(current_x != x && x_direction == -1) {
      next_x = std::floor(next_x);
      Block * block = getBlock(current_x, current_y);
      if(block != nullptr) {
        ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
      }
      current_x = next_x;
    }
    while(std::floor(current_x) != std::floor(x)) {
      next_x = std::floor(current_x + x_direction);
      if(x_direction == -1) {
        Block * block = getBlock(next_x, next_y);
        if(block != nullptr) {
          ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
      }
      else {
        Block * block = getBlock(current_x, current_y);
        if(block != nullptr) {
          ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
      }
      current_x = next_x;
    }
    if(x_direction == -1) {
      Block * block = getBlock(current_x, current_y);
      if(block != nullptr) {
        ap_cost -= block->speed * MathUtil::distance(current_x, current_y, x, y);
      }
    }
    else {
      Block * block = getBlock(current_x, current_y);
      if(block != nullptr) {
        ap_cost += block->speed * MathUtil::distance(current_x, current_y, x, y);
      }
    }
  }
  else if(ori_x == x) {
    if(current_y != y && y_direction == -1) {
      next_y = std::floor(next_y);
      Block * block = getBlock(current_x, current_y);
      if(block != nullptr) {
        ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
      }
      current_y = next_y;
    }
    while(std::floor(current_y) != std::floor(y)) {
      next_y = std::floor(current_y + y_direction);
      if(y_direction == -1) {
        Block * block = getBlock(next_x, current_y);
        if(block != nullptr) {
          ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
      }
      else {
        Block * block = getBlock(current_x, current_y);
        if(block != nullptr) {
          ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
      }
      current_y = next_y;
    }
    if(y_direction == -1) {
      Block * block = getBlock(current_x, current_y);
      if(block != nullptr) {
        ap_cost -= block->speed * MathUtil::distance(current_x, current_y, x, y);
      }
    }
    else {
      Block * block = getBlock(current_x, current_y);
      if(block != nullptr) {
        ap_cost += block->speed * MathUtil::distance(current_x, current_y, x, y);
      }
    }
  }
  else {
    float a = (y - ori_y) / (x - ori_x);
    float b = y - a * x;
    if(abs(a) < 1) {
      if(x_direction == -1) {
        next_x = std::floor(current_x);
        next_y = a * next_x + b;
        Block * block = getBlock(next_x, next_y);
        if(block != nullptr) {
          ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
        current_x = next_x;
        current_y = next_y;
      }
      while(std::floor(current_x) != std::floor(x)) {
        next_x = std::floor(current_x) + x_direction;
        next_y = a * next_x + b;
        if(x_direction == -1) {
          Block * block = getBlock(next_x, next_y);
          if(block != nullptr) {
            ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
          }
        }
        else {
          Block * block = getBlock(current_x, current_y);
          if(block != nullptr) {
            ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
          }
        }
        current_x = next_x;
        current_y = next_y;
      }
      if(x_direction == -1) {
        Block * block = getBlock(current_x, current_y);
        if(block != nullptr) {
          ap_cost -= block->speed * MathUtil::distance(current_x, current_y, x, y);
        }
      }
      else {
        Block * block = getBlock(current_x, current_y);
        if(block != nullptr) {
          ap_cost += block->speed * MathUtil::distance(current_x, current_y, x, y);
        }
      }
    }
    else {
      if(y_direction == -1) {
        next_y = std::floor(current_y);
        next_x = (next_y - b) / a;
        Block * block = getBlock(next_x, next_y);
        if(block != nullptr) {
          ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
        current_x = next_x;
        current_y = next_y;
      }
      while(std::floor(current_y) != std::floor(y)) {
        next_y = std::floor(current_y) + y_direction;
        next_x = (next_y - b) / a;
        if(y_direction == -1) {
          Block * block = getBlock(next_x, next_y);
          if(block != nullptr) {
            ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
          }
        }
        else {
          Block * block = getBlock(current_x, current_y);
          if(block != nullptr) {
            ap_cost += block->speed * MathUtil::distance(current_x, current_y, next_x, next_y);
          }
        }
        current_x = next_x;
        current_y = next_y;
      }
      if(y_direction == -1) {
        Block * block = getBlock(current_x, current_y);
        if(block != nullptr) {
          ap_cost -= block->speed * MathUtil::distance(current_x, current_y, x, y);
        }
      }
      else {
        Block * block = getBlock(current_x, current_y);
        if(block != nullptr) {
          ap_cost += block->speed * MathUtil::distance(current_x, current_y, x, y);
        }
      }
    }
  }
  return ap_cost / c->getMovementTimeModifier();
}

bool Map::tryMove(Character * c, float destX, float destY) {
  if(c->isEtheral()) {
    return true;
  }
  if(getBlock(destX, destY)->solid || (getBlock(destX, destY)->unwalkable && !c->isFlying())) {
    return false;
  }
  for(Character * other : characters) {
    if(!other->isMarkedDead() && c != other && !other->isEtheral() && MathUtil::distance(destX, destY, other->getX(), other->getY()) <= c->getSize() + other->getSize()) {
      return false;
    }
  }
  return true;
}

float Map::move(Character * c, float destX, float destY, World * world) {
  int32_t x = destX;
  int32_t y = destY;
  float orientation_z = MathUtil::getOrientationToTarget(c->getX(), c->getY(), x, y);
  if(orientation_z == 360.F) {
    return 0.F;
  }
  float range = MathUtil::distance(c->getX(), c->getY(), x, y);
  std::list<MathUtil::Pair> path = MathUtil::getPathFromOrientation(c->getX(), c->getY(), orientation_z, c->getSize(), range);
  for(MathUtil::Pair pair : path) {
    if(pair.x < offsetX || std::floor(pair.x) >= sizeX + offsetX || pair.y < offsetY || std::floor(pair.y) >= sizeY + offsetY || getBlock(pair.x, pair.y)->solid || (!c->isFlying() && getBlock(pair.y, pair.x)->unwalkable)) {
      break;
    }
    x = pair.x;
    y = pair.y;
  }
  if(MathUtil::distance(c->getX(), c->getY(), x, y) > range) {
    x = destX;
    y = destY;
  }
  float ap_cost = getMoveCost(c, c->getX(), c->getY(), y, x);
  if(tryMove(c, y, x)) {
    c->move(x, y, offsetZ, orientation_z, world);
    return ap_cost;
  }
  else {
    return -1;
  }
}

float Map::move(Character * c, float orientation_z, float destX, float destY, float ap, World * world) {
  int32_t lim_x = c->getX();
  int32_t lim_y = c->getY();
  float ap_cost = 0.F;
  float current_cost = 0.F;
  float current_x = c->getX();
  float current_y = c->getY();
  float next_x = current_x;
  float next_y = current_y;
  int32_t x_direction = 1;
  int32_t y_direction = 1;
  if(orientation_z > 180.F) {
    y_direction = -1;
  }
  if(orientation_z > 90.F && orientation_z < 270.F) {
    x_direction = -1;
  }
  MapLink * link = nullptr;
  std::list<MathUtil::Pair> path = MathUtil::getPathFromOrientation(c->getX(), c->getY(), orientation_z, c->getSize(), std::max(3.F, ap / 10.F));
  for(MathUtil::Pair pair : path) {
    if(pair.x < offsetX || std::floor(pair.x) >= sizeX + offsetX || pair.y < offsetY || std::floor(pair.y) >= sizeY + offsetY || getBlock(pair.x, pair.y)->solid || (!c->isFlying() && getBlock(pair.x, pair.y)->unwalkable)) {
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
    next_x = std::cos(orientation_z * M_PI / 180.F) * c->getMovementTimeModifier() * ap / 10.F + c->getX();
    next_y = std::sin(orientation_z * M_PI / 180.F) * c->getMovementTimeModifier() * ap / 10.F + c->getY();
    if(std::floor(next_x) == std::floor(lim_x) + x_direction || std::floor(next_y) == std::floor(lim_y) + y_direction) {
      next_x = lim_x;
      next_y = lim_y;
    }
    current_cost = getMoveCost(c, c->getX(), c->getY(), next_y, next_x);
  }
  else {
    if(orientation_z == 0.F || orientation_z == 180.F) {
      if(x_direction == -1) {
        next_x = std::floor(next_x);
        ap_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
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
          ap_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
          break;
        }
        if(x_direction == -1) {
          ap_cost += getBlock(current_x, next_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
        else {
          ap_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
      }
      if(ap_cost > ap) {
        next_x = current_x + (float) x_direction * (ap - current_cost) / getBlock(current_x, current_y)->ap_cost;
        current_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
      }
      else {
        current_cost = ap_cost;
      }
    }
    else if(orientation_z == 90.F || orientation_z == 270.F) {
      if(y_direction == -1) {
        next_y = std::floor(next_y);
        ap_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
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
          ap_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
          break;
        }
        if(y_direction == -1) {
          ap_cost += getBlock(next_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
        else {
          ap_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
        }
      }
      if(ap_cost > ap) {
        next_y = current_y + (float) y_direction * (ap - current_cost) / getBlock(current_x, current_y)->ap_cost;
        current_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
      }
      else {
        current_cost = ap_cost;
      }
    }
    else {
      float cos = std::cos(orientation_z * M_PI / 180.F);
      float sin = std::sin(orientation_z * M_PI / 180.F);
      float range_x = std::abs(-(current_x - std::floor(current_x)) / cos);
      float range_y = std::abs(-(current_y - std::floor(current_y)) / sin);
      float range;
      float x_x = current_x + cos * range_x;
      float x_y = current_y + sin * range_x;
      float y_x = current_x + cos * range_y;
      float y_y = current_y + sin * range_y;
      if(MathUtil::distanceSquare(lim_x, lim_y, x_x, x_y) < MathUtil::distanceSquare(lim_x, lim_y, x_x, x_y)) {
        next_x = x_x;
        next_y = x_y;
        range = range_x;
      }
      else {
        next_x = y_x;
        next_y = y_y;
        range = range_y;
      }
      ap_cost = getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * range;
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
        if(MathUtil::distanceSquare(lim_x, lim_y, x_x, x_y) < MathUtil::distanceSquare(lim_x, lim_y, x_x, x_y)) {
          next_x = x_x;
          next_y = x_y;
          range = range_x;
        }
        else {
          next_x = y_x;
          next_y = y_y;
          range = range_y;
        }
        if(MathUtil::distanceSquare(lim_x, lim_y, current_x, current_y) <= 2) {
          next_x = lim_x;
          next_y = lim_y;
          ap_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
          break;
        }
        ap_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * range;
      }
      if(ap_cost > ap) {
        next_x = current_x + ((ap - current_cost) / getBlock(current_x, current_y)->ap_cost) * cos;
        next_y = current_y + ((ap - current_cost) / getBlock(current_x, current_y)->ap_cost) * sin;
        current_cost += getBlock(current_x, current_y)->ap_cost / c->getMovementTimeModifier() * MathUtil::distance(current_x, current_y, next_x, next_y);
      }
      else {
        current_cost = ap_cost;
      }
    }
  }
  next_x = MathUtil::round(next_x);
  next_y = MathUtil::round(next_y);
  if(next_x < offsetX || std::floor(next_x) >= sizeX + offsetX || next_y < offsetY || std::floor(next_y) >= sizeY + offsetY || getBlock(next_x, next_y)->solid || (!c->isFlying() && getBlock(next_x, next_y)->unwalkable)) {
    c->move(lim_x, lim_y, offsetZ, orientation_z, world);
    return -1;
  }
  if(link != nullptr && next_x == lim_x && next_y == lim_y) {
    Map * next_map;
    float dest_orientation = orientation_z;
    float diff = 0.F;
    float next_dx = MathUtil::round(next_x - std::floor(next_x));
    float next_dy = MathUtil::round(next_y - std::floor(next_y));
    next_x = std::floor(next_x);
    next_y = std::floor(next_y);
    float high_limit = MathUtil::round(0.999F - c->getSize());
    float low_limit = MathUtil::round(c->getSize());
    switch(link->type) {
      case MAPLINK_THROUGH:
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
        if(next_x < next_map->offsetX || std::floor(next_x) >= next_map->sizeX + next_map->offsetX || next_y < next_map->offsetY || std::floor(next_y) >= next_map->sizeY + next_map->offsetY || next_map->getBlock(next_x, next_y)->solid || (!c->isFlying() && next_map->getBlock(next_x, next_y)->unwalkable)) {
          if(tryMove(c, lim_y, lim_x)) {
            if(MathUtil::distance(c->getX(), c->getY(), lim_x, lim_y) > MathUtil::distance(c->getX(), c->getY(), destX, destY)) {
              c->move(destX, destY, next_map->offsetZ, orientation_z, world);
              return -1.F;
            }
            c->move(lim_x, lim_y, offsetZ, orientation_z, world);
            return 0.F;
          }
          else {
            return -1.F;
          }
        }
        else {
          if(next_map->tryMove(c, next_y, next_x)) {
            c->move(next_x, next_y, offsetZ, dest_orientation, world);
          }
          else {
            return -1.F;
          }
        }
        break;
      case MAPLINK_BOUNCE:
      case MAPLINK_BACK:
        if((next_dx == low_limit || next_dx == high_limit) && link->map1 == this && link->x1 == next_x + x_direction && link->y1 == next_y) {
          next_map = link->map2;
          removeCharacter(c);
          link->map2->addCharacter(c);
          next_x = link->x2 - x_direction;
          next_y = link->y2;
          // BOUNCE orientation_z
          diff = (orientation_z > 270 || orientation_z < 90 ? 180 - orientation_z : orientation_z);
        }
        else if((next_dx == low_limit || next_dx == high_limit) && link->map2 == this && link->x2 == next_x + x_direction && link->y2 == next_y) {
          next_map = link->map1;
          removeCharacter(c);
          link->map1->addCharacter(c);
          next_x = link->x1 - x_direction;
          next_y = link->y1;
          // BOUNCE orientation_z
          diff = (orientation_z > 270 || orientation_z < 90 ? 180 - orientation_z : orientation_z);
        }
        else if((next_dy == low_limit || next_dy == high_limit) && link->map1 == this && link->y1 == next_y + y_direction && link->x1 == next_x) {
          next_map = link->map2;
          removeCharacter(c);
          link->map2->addCharacter(c);
          next_x = link->x2;
          next_y = link->y2 - y_direction;
          // BOUNCE orientation_z
          diff = (orientation_z > 180 ? 270 - orientation_z : 90 - orientation_z);
        }
        else if((next_dy == low_limit || next_dy == high_limit) && link->map2 == this && link->y2 == next_y + y_direction && link->x2 == next_x) {
          next_map = link->map1;
          removeCharacter(c);
          link->map1->addCharacter(c);
          next_x = link->x1;
          next_y = link->y1 - y_direction;
          // BOUNCE orientation_z
          diff = (orientation_z > 180 ? 270 - orientation_z : 90 - orientation_z);
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
        if(link->type == MAPLINK_BOUNCE) {
          dest_orientation = 180 + orientation_z + 2.F * diff;
        }
        else if(link->type == MAPLINK_BACK) {
          dest_orientation = orientation_z + 180.F;
        }
        while(dest_orientation >= 360.F) {
          dest_orientation -= 360.F;
        }
        if(next_x < next_map->offsetX || std::floor(next_x) >= next_map->sizeX + next_map->offsetX || next_y < next_map->offsetY || std::floor(next_y) >= next_map->sizeY + next_map->offsetY || next_map->getBlock(next_x, next_y)->solid || (!c->isFlying() && next_map->getBlock(next_x, next_y)->unwalkable)) {
          if(tryMove(c, lim_x, lim_y)) {
            if(MathUtil::distance(c->getX(), c->getY(), lim_x, lim_y) > MathUtil::distance(c->getX(), c->getY(), destX, destY)) {
              c->move(destX, destY, offsetZ, orientation_z, world);
              return -1.F;
            }
            c->move(lim_x, lim_y, offsetZ,orientation_z, world);
            return low_limit;
          }
          else {
            return -1.F;
          }
        }
        else {
          if(next_map->tryMove(c, next_x, next_y)) {
            c->move(next_x, next_y, offsetZ, dest_orientation, world);
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
    if(tryMove(c, next_x, next_y)) {
      if(MathUtil::distance(c->getX(), c->getY(), next_x, next_y) > MathUtil::distance(c->getX(), c->getY(), destX, destY)) {
        c->move(destX, destY, offsetZ, orientation_z, world);
        return -1.F;
      }
      if(next_x == c->getX() && next_y == c->getY()) {
        return -1.F;
      }
      c->move(next_x, next_y, offsetZ,orientation_z, world);
      return 0.F;
    }
    else {
      return -1.F;
    }
  }
}

float Map::actProjectile(Projectile * p, Adventure * adventure, float speed) {
  bool to_destroy = false;
  if(!p->isLost() && p->getTarget()->type == TARGET_CHARACTER && adventure->getCharacter(p->getTarget()->id)->getCurrentMap()->id == id) {
    p->setOrientationZ(MathUtil::getOrientationToTarget(p->getX(), p->getY(), p->getTarget()->x, p->getTarget()->y));
  }
  float x = MathUtil::round(std::cos(p->getOrientation().z * M_PI / 180.F) * speed + p->getX());
  float y = MathUtil::round(std::sin(p->getOrientation().z * M_PI / 180.F) * speed + p->getY());
  float max_x;
  float max_y;
  MapLink * link = nullptr;
  std::list<MathUtil::Pair> path = MathUtil::getPathFromOrientation(p->getX(), p->getY(), p->getOrientation().z, p->size , speed);
  for(MathUtil::Pair pair : path) {
    if(pair.x < offsetX || pair.x >= sizeX + offsetX || pair.y < offsetY || pair.y >= sizeY + offsetY || getBlock(pair.x, pair.y)->solid) {
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
      if(!c->isMarkedDead() && c->getY() <=  y + c->getSize() + p->getSize() && c->getY() >= y - c->getSize() - p->getSize()) {
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
      if(!c->isMarkedDead() && c->getX() <= x + c->getSize() + p->getSize() && c->getX() >= x - c->getSize() - p->getSize()) {
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
    float tan = std::tan(p->getOrientation().z * M_PI / 180.F);
    for(Character * c : characters) {
      if(!c->isMarkedDead() && ((c->getX() <= x + c->getSize() + p->getSize() && c->getX() >= p->getX() - c->getSize() - p->getSize()) ||
        (c->getX() <= p->getX() + c->getSize() + p->getSize() && c->getX() >= x - c->getSize() - p->getSize()) )) {
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
    return MathUtil::distance(p->getX(), p->getY(), a->getX(), a->getY()) <
      MathUtil::distance(p->getX(), p->getY(), b->getX(), b->getY());
  };
  targets.sort(rangeSort);
  for(Character * character : targets) {
    if(!character->isMarkedDead() && !p->isLost() && p->getArea() > 0 && p->getTarget() == nullptr &&
      MathUtil::distance(p->getX(), p->getY(), character->getX(), character->getY())
      >= MathUtil::distance(p->getX(), p->getY(), p->getDestX(), p->getDestY()) ) {
        // exploding on targeted zone when other targets where found
        p->move(p->getDestX(), p->getDestY(), offsetZ, p->getOrientation().z);
        p->attack(nullptr, characters, adventure);
      }
    p->attack(character, characters, adventure);
    if(p->noDamage()) {
      break;
    }
  }
  if(targets.empty() && !p->isLost() && p->getArea() > 0 && p->getTarget() == nullptr &&
    MathUtil::distance(p->getX(), p->getY(), x, y)
    >= MathUtil::distance(p->getX(), p->getY(), p->getDestX(), p->getDestY()) ) {
      // exploding on targeted zone when no other target where found
      p->move(p->getDestX(), p->getDestY(), offsetZ, p->getOrientation().z);
      p->attack(nullptr, characters, adventure);
  }
  std::list<Character *> tokill = std::list<Character *>();
  for(Character * c : characters) {
    if(!c->isMarkedDead() && !c->isAlive()) {
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
    float range = MathUtil::distance(x, y, p->getX(), p->getY());
    Map * next_map;
    float orientation_z = p->getOrientation().z;
    float dest_orientation = orientation_z;
    float diff = 0.F;
    int32_t x_direction = 1;
    int32_t y_direction = 1;
    if(orientation_z > 180.F) {
      y_direction = -1;
    }
    if(orientation_z > 90.F && orientation_z < 270.F) {
      x_direction = -1;
    }
    float dx = MathUtil::round(x - std::floor(x));
    float dy = MathUtil::round(y - std::floor(y));
    x = std::floor(x);
    y = std::floor(y);
    float high_limit = MathUtil::round(0.999F - p->getSize());
    float low_limit = MathUtil::round(p->getSize());
    switch(link->type) {
      case MAPLINK_THROUGH:
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
        if(x < next_map->offsetX || std::floor(x) >= next_map->sizeX + next_map->offsetX || y < next_map->offsetY || std::floor(y) >= next_map->sizeY + next_map->offsetY || next_map->getBlock(x, y)->solid) {
          p->move(old_x, old_y, offsetZ, orientation_z);
          return 0.F;
        }
        else {
          p->move(x, y, offsetZ, dest_orientation);
        }
        break;
      case MAPLINK_BOUNCE:
      case MAPLINK_BACK:
        if((dx == low_limit || dx == high_limit) && link->map1 == this && link->x1 == x + x_direction && link->y1 == y) {
          next_map = link->map2;
          x = link->x2 - x_direction;
          y = link->y2;
          // BOUNCE orientation_z
          diff = (orientation_z > 270 || orientation_z < 90 ? 180 - orientation_z : orientation_z);
        }
        else if((dx == low_limit || dx == high_limit) && link->map2 == this && link->x2 == x + x_direction && link->y2 == y) {
          next_map = link->map1;
          x = link->x1 - x_direction;
          y = link->y1;
          // BOUNCE orientation_z
          diff = (orientation_z > 270 || orientation_z < 90 ? 180 - orientation_z : orientation_z);
        }
        else if((dy == low_limit || dy == high_limit) && link->map1 == this && link->y1 == y + y_direction && link->x1 == x) {
          next_map = link->map2;
          x = link->x2;
          y = link->y2 - y_direction;
          // BOUNCE orientation_z
          diff = (orientation_z > 180 ? 270 - orientation_z : 90 - orientation_z);
        }
        else if((dy == low_limit || dy == high_limit) && link->map2 == this && link->y2 == y + y_direction && link->x2 == x) {
          next_map = link->map1;
          x = link->x1;
          y = link->y1 - y_direction;
          // BOUNCE orientation_z
          diff = (orientation_z > 180 ? 270 - orientation_z : 90 - orientation_z);
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
        if(link->type == MAPLINK_BOUNCE) {
          dest_orientation = 180 + orientation_z + 2.F * diff;
        }
        else if(link->type == MAPLINK_BACK) {
          dest_orientation = orientation_z + 180.F;
        }
        while(dest_orientation >= 360.F) {
          dest_orientation -= 360.F;
        }
        if(x < next_map->offsetX || std::floor(x) >= next_map->sizeX + next_map->offsetX || y < next_map->offsetY || std::floor(y) >= next_map->sizeY + next_map->offsetY || next_map->getBlock(x, y)->solid) {
          p->move(old_x, old_y, offsetZ, orientation_z);
          return 0.F;
        }
        else {
          p->move(x + dx, y + dy, offsetZ, dest_orientation);
        }
        break;
    }
    return speed - range;
  }
  p->move(x, y, offsetZ, p->getOrientation().z);
  return 0.F;
}

*/
void Map::clearProjectiles() {
  projectiles.clear();
}

std::string Map::block_to_string(int32_t x, int32_t y) {
  std::stringstream * ss = new std::stringstream();
  String::insert_long(ss, id);
  String::insert_int(ss, x);
  String::insert_int(ss, y);
  String::insert(ss, getBlock(MathUtil::Vector3i(x, y, 0))->name);
  std::string result = ss->str();
  delete ss;
  return result;
}
