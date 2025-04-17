#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/Projectile.h"

#include "util/String.h"
#include "util/MathUtil.h"

void::Projectile::init(std::list<Effect *> effects, float overcharge, int32_t overcharge_duration, World * world, bool teleport, bool change_owner_orientation) {
  for(Effect * effect : effects) {
    Effect * toadd = new Effect(effect, overcharge, overcharge_duration);
    this->effects.push_back(toadd);
  }
  lost = false;
  orientation.z = world->setPathToTarget(current_map_id, owner->getCoord().x, owner->getCoord().y, target);
  if(change_owner_orientation) {
    owner->setOrientationZ(orientation.z);
  }
  if(!teleport) {
    coord.x = owner->getCoord().x + (owner->getSize().x + size) * std::cos(orientation.z * M_PI / 180.F);
    coord.y = owner->getCoord().y + (owner->getSize().y + size) * std::sin(orientation.z * M_PI / 180.F);
    coord.z = owner->getCoord().z;
  }
}

int32_t Projectile::getCurrentMapId() { return current_map_id; }
MathUtil::Vector3 Projectile::getCoord() { return coord; }
float Projectile::getDestX() {
  return target->coord.x;
}
float Projectile::getDestY() {
  return target->coord.y;
}

MathUtil::Vector3 Projectile::getOrientation() { return orientation; }
bool Projectile::isLost() { return lost; }
int32_t Projectile::getDamageFromType(int32_t damage_type) { return damages[damage_type]; }
int32_t Projectile::getRawDamage() {
  int32_t power = 0;
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += current_damages[i];
  }
  return power;
}
float Projectile::getSpeed() { return speed; }
float Projectile::getArea() { return area; }
float Projectile::getSize() { return size; }
int32_t Projectile::getFalloffTimer() { return falloff_timer; }
float Projectile::getWastePerTick() { return waste_per_tick; }
float Projectile::getWastePerArea() { return waste_per_area; }
float Projectile::getWastePerHit() { return waste_per_hit; }
MathUtil::Target * Projectile::getTarget() { return target; }
Character * Projectile::getOwner() { return owner; }

bool Projectile::isAtDest() {
  return coord.x == getDestX() && coord.y == getDestY();
}

int32_t Projectile::getLight() {
  int32_t light = 0;
  for(Effect * effect : effects) {
    if(effect->type == EFFECT_LIGHT) {
      light += effect->power;
    }
  }
  return light;
}

Skill * Projectile::getSkill() { return skill; }
std::list<Effect *> Projectile::getEffects() { return effects; }

bool Projectile::noDamage() { return getRawDamage() <= 0; }

void Projectile::setX(float x) { coord.x = x; }
void Projectile::setY(float y) { coord.y = y; }
void Projectile::setOrientationZ(float orientation_z) { orientation.z = orientation_z; }
void Projectile::setSpeed(float speed) { this->speed = speed; }
void Projectile::setArea(float area) { this->area = area; }
void Projectile::setFalloffTimer(int32_t falloff_timer) { this->falloff_timer = falloff_timer; }
void Projectile::setWastePerTick(float waste_per_tick) { this->waste_per_tick = waste_per_tick; }
void Projectile::setWastePerArea(float waste_per_area) { this->waste_per_area = waste_per_area; }
void Projectile::setWastePerHit(float waste_per_hit) { this->waste_per_hit = waste_per_hit; }
void Projectile::setTarget(MathUtil::Target * target) { this->target = target; }
void Projectile::setOwner(Character * owner) { this->owner = owner; }
void Projectile::setLost(bool state) { lost = state; }
void Projectile::markDestroyed() {
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    current_damages[i] = 0;
  }
}

void::Projectile::move(MathUtil::Vector3 coord, MathUtil::Vector3 orientation) {
  this->coord = coord;
  this->orientation.z = orientation.z;
}

void Projectile::reduceDamageTick() {
  if(current_timer++ >= falloff_timer) {
    for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
      current_damages[damage_type] = (int32_t) std::max(0., ceil( (float) current_damages[damage_type] - (float) damages[damage_type] * waste_per_tick));
    }
  }
}

void Projectile::reduceDamageHit() {
  for(int32_t damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    current_damages[damage_type] = (int32_t) std::max(0., ceil( (float) current_damages[damage_type] - (float) damages[damage_type] * waste_per_hit));
  }
}

void Projectile::attack(Character * target, std::list<Character *> characters, Adventure * adventure) {
  if(target != nullptr && (target->getTeam() == owner->getTeam() || alreadyHit(target))) {
    return;
  }
  if(this->target->type == TARGET_CHARACTER && target == this->target->character) {
    setLost(true);
  }
  if(area == 0.F) {
    target->receiveDamage(current_damages, owner, owner->getStatusPower());
    if(skill != nullptr) {
      MathUtil::Target * t = new MathUtil::Target();
      t->type = TARGET_CHARACTER;
      t->character = target;
      skill->activate(owner, t, adventure, overcharge);
    }
  }
  else {
    for(Character * c : characters) {
      if(c != nullptr && !(c->isEtheral() || c->isInvulnerable())) {
        float range;
        if(target == nullptr) {
          // exploding on targeted zone
          range = MathUtil::distance(coord, c->getCoord());
        }
        else {
          range = MathUtil::distance(target->getCoord(), c->getCoord());
        }
        if(range <= area - c->getSize().x) {
          std::array<float, DAMAGE_TYPE_NUMBER> reducedDamages;
          for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
            reducedDamages[i] = current_damages[i] * pow(1 - waste_per_area, range);
          }
          target->receiveDamage(reducedDamages, owner, owner->getStatusPower());
          if(skill != nullptr) {
            MathUtil::Target * t = new MathUtil::Target();
            t->type = TARGET_CHARACTER;
            t->character = target;
            skill->activate(owner, t, adventure, overcharge);
          }
        }
      }
    }
  }
  if(target != nullptr) {
    current_targets.push_back(target);
  }
  reduceDamageHit();
}

