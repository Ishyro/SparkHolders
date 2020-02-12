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
    const int gold_value;
    const bool use_ammo;
    const int ammo_type;
    const std::list<Effect *> effects;
    Weapon(std::string name) {
      // TODO XML_READER
    }
    int getCurrentCapacity();
    int getRawDamage();
    int getDamageType(int damage_type);
  private:
    int current_capacity;
    int damages[DAMAGE_TYPE_NUMBER];
};

#endif // _WEAPON_H_
