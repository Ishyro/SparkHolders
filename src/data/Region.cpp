#include "data/items/Gear.h"

#include "data/Region.h"

#include "data/Settings.h"

namespace hitbox {
  bool left_to_right(const HitboxOwner * h1, const HitboxOwner * h2) { return h1->angle < h2->angle; }
  bool right_to_left(const HitboxOwner * h1, const HitboxOwner * h2) { return h1->angle > h2->angle; }
  bool front_to_back(const HitboxOwner * h1, const HitboxOwner * h2) { return h1->distance < h2->distance; }
  bool back_to_front(const HitboxOwner * h1, const HitboxOwner * h2) { return h1->distance > h2->distance; }
}

void Region::addCharacter(Character * character) {
  MathUtil::Vector3 coord = character->getCoord();
  int32_t a = (int32_t) std::floor( (coord.x - (float) id.x) / (float) CHUNK_SIZE) + 1;
  int32_t b = (int32_t) std::floor( (coord.y - (float) id.y) / (float) CHUNK_SIZE) + 1;
  int32_t c = (int32_t) std::floor( (coord.z - (float) id.z) / (float) CHUNK_SIZE) + 1;
  int32_t chunk_indice = a + b * 3 + c * 9;
  if(chunk_indice >= 0 && chunk_indice < 27) {
    chunks[a + b * 3 + c * 9]->addCharacter(character);
    character->setRegion(this);
  }
}

bool Region::removeCharacter(Character * character) {
  MathUtil::Vector3 coord = character->getCoord();
  int32_t a = (int32_t) std::floor( (coord.x - (float) id.x) / (float) CHUNK_SIZE) + 1;
  int32_t b = (int32_t) std::floor( (coord.y - (float) id.y) / (float) CHUNK_SIZE) + 1;
  int32_t c = (int32_t) std::floor( (coord.z - (float) id.z) / (float) CHUNK_SIZE) + 1;
  int32_t chunk_indice = a + b * 3 + c * 9;
  if(chunk_indice >= 0 && chunk_indice < 27) {
    bool no_characters_left = chunks[a + b * 3 + c * 9]->removeCharacter(character);
    // 13 is the central chunk
    if(chunk_indice == 13) {
      return no_characters_left;
    }
  }
  return false;
}


void Region::addShield(Shield * shield) {
  // always middle chunk
  chunks[13]->addShield(shield);
}

void Region::removeShield(Shield * shield) {
  // always middle chunk
  chunks[13]->removeShield(shield);
}

Block * Region::getBlock(MathUtil::Vector3 coord) {
  return getBlock(MathUtil::Vector3i(coord));
}

Block * Region::getBlock(MathUtil::Vector3i coord) {
  int32_t a = (int32_t) std::floor( (float) (coord.x - id.x) / (float) CHUNK_SIZE) + 1;
  int32_t b = (int32_t) std::floor( (float) (coord.y - id.y) / (float) CHUNK_SIZE) + 1;
  int32_t c = (int32_t) std::floor( (float) (coord.z - id.z) / (float) CHUNK_SIZE) + 1;
  int32_t chunk_indice = a + b * 3 + c * 9;
  return chunk_indice >= 0 && chunk_indice < 27 ? chunks[a + b * 3 + c * 9]->getBlock(coord) : nullptr;
}

bool Region::tryMove(Character * c, MathUtil::Vector3 dest) {
  if(c->isEtheral()) {
    return true;
  }
  // check on foot
  Block * block = getBlock(dest);
  if(block != nullptr && block->type == BLOCK_SOLID) {
    return false;
  }
  // check on sizeZ
  for(int i = 1; i < c->getSize().z; i++) {
    MathUtil::Vector3 vec_up = MathUtil::Vector3(dest.x, dest.y, dest.z + i);
    Block * up = getBlock(vec_up);
    if(up != nullptr && up->type != BLOCK_LIQUID && up->type != BLOCK_GAS) {
      return false;
    }
  }
  MathUtil::HitboxOBB * hitbox = new MathUtil::HitboxOBB(c->getHitbox());
  hitbox->applyMove(dest, MathUtil::getDirectionFromOrientation(c->getOrientation()));
  for(Character * other : getCharacters()) {
    if(!other->isMarkedDead() && c != other && !other->isEtheral() && MathUtil::collide(hitbox, other->getHitbox())) {
      return false;
    }
  }
  for(Furniture * furniture : getFurnitures()) {
    if(furniture->getSolid() && MathUtil::collide(hitbox, furniture->getHitbox())) {
      return false;
    }
  }
  return true;
}