bool Projectile::alreadyHit(Character * c) {
  for(Character * a : previous_targets) {
    if(a == c) {
      return true;
    }
  }
  for(Character * a : current_targets) {
    if(a == c) {
      return true;
    }
  }
  return false;
}

void Projectile::clearOldTargets() {
  previous_targets.clear();
  for(Character * target : current_targets) {
    previous_targets.push_back(target);
  }
  current_targets.clear();
}

std::string Projectile::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, projectile_type);
  String::insert_float(ss, size);
  String::insert_float(ss, coord.x);
  String::insert_float(ss, coord.y);
  String::insert_float(ss, coord.z);
  String::insert_float(ss, orientation.z);
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_float(ss, current_damages[i]);
  }
  String::insert_float(ss, speed);
  String::insert_float(ss, area);
  String::insert_float(ss, waste_per_tick);
  String::insert_float(ss, waste_per_area);
  String::insert_float(ss, waste_per_hit);
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Projectile::full_to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, projectile_type);
  String::insert_float(ss, size);
  String::insert_bool(ss, homing);
  String::insert_int(ss, current_map_id);
  String::insert_float(ss, coord.x);
  String::insert_float(ss, coord.y);
  String::insert_float(ss, coord.z);
  if(skill != nullptr) {
    String::insert(ss, skill->name);
  }
  else {
    String::insert(ss, "none");
  }
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  String::insert_float(ss, orientation.z);
  String::insert_float(ss, speed);
  String::insert_float(ss, area);
  String::insert_int(ss, overcharge);
  String::insert_int(ss, overcharge_duration);
  String::insert_int(ss, overcharge_range);
  String::insert_int(ss, falloff_timer);
  String::insert_float(ss, waste_per_tick);
  String::insert_float(ss, waste_per_area);
  String::insert_float(ss, waste_per_hit);
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_float(ss, damages[i]);
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

ProjectileDisplay * Projectile::from_string(std::string to_read) {
  ProjectileDisplay * display = new ProjectileDisplay();
  std::stringstream * ss = new std::stringstream(to_read);
  display->name = String::extract(ss);
  display->id = String::extract_long(ss);
  display->projectile_type = String::extract_int(ss);
  display->size = String::extract_float(ss);
  display->x = String::extract_float(ss);
  display->y = String::extract_float(ss);
  display->z = String::extract_float(ss);
  display->orientation_z = String::extract_float(ss);
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damages[i] = String::extract_float(ss);
  }
  display->speed = String::extract_float(ss);
  display->area = String::extract_float(ss);
  display->waste_per_tick = String::extract_float(ss);
  display->waste_per_area = String::extract_float(ss);
  display->waste_per_hit = String::extract_float(ss);
  delete ss;
  return display;
}

Projectile * Projectile::full_from_string(std::string to_read, Adventure * adventure) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int64_t id = String::extract_long(ss);
  int32_t projectile_type = String::extract_int(ss);
  float size = String::extract_float(ss);
  bool homing = String::extract_bool(ss);
  int32_t current_map_id = String::extract_int(ss);
  float x = String::extract_float(ss);
  float y = String::extract_float(ss);
  float z = String::extract_float(ss);
  std::string skill_str = String::extract(ss);
  Skill * skill = nullptr;
  if(skill_str != "none") {
    skill = (Skill *) adventure->getDatabase()->getSkill(skill_str);
  }
  std::stringstream * ss_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * effects = new std::list<Effect *>();
  while(ss_effects->rdbuf()->in_avail() != 0) {
    effects->push_back(Effect::from_string(String::extract(ss_effects)));
  }
  delete ss_effects;
  MathUtil::Target * target = nullptr;
  Character * owner = nullptr;
  float orientation_z = String::extract_float(ss);
  float speed = String::extract_float(ss);
  float area = String::extract_float(ss);
  float overcharge = String::extract_int(ss);
  int32_t overcharge_duration = String::extract_int(ss);
  int32_t overcharge_range = String::extract_int(ss);
  int32_t falloff_timer = String::extract_int(ss);
  float waste_per_tick = String::extract_float(ss);
  float waste_per_area = String::extract_float(ss);
  float waste_per_hit = String::extract_float(ss);
  std::array<float, DAMAGE_TYPE_NUMBER> damages;
  for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] = String::extract_float(ss);
  }
  delete ss;
  return new Projectile(
    name,
    id,
    projectile_type,
    size,
    homing,
    current_map_id,
    x,
    y,
    z,
    skill,
    *effects,
    target,
    owner,
    orientation_z,
    speed,
    area,
    overcharge,
    overcharge_duration,
    overcharge_range,
    falloff_timer,
    waste_per_tick,
    waste_per_area,
    waste_per_hit,
    damages
  );
}
