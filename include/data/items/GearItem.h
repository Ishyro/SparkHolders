#ifndef _GEAR_ITEM_H_
#define _GEAR_ITEM_H_

#include <list>
#include <string>

#include "data/items/Item.h"

#include "Values.h"

class GearItem : public Item {
  public:
    const int swap_time;
    GearItem(
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
      int swap_time
    ):
      Item(
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
        effects
      ),
      swap_time(swap_time)
    {}
    GearItem(GearItem * item, int tier):
      Item(item, tier),
      swap_time(item->swap_time)
    {}
    float getWeight();
    virtual std::string to_string() = 0;
};

#endif // _GEAR_ITEM_H_