void Region::move(Character * character, World * world) {
  MathUtil::Vector3 current = character->getCoord();
  MathUtil::Vector3 speed = character->getSpeed();
  Block * under = getBlock(MathUtil::Vector3i(current.x, current.y, current.z - 0.01));
  if(under == nullptr) {
    // TODO slow down and gravity
    speed.z -= 9.0F * Settings::getTickRate().count() / 1000.F;
  }
  // speed is not 0
  if(MathUtil::distanceSquare(MathUtil::Vector3(0.F, 0.F, 0.F), speed) >= 0.01F) {
    MathUtil::Vector3 dest = MathUtil::round(MathUtil::Vector3(
      current.x + speed.x * Settings::getTickRate().count() / 1000.F,
      current.y + speed.y * Settings::getTickRate().count() / 1000.F,
      current.z + speed.z * Settings::getTickRate().count() / 1000.F
    ));
    float distance = MathUtil::distance(current, dest);
    MathUtil::Vector3 direction = MathUtil::Vector3((dest.x - current.x) / distance, (dest.y - current.y) / distance, (dest.z - current.z) / distance);
    for(float step = 0.1F; step < distance; step += 0.1F) {
      MathUtil::Vector3 coord = MathUtil::Vector3(current.x + step * direction.x, current.y + step * direction.y, current.z + step * direction.z);
      Block * block = getBlock(coord);
      if(block != nullptr && block->type != BLOCK_LIQUID && block->type != BLOCK_GAS) {
        // collide
        std::array<float, DAMAGE_TYPE_NUMBER> damages = {0.F};
        damages[DAMAGE_BLUNT] = 0.01F * distance * distance * character->getGear()->getWeight();
        if(damages[DAMAGE_BLUNT] >= 5.F) {
          character->receiveDamages(damages, nullptr, 0.5F);
        }
        speed.z = 0.F;
        // TODO find closest place to move
        if(block->type == BLOCK_SOLID) {
          if(direction.z < 0.F) {
            dest.z = std::ceil(coord.z);
          }
          else {
            dest.x = coord.x - direction.x;
            dest.y = coord.y - direction.y;
            dest.z = coord.z - direction.z;
          }
        }
        else {
          // correct coords on slope / stairs
          dest = getCoordsOnSlope(dest, block->orientation_z);
        }
        break;
      }
    }
    if(tryMove(character, dest)) {
      MathUtil::Vector3 orientation = MathUtil::getOrientationFromDirection(speed);
      orientation.x = 90.F;
      character->move(dest, orientation, world);
    }
    character->setSpeed(MathUtil::Vector3(0, 0, speed.z));
  }
}

void Region::setSpeed(Character * character, MathUtil::Vector3 orientation) {
  float orientation_z = orientation.z;
  float orientation_x;
  MathUtil::Vector3 current = character->getCoord();
  Block * inside = getBlock(MathUtil::Vector3i(current.x, current.y, current.z));
  // TODO, with this we can walk on water
  if(character->isFlying() || (inside != nullptr && inside->type == BLOCK_LIQUID)) {
    orientation_x = orientation.x;
  }
  else {
    if(inside != nullptr && (inside->type == BLOCK_STAIRS || inside->type == BLOCK_SLOPE)) {
      orientation_x = getOrientationXOnSlope(orientation_z, inside->orientation_z);
    }
    else {
      orientation_x = 90.F;
    }
  }
  float cos_long = std::cos(orientation_z * M_PI / 180.F);
  float sin_long = std::sin(orientation_z * M_PI / 180.F);
  float cos_colat = std::cos(orientation_x * M_PI / 180.F);
  float sin_colat = std::sin(orientation_x * M_PI / 180.F);
  //
  float factor_x = sin_colat * cos_long;
  float factor_y = sin_colat * sin_long;
  float factor_z = cos_colat;
  MathUtil::Vector3 speed = character->getSpeed();
  if(character->isFlying()) {
    float range_with_cost = character->getMovementTimeModifier() * 2.F;
    speed.x += factor_x * range_with_cost;
    speed.y += factor_y * range_with_cost;
    speed.z += factor_z * range_with_cost;
    speed = MathUtil::round(speed);
  }
  else {
    Block * under = getBlock(MathUtil::Vector3i(current.x, current.y, current.z - 0.01)); 
    float range_with_cost;
    if(under != nullptr) {
      range_with_cost = character->getMovementTimeModifier() * under->speed;
      speed.x += factor_x * range_with_cost;
      speed.y += factor_y * range_with_cost;
      speed.z += factor_z * range_with_cost;
    }
    else {
      range_with_cost = character->getMovementTimeModifier() * 2.F;
      speed.x += factor_x * range_with_cost;
      speed.y += factor_y * range_with_cost;
      speed.z += factor_z * range_with_cost;
    }
    speed = MathUtil::round(speed);
  }
  character->setSpeed(speed);
}

