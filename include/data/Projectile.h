#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <string>
#include <list>
#include <cmath>

#include "Values.h"

typedef struct ProjectileDisplay {
  std::string name;
  long id;
  int projectile_type;
  float size;
  float x;
  float y;
  float z;
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
    const long id;
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
      id(0),
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
      x = 0.F;
      y = 0.F;
      z = 0.F;
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
      World * world,
      int current_map_id,
      Target * target,
      Character * owner,
      int overcharge_power,
      int overcharge_duration,
      int overcharge_range,
      bool change_owner_orientation
    ):
      name(projectile->name),
      id(++projectile::id_cpt),
      projectile_type(projectile->projectile_type),
      size(projectile->size),
      homing(projectile->homing),
      current_map_id(current_map_id),
      skill(projectile->skill),
      effects(std::list<Effect *>()),
      target(target),
      owner(owner),
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
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = realDamages[i];
        this->current_damages[i] = realDamages[i];
      }
      init(projectile->effects, overcharge_power, overcharge_duration, world, false, change_owner_orientation);
    }
    Projectile(
      const Projectile * projectile,
      int realDamages[DAMAGE_TYPE_NUMBER],
      World * world,
      int current_map_id,
      float x,
      float y,
      float z,
      Target * target,
      Character * owner,
      int overcharge_power,
      int overcharge_duration,
      int overcharge_range,
      bool change_owner_orientation
    ):
      name(projectile->name),
      id(++projectile::id_cpt),
      projectile_type(projectile->projectile_type),
      size(projectile->size),
      homing(projectile->homing),
      current_map_id(current_map_id),
      x(x),
      y(y),
      z(z),
      skill(projectile->skill),
      effects(std::list<Effect *>()),
      target(target),
      owner(owner),
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
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = realDamages[i];
        this->current_damages[i] = realDamages[i];
      }
      init(projectile->effects, overcharge_power, overcharge_duration, world, true, change_owner_orientation);
    }
    Projectile(
      const std::string name,
      long id,
      int projectile_type,
      float size,
      bool homing,
      int current_map_id,
      float x,
      float y,
      float z,
      Skill * skill,
      std::list<Effect *> effects,
      Target * target,
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
      id(id),
      projectile_type(projectile_type),
      size(size),
      homing(homing),
      current_map_id(current_map_id),
      x(x),
      y(y),
      z(z),
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
    void init(std::list<Effect *> effects, int overcharge_power, int overcharge_duration, World * world, bool teleport, bool change_owner_orientation);
    int getCurrentMapId();
    float getX();
    float getY();
    float getZ();
    float getDestX();
    float getDestY();
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
    Target * getTarget();
    Character * getOwner();
    bool isAtDest();
    bool noDamage();
    void setX(float setX);
    void setY(float setY);
    void setOrientation(float orientation);
    void setSpeed(float speed);
    void setArea(float area);
    void setFalloffTimer(int falloff_timer);
    void setWastePerTick(float waste_per_tick);
    void setWastePerArea(float waste_per_area);
    void setWastePerHit(float waste_per_hit);
    void setTarget(Target * target);
    void setOwner(Character * owner);
    void setLost(bool state);
    void markDestroyed();
    void move(float x, float y, float z, float orientation, World * world);
    void reduceDamageTick();
    void reduceDamageHit();
    void attack(Character * target, std::list<Character *> characters, Adventure * adventure);
    bool alreadyHit(Character * c);
    void clearOldTargets();
    std::string to_string();
    std::string full_to_string();
    static ProjectileDisplay * from_string(std::string to_read);
    static Projectile * full_from_string(std::string to_read, Adventure * adventure);
    bool operator == (const Projectile& p) const { return id == p.id; }
    bool operator != (const Projectile& p) const { return !operator==(p); }
  private:
    int current_map_id;
    float x;
    float y;
    float z;
    int target_type;
    bool lost;
    Skill * skill;
    std::list<Effect *> effects;
    Target * target;
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
};

#endif // _PROJECTILE_H_
