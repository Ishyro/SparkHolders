#ifndef _WEAPON_ITEM_H_
#define _WEAPON_ITEM_H_

#include <list>
#include <string>

#include "data/items/GearItem.h"

#include "Values.h"

class WeaponItem : public GearItem {
  public:
    const float range;
    const int strike_time;
    const bool use_projectile;
    const bool use_ammo;
    const int ammo_type;
    const int capacity;
    const int reload_time;
    WeaponItem(
      std::string name,
      long id,
      int type,
      int type2,
      int tier,
      int max_tier,
      float weight,
      int gold_value,
      bool droppable,
      bool usable,
      bool consumable,
      int use_time,
      std::list<Effect *> effects,
      int armor,
      int swap_time,
      float range,
      int strike_time,
      bool use_projectile,
      bool use_ammo,
      int ammo_type,
      int capacity,
      int reload_time,
      AmmunitionItem * ammo,
      int damages[DAMAGE_TYPE_NUMBER]
    ):
      GearItem(
        name,
        id,
        type,
        type2,
        tier,
        max_tier,
        weight,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        effects,
        armor,
        swap_time
      ),
      range(range),
      strike_time(strike_time),
      use_projectile(use_projectile),
      use_ammo(use_ammo),
      ammo_type(ammo_type),
      capacity(capacity),
      reload_time(reload_time),
      ammo(ammo)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
      }
    }
    WeaponItem(WeaponItem * item, int tier):
      GearItem(item, tier),
      range(item->range),
      strike_time(item->strike_time),
      use_projectile(item->use_projectile),
      use_ammo(item->use_ammo),
      ammo_type(item->ammo_type),
      capacity(item->capacity),
      ammo(item->ammo),
      reload_time(item->reload_time)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages[i] = item->damages[i];
      }
    }
    int getRawDamage();
    int getDamageFromType(int damage_type);
    int getCurrentCapacity();
    AmmunitionItem * getAmmo();
    void useAmmo();
    AmmunitionItem * reload(AmmunitionItem * ammo);
    std::string to_string();
  private:
    AmmunitionItem * ammo;
    int damages[DAMAGE_TYPE_NUMBER];
};

#endif // _WEAPON_ITEM_H_