MathUtil::Vector3 Region::getCoordsOnSlope(MathUtil::Vector3 dest, int32_t block_orientation) {
  MathUtil::Vector3 result = MathUtil::Vector3(dest.x, dest.y, std::floor(dest.z));
  switch(block_orientation) {
    case 0:
      result.z = MathUtil::round(result.z + (dest.x - std::floor(dest.x)));
      break;
    case 90:
      result.z = MathUtil::round(result.z + (dest.y - std::floor(dest.y)));
      break;
    case 180:
      result.z = MathUtil::round(result.z + 1 - (dest.x - std::floor(dest.x)));
      break;
    case 270:
      result.z = MathUtil::round(result.z + 1 - (dest.y - std::floor(dest.y)));
      break;
  }
  return result;
}

float Region::getOrientationXOnSlope(float character_orientation, int32_t block_orientation) {
  int32_t x_direction = 1;
  int32_t y_direction = 1;
  if(character_orientation > 180.F) {
    y_direction = -1;
  }
  if(character_orientation > 90.F && character_orientation < 270.F) {
    x_direction = -1;
  }
  switch(block_orientation) {
    case 0:
      return x_direction == 1 ? 45.F : 135.F;
    case 180:
      return x_direction == -1 ? 45.F : 135.F;
    case 90:
      return y_direction == 1 ? 45.F : 135.F;
    case 270:
      return y_direction == -1 ? 45.F : 135.F;
    // no happening
    default:
      return 0.F;
  }
}

bool Region::canSee(Character * watcher, Character * target) {
  if(target->isInvisible() || target->isEtheral()) {
    return false;
  }
  MathUtil::Vector3 watcher_head = watcher->getCoord();
  watcher_head.z += watcher->getSize().z;
  MathUtil::Vector3 target_head = target->getCoord();
  target_head.z += target->getSize().z;
  float distance = MathUtil::distance(watcher_head, target_head);
  MathUtil::Vector3 direction = MathUtil::Vector3((target_head.x - watcher_head.x) / distance, (target_head.y - watcher_head.y) / distance, (target_head.z - watcher_head.z) / distance);
  for(int32_t step = 1; step < distance; step++) {
    MathUtil::Vector3 coord = MathUtil::Vector3(watcher_head.x + step * direction.x, watcher_head.y + step * direction.y, watcher_head.z + step * direction.z);
    Block * block = getBlock(coord);
    if(block != nullptr && block->opaque) {
      return false;
    }
  }
  return true;
}

std::list<Character *> Region::getCharacters() {
  std::list<Character *> result = std::list<Character *>();
  for(BlocksChunk * chunk : chunks) {
    for(Character * character : chunk->getCharacters()) {
      result.push_front(character);
    }
  }
  return result;
}

std::list<Character *> Region::getCharacters(Character * character) {
  std::list<Character *> result = std::list<Character *>();
  for(Character * c : getCharacters()) {
    if(canSee(character, c)) {
      result.push_back(c);
    }
  }
  return result;
}

std::list<Furniture *> Region::getFurnitures() {
  std::list<Furniture *> result = std::list<Furniture *>();
  for(BlocksChunk * chunk : chunks) {
    for(Furniture * furniture : chunk->getFurnitures()) {
      result.push_front(furniture);
    }
  }
  return result;
}

std::list<Furniture *> Region::getFurnitures(Character * character) {
  std::list<Furniture *> result = std::list<Furniture *>();
  for(BlocksChunk * chunk : chunks) {
    for(Furniture * furniture : chunk->getFurnitures()) {
      result.push_front(furniture);
    }
  }
  return result;
}

