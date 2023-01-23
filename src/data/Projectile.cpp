#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/Projectile.h"

#include "utils/String.h"
#include "utils/MapUtil.h"

void Projectile::initializeEffects(std::list<Effect *> effects, int overcharge_power, int overcharge_duration) {
  for(Effect * effect : effects) {
    Effect * toadd = new Effect(effect, overcharge_power, overcharge_duration);
    this->effects.push_back(toadd);
  }
}

int Projectile::getX() { return x; }
int Projectile::getY() { return y; }
float Projectile::getDX() { return dx; }
float Projectile::getDY() { return dy; }
int Projectile::getDestX() {
  if(target != nullptr) {
    return target->getX();
  }
  else {
    return dest_x;
  }
}
int Projectile::getDestY() {
  if(target != nullptr) {
    return target->getY();
  }
  else {
    return dest_y;
  }
}
float Projectile::getDestDX() {
  if(target != nullptr) {
    return target->getDX();
  }
  else {
    return dest_dx;
  }
}
float Projectile::getDestDY() {
  if(target != nullptr) {
    return target->getDY();
  }
  else {
    return dest_dy;
  }
}

float Projectile::getOrientation() { return orientation; }
bool Projectile::isLost() { return lost; }
int Projectile::getDamageFromType(int damage_type) { return damages[damage_type]; }
int Projectile::getRawDamage() {
  int power = 0;
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += current_damages[i];
  }
  return power;
}
float Projectile::getSpeed() { return speed; }
float Projectile::getArea() { return area; }
float Projectile::getSize() { return size; }
int Projectile::getFalloffTimer() { return falloff_timer; }
float Projectile::getWastePerTick() { return waste_per_tick; }
float Projectile::getWastePerArea() { return waste_per_area; }
float Projectile::getWastePerHit() { return waste_per_hit; }
Character * Projectile::getTarget() { return target; }
Character * Projectile::getOwner() { return owner; }

bool Projectile::isAtDest() {
  return x == getDestX() && y == getDestY();
}

int Projectile::getLight() {
  int light = 0;
  for(Effect * effect : effects) {
    if(effect->type == LIGHT) {
      light += effect->power;
    }
  }
  return light;
}

Skill * Projectile::getSkill() { return skill; }
std::list<Effect *> Projectile::getEffects() { return effects; }

bool Projectile::noDamage() { return getRawDamage() <= 0; }

void Projectile::setX(int x) { this->x = x; }
void Projectile::setY(int y) { this->y = y; }
void Projectile::setDX(float dx) { this->dx = dx; }
void Projectile::setDY(float dy) { this->dy = dy; }
void Projectile::setDestX(int dest_x) { this->dest_x = dest_x; }
void Projectile::setDestY(int dest_y) { this->dest_y = dest_y; }
void Projectile::setOrientation(float orientation) { this->orientation = orientation; }
void Projectile::setSpeed(float speed) { this->speed = speed; }
void Projectile::setArea(float area) { this->area = area; }
void Projectile::setFalloffTimer(int falloff_timer) { this->falloff_timer = falloff_timer; }
void Projectile::setWastePerTick(float waste_per_tick) { this->waste_per_tick = waste_per_tick; }
void Projectile::setWastePerArea(float waste_per_area) { this->waste_per_area = waste_per_area; }
void Projectile::setWastePerHit(float waste_per_hit) { this->waste_per_hit = waste_per_hit; }
void Projectile::setTarget(Character * target) { this->target = target; }
void Projectile::setOwner(Character * owner) { this->owner = owner; }
void Projectile::setLost(bool state) { lost = state; }

void::Projectile::move(int y, int x, float dy, float dx) {
  this->x = x;
  this->y = y;
  this->dx = dx;
  this->dy = dy;
}

void Projectile::reduceDamageTick() {
  if(current_timer++ >= falloff_timer) {
    for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
      current_damages[damage_type] = (int) std::max(0., ceil( (float) current_damages[damage_type] - (float) damages[damage_type] * waste_per_tick));
    }
  }
}

void Projectile::reduceDamageHit() {
  for(int damage_type = 0; damage_type < DAMAGE_TYPE_NUMBER; damage_type++) {
    current_damages[damage_type] = (int) std::max(0., ceil( (float) current_damages[damage_type] - (float) damages[damage_type] * waste_per_hit));
  }
}

