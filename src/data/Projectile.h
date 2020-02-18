#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <string>
#include <cmath>

#include "src/Values.h"
#include "src/data/Effect.h"
#include "src/data/skills/Skill.h"
#include "src/data/Map.h"
#include "src/data/World.h"

typedef struct Ammunition {
  Projectile * projectile;
  long number;
  long gold_value;
} Ammunition;

namespace projectile {
  static long id_cpt = 0;
}

class Projectile {
  public:
    const long id = ++projectile::id_cpt;
    const int projectile_type;
    const int target_type;
    const bool homing;
    const Skill * skill;
    Projectile(
      int projectile_type,
      int target_type,
      int damages[DAMAGE_TYPE_NUMBER],
      int speed,
      float waste_per_hit,
      float waste_per_tile,
      int x,
      int y,
      int orientation,
      Skill * skill,
      Character * owner,
      Character * target,
      int dest_x,
      int dest_y
    ):
      projectile_type(projectile_type),
      target_type(target_type),
      speed(speed),
      waste_per_hit(waste_per_hit),
      waste_per_tile(waste_per_tile),
      x(x),
      y(y),
      orientation(orientation),
      skill(skill)
      {
        for (int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
          this->damages[i] = damages[i];
        }
      }
    int getX();
    int getY();
    int getDestX();
    int getDestY();
    int getOrientation();
    int getRawDamage();
    int getDamageType(int damage_type);
    int getSpeed();
    bool isAtDest();
    bool noDamage();
    void move();
    void setLost(bool state);
    void attack_single_target(Character * target, Adventure * adventure);
    void attack_multiple_targets(std::list<Character *> characters, Adventure * adventure);
  private:
    int x;
    int y;
    int dest_x;
    int dest_y;
    bool lost;
    Character * target;
    Character * owner;
    int orientation;
    int speed;
    int area;
    int overcharge;
    float waste_per_tile;
    float waste_per_tile_area;
    float waste_per_hit;
    int damages[DAMAGE_TYPE_NUMBER];
    void nextOrientation();
};

#endif // _PROJECTILE_H_
