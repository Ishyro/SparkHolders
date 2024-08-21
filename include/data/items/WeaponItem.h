#ifndef _WEAPON_ITEM_H_
#define _WEAPON_ITEM_H_

#include <list>
#include <string>

#include "data/items/GearItem.h"

#include "Values.h"

class WeaponItem : public GearItem {
  public:
    const float range;
    const int32_t strike_time;
    const float status_power;
    const bool use_projectile;
    const bool use_ammo;
    const int32_t ammo_type;
    const int32_t capacity;
    const int32_t reload_time;
    WeaponItem(
      std::string name,
      int64_t id,
      int32_t type,
      int32_t subtype,
      int32_t tier,
      int32_t max_tier,
      float weight,
      int32_t sizeX,
      int32_t sizeY,
      int32_t gold_value,
      bool droppable,
      bool usable,
      bool consumable,
      int32_t use_time,
      std::list<Effect *> effects,
      int32_t swap_time,
      float range,
      int32_t strike_time,
      float status_power,
      bool use_projectile,
      bool use_ammo,
      int32_t ammo_type,
      int32_t capacity,
      int32_t reload_time,
      AmmunitionItem * ammo,
      std::array<float, DAMAGE_TYPE_NUMBER> damages,
      std::array<float, DAMAGE_TYPE_NUMBER> damage_reductions
    ):
      GearItem(
        name,
        id,
        type,
        subtype,
        tier,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        effects,
        swap_time
      ),
      range(range),
      strike_time(strike_time),
      status_power(status_power),
      use_projectile(use_projectile),
      use_ammo(use_ammo),
      ammo_type(ammo_type),
      capacity(capacity),
      reload_time(reload_time),
      ammo(ammo)
    {
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
      }
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damage_reductions[i] = damage_reductions[i];
      }
    }
    WeaponItem(WeaponItem * item, int32_t tier):
      GearItem(item, tier),
      range(item->range),
      strike_time(item->strike_time),
      status_power(item->status_power),
      use_projectile(item->use_projectile),
      use_ammo(item->use_ammo),
      ammo_type(item->ammo_type),
      capacity(item->capacity),
      ammo(item->ammo),
      reload_time(item->reload_time)
    {
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages[i] = item->damages[i];
      }
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damage_reductions[i] = item->damage_reductions[i];
      }
    }
    float getRawDamage();
    float getDamageFromType(int32_t damage_type);
    float getDamageReductionFromType(int32_t damage_type);
    int32_t getCurrentCapacity();
    AmmunitionItem * getAmmo();
    void useAmmo();
    AmmunitionItem * reload(AmmunitionItem * ammo);
    std::string to_string();
  private:
    AmmunitionItem * ammo;
    std::array<float, DAMAGE_TYPE_NUMBER> damages;
    std::array<float, DAMAGE_TYPE_NUMBER> damage_reductions;
};

#endif // _WEAPON_ITEM_H_