std::list<Shield *> Region::getShields() {
  std::list<Shield *> result = std::list<Shield *>();
  for(BlocksChunk * chunk : chunks) {
    for(Shield * shield : chunk->getShields()) {
      result.push_front(shield);
    }
  }
  return result;
}

std::map<const MathUtil::Vector3i, Block *> Region::getBlocks() {
  std::map<const MathUtil::Vector3i, Block *> result = std::map<const MathUtil::Vector3i, Block *>();
  for(BlocksChunk * chunk : chunks) {
    for(auto pair : chunk->getBlocks()) {
      result.insert(pair);
    }
  }
  return result;
}

Furniture * Region::getFurniture(MathUtil::Vector3i coord) {
  for(BlocksChunk * chunk : chunks) {
    for(Furniture * furniture : chunk->getFurnitures()) {
      if(furniture->getCoord() == coord) {
        return furniture;
      }
    }
  }
  return nullptr;
}

std::list<HitboxOwner *> Region::sortHitboxes(Attack * attack) {
  std::list<HitboxOwner *> result = std::list<HitboxOwner *>();
  for(Character * other : getCharacters()) {
    if(attack->owner != other && MathUtil::collide(attack->hitbox, other->getHitbox())) {
      HitboxOwner * hitbox = new HitboxOwner();
      hitbox->type = HITBOX_OWNER_CHARACTER;
      hitbox->owner = other;
      switch(attack->hit_order) {
        case ATTACK_LEFT_TO_RIGHT:
        case ATTACK_RIGHT_TO_LEFT:
          hitbox->angle = MathUtil::getOrientationToTarget(attack->hitbox->origin - other->getHitbox()->origin);
          break;
        case ATTACK_FRONT_TO_BACK:
        case ATTACK_BACK_TO_FRONT:
          hitbox->distance = MathUtil::distanceSquare(attack->hitbox->origin, other->getHitbox()->origin);
          break;
      }
      result.push_back(hitbox);
    }
  }
  for(Shield * shield : getShields()) {
    if(attack->owner != shield->owner && MathUtil::collide(attack->hitbox, shield->hitbox)) {
      HitboxOwner * hitbox = new HitboxOwner();
      hitbox->type = HITBOX_OWNER_SHIELD;
      hitbox->owner = shield->owner;
      switch(attack->hit_order) {
        case ATTACK_LEFT_TO_RIGHT:
        case ATTACK_RIGHT_TO_LEFT:
          hitbox->angle = MathUtil::getOrientationToTarget(attack->hitbox->origin - shield->hitbox->origin);
          break;
        case ATTACK_FRONT_TO_BACK:
        case ATTACK_BACK_TO_FRONT:
          hitbox->distance = MathUtil::distanceSquare(attack->hitbox->origin, shield->hitbox->origin);
          break;
      }
      result.push_back(hitbox);
    }
  }
  for(Furniture * furniture : getFurnitures()) {
    if(MathUtil::collide(attack->hitbox, furniture->getHitbox())) {
      HitboxOwner * hitbox = new HitboxOwner();
      hitbox->type = HITBOX_OWNER_FURNITURE;
      hitbox->owner = nullptr;
      switch(attack->hit_order) {
        case ATTACK_LEFT_TO_RIGHT:
        case ATTACK_RIGHT_TO_LEFT:
          hitbox->angle = MathUtil::getOrientationToTarget(attack->hitbox->origin - furniture->getHitbox()->origin);
          break;
        case ATTACK_FRONT_TO_BACK:
        case ATTACK_BACK_TO_FRONT:
          hitbox->distance = MathUtil::distanceSquare(attack->hitbox->origin, furniture->getHitbox()->origin);
          break;
      }
      result.push_back(hitbox);
    }
  }
  switch(attack->hit_order) {
    case ATTACK_LEFT_TO_RIGHT:
      result.sort(hitbox::left_to_right);
      break;
    case ATTACK_RIGHT_TO_LEFT:
      result.sort(hitbox::right_to_left);
      break;
    case ATTACK_FRONT_TO_BACK:
      result.sort(hitbox::front_to_back);
      break;
    case ATTACK_BACK_TO_FRONT:
      result.sort(hitbox::back_to_front);
      break;
  }
  return result;
}
