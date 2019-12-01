#include "src/loader/CharacterLoader.h"
#include "src/data/Class.h"
#include "src/data/Gear.h"
#include "src/data/Effect.h"
#include "src/data/Skill.h"


// orientations

#define NORTH 1
#define NORTH_EAST 2
#define EAST 3
#define SOUTH_EAST 4
#define SOUTH 5
#define SOUTH_WEST 6
#define WEST 7
#define NORTH_WEST 8

// type

#define FIRE 1
#define THUNDER 2
#define AIR 3
#define ARROW 4
#define BULLET 5

// waste

#define ALL -1

class Projectile {
  public:
    const long id;
    const int type;
    const string name;
    const Class class;
    const string race;
    const string sex;
    Projectile(int type, int strength, int speed, int waste_pet_hit, int waste_per_tile, int x, int y, int orientation):type(type),
      strength(strength), speed(speed), waste_pet_hit(waste_pet_hit), waste_per_tile(waste_per_tile), x(x), y(y), orientation(orientation){}
    int getX();
    int getY();
    int getOrientation();
    int getStrength();
    int getSpeed();

    void move(int orientation);
    void move(int x, int y, int orientation);
  private:
    static long id_cpt = 0;
    int x;
    int y;
    int orientation;
    int strength;
    int speed;
    int waste_per_tile;
    int waste_pet_hit;
};
