#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <string>
#include <list>
#include <cmath>

#include "Values.h"

typedef struct Ammunition {
  Projectile * projectile;
  int number;
  int gold_value;
  int ammo_type;
} Ammunition;

typedef struct ProjectileDisplay {
  std::string name;
  long id;
  int projectile_type;
  float size;
  int x;
  int y;
  float dx;
  float dy;
  float orientation;
  int damages[DAMAGE_TYPE_NUMBER];
  float speed;
  float area;
  float waste_per_tick;
  float waste_per_area;
  float waste_per_hit;
} ProjectileDisplay;

namespace projectile {
  static long id_cpt = 0;
}

class Projectile {
  public:
    const long id = ++projectile::id_cpt;
    const float size;
    const std::string name;
    const int projectile_type;
    const bool homing;
    Projectile(
      const std::string name,
      int projectile_type,
      float size,
      bool homing,
      Skill * skill,
      std::list<Effect *> effects,
      float speed,
      float area,
      int falloff_timer,
      float waste_per_tick,
      float waste_per_area,
      float waste_per_hit,
      int damages[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      projectile_type(projectile_type),
      size(size),
      homing(homing),
      skill(skill),
      effects(effects),
      speed(speed),
      area(area),
      falloff_timer(falloff_timer),
      waste_per_tick(waste_per_tick),
      waste_per_area(waste_per_area),
      waste_per_hit(waste_per_hit),
      previous_targets(std::list<Character *>()),
      current_targets(std::list<Character *>())
    {
      lost = false;
      current_map_id = 0;
      x = 0;
      y = 0;
      dx = 0.F;
      dy = 0.F;
      dest_x = 0;
      dest_y = 0;
      target = nullptr;
      owner = nullptr;
      orientation = 0.F;
      overcharge_power = 1;
      overcharge_duration = 1;
      overcharge_range = 1;
      current_timer = 0;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
        this->current_damages[i] = damages[i];
      }
    }
    Projectile(
      const Projectile * projectile,
      int realDamages[DAMAGE_TYPE_NUMBER],
      int current_map_id,
      int x,
      int y,
      float dx,
      float dy,
      int dest_x,
      int dest_y,
      float dest_dx,
      float dest_dy,
      Character * target,
      Character * owner,
      float orientation,
      int overcharge_power,
      int overcharge_duration,
      int overcharge_range
    ):
      name(projectile->name),
      projectile_type(projectile->projectile_type),
      size(projectile->size),
      homing(projectile->homing),
      current_map_id(current_map_id),
      x(x),
      y(y),
      dx(dx),
      dy(dy),
      dest_x(dest_x),
      dest_y(dest_y),
      dest_dx(dest_dx),
      dest_dy(dest_dy),
      skill(projectile->skill),
      effects(std::list<Effect *>()),
      target(target),
      owner(owner),
      orientation(orientation),
      speed(projectile->speed * overcharge_range),
      area(projectile->area * overcharge_range),
      overcharge_power(overcharge_power),
      overcharge_duration(overcharge_duration),
      overcharge_range(overcharge_range),
      falloff_timer(projectile->falloff_timer),
      current_timer(projectile->current_timer),
      waste_per_tick(projectile->waste_per_tick / overcharge_duration),
      waste_per_area(projectile->waste_per_area / overcharge_range),
      waste_per_hit(projectile->waste_per_hit),
      previous_targets(std::list<Character *>()),
      current_targets(std::list<Character *>())
    {
      lost = false;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = realDamages[i];
        this->current_damages[i] = realDamages[i];
      }
      initializeEffects(projectile->effects, overcharge_power, overcharge_duration);
    }
    Projectile(
      const std::string name,
      int projectile_type,
      float size,
      bool homing,
      int current_map_id,
      int x,
      int y,
      float dx,
      float dy,
      int dest_x,
      int dest_y,
      float dest_dx,
      float dest_dy,
      Skill * skill,
      std::list<Effect *> effects,
      Character * target,
      Character * owner,
      float orientation,
      float speed,
      float area,
      int overcharge_power,
      int overcharge_duration,
      int overcharge_range,
      int falloff_timer,
      float waste_per_tick,
      float waste_per_area,
      float waste_per_hit,
      int damages[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      projectile_type(projectile_type),
      size(size),
      homing(homing),
      current_map_id(current_map_id),
      x(x),
      y(y),
      dx(dx),
      dy(dy),
      dest_x(dest_x),
      dest_y(dest_y),
      dest_dx(dest_dx),
      dest_dy(dest_dy),
      skill(skill),
      effects(effects),
      target(target),
      owner(owner),
      orientation(orientation),
      speed(speed),
      area(area),
      overcharge_power(overcharge_power),
      overcharge_duration(overcharge_duration),
      overcharge_range(overcharge_range),
      falloff_timer(falloff_timer),
      waste_per_tick(waste_per_tick),
      waste_per_area(waste_per_area),
      waste_per_hit(waste_per_hit),
      previous_targets(std::list<Character *>()),
      current_targets(std::list<Character *>())
    {
      lost = false;
      current_timer = 0;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
        this->current_damages[i] = damages[i];
      }
    }
    void initializeEffects(std::list<Effect *> effects, int overcharge_power, int overcharge_duration);
    int getX();
    int getY();
    float getDX();
    float getDY();
    int getDestX();
    int getDestY();
    float getDestDX();
    float getDestDY();
    float getOrientation();
    bool isLost();
    int getRawDamage();
    int getDamageFromType(int damage_type);
    float getSpeed();
    float getArea();
    float getSize();
    int getFalloffTimer();
    float getWastePerTick();
    float getWastePerArea();
    float getWastePerHit();
    int getLight();
    Skill * getSkill();
    std::list<Effect *> getEffects();
    Character * getTarget();
    Character * getOwner();
    bool isAtDest();
    bool noDamage();
    void setX(int setX);
    void setY(int setY);
    void setDX(float setDX);
    void setDY(float setDY);
    void setDestX(int destX);
    void setDestY(int destY);
    void setOrientation(float orientation);
    void setSpeed(float speed);
    void setArea(float area);
    void setFalloffTimer(int falloff_timer);
    void setWastePerTick(float waste_per_tick);
    void setWastePerArea(float waste_per_area);
    void setWastePerHit(float waste_per_hit);
    void setTarget(Character * target);
    void setOwner(Character * owner);
    void setLost(bool state);
    void move(int y, int x, float dy, float dx);
    void reduceDamageTick();
    void reduceDamageHit();
    void attack(Character * target, std::list<Character *> characters, Adventure * adventure);
    bool alreadyHit(Character * c);
    void clearOldTargets();
    std::string to_string(int offsetY, int offsetX);
    std::string full_to_string();
    static std::string ammo_to_string(Ammunition * ammo);
    static ProjectileDisplay * from_string(std::string to_read);
    static Projectile * full_from_string(std::string to_read);
    static Ammunition * ammo_from_string(std::string to_read);
    bool operator == (const Projectile& p) const { return id == p.id; }
    bool operator != (const Projectile& p) const { return !operator==(p); }
  private:
    int current_map_id;
    int x;
    int y;
    float dx;
    float dy;
    int target_type;
    int dest_x;
    int dest_y;
    float dest_dx;
    float dest_dy;
    bool lost;
    Skill * skill;
    std::list<Effect *> effects;
    Character * target;
    Character * owner;
    float orientation;
    float speed;
    float area;
    int overcharge_power;
    int overcharge_duration;
    int overcharge_range;
    int falloff_timer;
    int current_timer;
    float waste_per_tick;
    float waste_per_area;
    float waste_per_hit;
    int damages[DAMAGE_TYPE_NUMBER];
    int current_damages[DAMAGE_TYPE_NUMBER];
    std::list<Character *> previous_targets;
    std::list<Character *> current_targets;
    void nextOrientation(Map * map);
};

#endif // _PROJECTILE_H_
