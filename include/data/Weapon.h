#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <list>
#include <string>

#include "Values.h"

#include "data/Effect.h"

#include "utils/FileOpener.h"

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
    Weapon(
      std::string name,
      bool melee,
      int range,
      int type,
      int weight,
      int gold_value,
      bool use_ammo,
      int ammo_type,
      int damages[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      melee(melee),
      range(range),
      type(type),
      weight(weight),
      gold_value(gold_value),
      use_ammo(use_ammo),
      ammo_type(ammo_type)
    {
      current_capacity=0;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
      }
    }
    int getCurrentCapacity();
    int getRawDamage();
    int getDamageType(int damage_type);
  private:
    int current_capacity;
    int damages[DAMAGE_TYPE_NUMBER];
};

#endif // _WEAPON_H_
