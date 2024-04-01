#ifndef _ARMOR_ITEM_H_
#define _ARMOR_ITEM_H_

#include <list>
#include <string>

#include "data/items/GearItem.h"

#include "Values.h"

class ArmorItem : public GearItem {
  public:
    ArmorItem(
      std::string name,
      int64_t id,
      int32_t type,
      int32_t type2,
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
      float damage_reductions[DAMAGE_TYPE_NUMBER]
    ):
      GearItem(
        name,
        id,
        type,
        type2,
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
      )
    {
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damage_reductions[i] = damage_reductions[i];
      }
    }
    ArmorItem(ArmorItem * item, int32_t tier):
      GearItem(item, tier)
    {
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damage_reductions[i] = item->damage_reductions[i];
      }
    }
    float getDamageReductionFromType(int32_t damage_type);
    std::string to_string();
  private:
    float damage_reductions[DAMAGE_TYPE_NUMBER];
};

#endif // _ARMOR_ITEM_H_