void Projectile::attack(Character * target, std::list<Character *> characters, Adventure * adventure) {
  if(target != nullptr && (target->getTeam() == owner->getTeam() || alreadyHit(target))) {
    return;
  }
  if(target == this->target) {
    setLost(true);
  }
  if(area == 0.F) {
    target->receiveAttack(current_damages, orientation);
    if(skill != nullptr) {
      skill->activate(owner, target, adventure, overcharge_power, overcharge_duration, overcharge_range);
    }
  }
  else {
    for(Character * c : characters) {
      if(c != nullptr && !(c->isEtheral() || c->isInvulnerable())) {
        float range;
        if(target == nullptr) {
          // exploding on targeted zone
          range = MapUtil::distance(x, y, dx, dy, c->getX(), c->getY(), c->getDX(), c->getDY());
        }
        else {
          range = MapUtil::distance(target->getX(), target->getY(), target->getDX(), target->getDY(), c->getX(), c->getY(), c->getDX(), c->getDY());
        }
        if(range <= area - c->getSize()) {
          int reducedDamages[DAMAGE_TYPE_NUMBER];
          for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
            reducedDamages[i] = (int) ceil( ((float) current_damages[i]) * pow(1 - waste_per_area, range));
          }
          target->receiveAttack(reducedDamages, 360.F);
          if(skill != nullptr) {
            skill->activate(owner, target, adventure, overcharge_power, overcharge_duration, overcharge_range);
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

std::string Projectile::to_string(int offsetY, int offsetX) {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, projectile_type);
  String::insert_float(ss, size);
  String::insert_int(ss, x - offsetX);
  String::insert_int(ss, y - offsetY);
  String::insert_float(ss, dx);
  String::insert_float(ss, dy);
  String::insert_float(ss, orientation);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_int(ss, current_damages[i]);
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
  String::insert_int(ss, projectile_type);
  String::insert_float(ss, size);
  String::insert_bool(ss, homing);
  String::insert_int(ss, current_map_id);
  String::insert_int(ss, x);
  String::insert_int(ss, y);
  String::insert_float(ss, dx);
  String::insert_float(ss, dy);
  String::insert_int(ss, dest_x);
  String::insert_int(ss, dest_y);
  String::insert_float(ss, dest_dx);
  String::insert_float(ss, dest_dy);
  if(skill != nullptr) {
    String::insert(ss, skill->to_string());
  } else {
    String::insert(ss, "none");
  }
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  String::insert_float(ss, orientation);
  String::insert_float(ss, speed);
  String::insert_float(ss, area);
  String::insert_int(ss, overcharge_power);
  String::insert_int(ss, overcharge_duration);
  String::insert_int(ss, overcharge_range);
  String::insert_int(ss, falloff_timer);
  String::insert_float(ss, waste_per_tick);
  String::insert_float(ss, waste_per_area);
  String::insert_float(ss, waste_per_hit);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_int(ss, damages[i]);
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

std::string Projectile::ammo_to_string(Ammunition * ammo) {
  std::stringstream * ss = new std::stringstream();
  if(ammo != nullptr) {
    String::insert(ss, ammo->projectile->full_to_string());
    String::insert_int(ss, ammo->number);
    String::insert_int(ss, ammo->gold_value);
    String::insert_int(ss, ammo->ammo_type);
  }
  else {
    String::insert(ss, "none");
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
  display->x = String::extract_int(ss);
  display->y = String::extract_int(ss);
  display->dx = String::extract_float(ss);
  display->dy = String::extract_float(ss);
  display->orientation = String::extract_float(ss);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    display->damages[i] = String::extract_int(ss);
  }
  display->speed = String::extract_float(ss);
  display->area = String::extract_float(ss);
  display->waste_per_tick = String::extract_float(ss);
  display->waste_per_area = String::extract_float(ss);
  display->waste_per_hit = String::extract_float(ss);
  delete ss;
  return display;
}

Projectile * Projectile::full_from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int projectile_type = String::extract_int(ss);
  float size = String::extract_float(ss);
  bool homing = String::extract_bool(ss);
  int current_map_id = String::extract_int(ss);
  int x = String::extract_int(ss);
  int y = String::extract_int(ss);
  float dx = String::extract_float(ss);
  float dy = String::extract_float(ss);
  int dest_x = String::extract_int(ss);
  int dest_y = String::extract_int(ss);
  float dest_dx = String::extract_float(ss);
  float dest_dy = String::extract_float(ss);
  std::string skill_str = String::extract(ss);
  Skill * skill = nullptr;
  if(skill_str != "none") {
    skill = Skill::from_string(skill_str);
  }
  std::stringstream * ss_effects = new std::stringstream(String::extract(ss));
  std::list<Effect *> * effects = new std::list<Effect *>();
  while(ss_effects->rdbuf()->in_avail() != 0) {
    effects->push_back(Effect::from_string(String::extract(ss_effects)));
  }
  delete ss_effects;
  Character * target = nullptr;
  Character * owner = nullptr;
  float orientation = String::extract_float(ss);
  float speed = String::extract_float(ss);
  float area = String::extract_float(ss);
  int overcharge_power = String::extract_int(ss);
  int overcharge_duration = String::extract_int(ss);
  int overcharge_range = String::extract_int(ss);
  int falloff_timer = String::extract_int(ss);
  float waste_per_tick = String::extract_float(ss);
  float waste_per_area = String::extract_float(ss);
  float waste_per_hit = String::extract_float(ss);
  int damages[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] = String::extract_int(ss);
  }
  delete ss;
  return new Projectile(
    name,
    projectile_type,
    size,
    homing,
    current_map_id,
    x,
    y,
    dx,
    dy,
    dest_x,
    dest_y,
    dest_dx,
    dest_dy,
    skill,
    *effects,
    target,
    owner,
    orientation,
    speed,
    area,
    overcharge_power,
    overcharge_duration,
    overcharge_range,
    falloff_timer,
    waste_per_tick,
    waste_per_area,
    waste_per_hit,
    damages
  );
}

Ammunition * Projectile::ammo_from_string(std::string to_read) {
  Ammunition * ammo = nullptr;
  std::stringstream * ss = new std::stringstream(to_read);
  std::string first_message = String::extract(ss);
  if(first_message != "none") {
    ammo = new Ammunition();
    ammo->projectile = Projectile::full_from_string(first_message);
    ammo->number = String::extract_int(ss);
    ammo->gold_value = String::extract_int(ss);
    ammo->ammo_type = String::extract_int(ss);
  }
  delete ss;
  return ammo;
}
