#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <string>
#include <cmath>

#include "src/Values.h"
#include "src/data/Effect.h"
#include "src/data/Skill.h"
#include "src/data/Map.h"
#include "src/data/World.h"

#define ALL -1 // for waste

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
    Projectile(int projectile_type, int damages[DAMAGE_TYPE_NUMBER], int speed, float waste_per_hit, float waste_per_tile, int x, int y, int orientation):
      projectile_type(projectile_type),
      speed(speed),
      waste_per_hit(waste_per_hit),
      waste_per_tile(waste_per_tile),
      x(x),
      y(y),
      orientation(orientation) {
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
    int getRawDamage();
    bool noDamage();
    void move();
    void attack_single_target(Character * target, World * world);
    void attack_multiple_targets(Map * map, World * world);
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
