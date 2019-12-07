#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <string>

#include "src/Values.h"
#include "src/data/Effect.h"
#include "src/data/Skill.h"

#define ALL -1 // for waste

namespace projectile {
  static long id_cpt = 0;
}

class Projectile {
  public:
    const long id = ++projectile::id_cpt;
    const int projectile_type;
    const int damage_type;
    Projectile::Projectile(int projectile_type, int damage_type, int strength, int speed, int waste_pet_hit, int waste_per_tile, int x, int y, int orientation):
      projectile_type(projectile_type),
      damage_type(damage_type),
      power(power),
      speed(speed),
      waste_pet_hit(waste_pet_hit),
      waste_per_tile(waste_per_tile),
      x(x),
      y(y),
      orientation(orientation) {}
    int getX();
    int getY();
    int getOrientation();
    int getPower();
    int getSpeed();

    void move(int orientation);
    void move(int x, int y, int orientation);
  private:
    int x;
    int y;
    int orientation;
    int power;
    int speed;
    int waste_per_tile;
    int waste_pet_hit;
};

#endif // _PROJECTILE_H_
