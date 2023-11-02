#ifndef _BASIC_ITEM_H_
#define _BASIC_ITEM_H_

#include <list>
#include <string>

#include "data/items/Item.h"

#include "Values.h"

class BasicItem : public Item {
  public:
    BasicItem(
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
      std::list<Effect *> effects
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
      )
    {}
    BasicItem(BasicItem * item, int tier):
      Item(item, tier)
    {}
    float getWeight();
    std::string to_string();
};

#endif // _BASIC_ITEM_H_
