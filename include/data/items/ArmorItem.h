#ifndef _ARMOR_ITEM_H_
#define _ARMOR_ITEM_H_

#include <list>
#include <string>
#include <array>

#include "data/items/GearItem.h"

#include "Values.h"

class ArmorItem : public GearItem {
  public:
    ArmorItem(
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
      int32_t max_durability,
      int32_t durability,
      std::array<float, DAMAGE_TYPE_NUMBER> damages
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
        swap_time,
        max_durability,
        durability
      )
    {
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
      }
    }
    ArmorItem(ArmorItem * item, int32_t tier):
      GearItem(item, tier)
    {
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages[i] = item->damages[i];
      }
    }
    float getDamageReductionFromType(int32_t damage_type);
    std::string to_string();
  private:
    std::array<float, DAMAGE_TYPE_NUMBER> damages;
};

#endif // _ARMOR_ITEM_H_
