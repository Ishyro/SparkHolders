#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <string>
#include <cmath>

#include "Values.h"

typedef struct Ammunition {
  Projectile * projectile;
  long number;
  long gold_value;
  int ammo_type;
} Ammunition;

namespace projectile {
  static long id_cpt = 0;
}

class Projectile {
  public:
    const long id = ++projectile::id_cpt;
    const std::string name;
    const int projectile_type;
    const int target_type;
    const bool homing;
    Projectile(
      const std::string name,
      int projectile_type,
      int target_type,
      bool homing,
      long x,
      long y,
      long dest_x,
      long dest_y,
      Skill * skill,
      Character * target,
      Character * owner,
      int orientation,
      int speed,
      int area,
      long overcharge,
      float waste_per_tile,
      float waste_per_tile_area,
      float waste_per_hit,
      int damages[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      projectile_type(projectile_type),
      target_type(target_type),
      homing(homing),
      x(x),
      y(y),
      dest_x(dest_x),
      dest_y(dest_y),
      skill(skill),
      target(target),
      owner(owner),
      orientation(orientation),
      speed(speed),
      area(area),
      overcharge(overcharge),
      waste_per_tile(waste_per_tile),
      waste_per_tile_area(waste_per_tile_area),
      waste_per_hit(waste_per_hit)
    {
      lost = false;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
      }
    }
    long getX();
    long getY();
    long getDestX();
    long getDestY();
    int getOrientation();
    int getRawDamage();
    int getDamageFromType(int damage_type);
    int getSpeed();
    int getArea();
    Character * getTarget();
    Character * getOwner();
    bool isAtDest();
    bool noDamage();
    void setX(long setX);
    void setY(long setY);
    void setDestX(long destX);
    void setDestY(long destY);
    void setTarget(Character * target);
    void setOwner(Character * owner);
    void move();
    void setLost(bool state);
    void attack_single_target(Character * target, Adventure * adventure);
    void attack_multiple_targets(std::list<Character *> characters, Adventure * adventure);
    std::string to_string();
  private:
    long current_map_id;
    long x;
    long y;
    long dest_x;
    long dest_y;
    bool lost;
    Skill * skill;
    Character * target;
    Character * owner;
    int orientation;
    int speed;
    int area;
    long overcharge;
    float waste_per_tile;
    float waste_per_tile_area;
    float waste_per_hit;
    int damages[DAMAGE_TYPE_NUMBER];
    void nextOrientation();
};

#endif // _PROJECTILE_H_
