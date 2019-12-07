#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <list>
#include <string>

#include "src/Values.h"
#include "src/data/Effect.h"

namespace weapon {
  static long id_cpt = 0;
}

class Weapon {
  public:
    const std::string name;
    const long id = ++weapon::id_cpt;
    const bool melee;
    const int range; // 0 if not melee
    const int type;
    const int weight;
    const int damage_type;
    const int power;
    const int gold_value;
    const int capacity;
    const int ammo_price;
    const std::list<Effect *> effects;
    Weapon(std::string name);
    int getCurrentCapacity();
    bool useAmmo();
  private:
    int current_capacity;
};

#endif // _WEAPON_H_
