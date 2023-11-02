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
      long id,
      int type,
      int type2,
      int tier,
      int max_tier,
      float weight,
      int sizeX,
      int sizeY,
      int gold_value,
      bool droppable,
      bool usable,
      bool consumable,
      int use_time,
      std::list<Effect *> effects,
      int swap_time,
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
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damage_reductions[i] = damage_reductions[i];
      }
    }
    ArmorItem(ArmorItem * item, int tier):
      GearItem(item, tier)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damage_reductions[i] = item->damage_reductions[i];
      }
    }
    float getDamageReductionFromType(int damage_type);
    std::string to_string();
  private:
    float damage_reductions[DAMAGE_TYPE_NUMBER];
};

#endif // _ARMOR_ITEM_H_
