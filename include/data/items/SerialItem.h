#ifndef _SERIAL_ITEM_H_
#define _SERIAL_ITEM_H_

#include <list>
#include <string>

#include "data/items/Item.h"

#include "Values.h"

class SerialItem : public Item {
  public:
    const int32_t max;
    SerialItem(
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
      int32_t number,
      int32_t max
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
      number(number),
      max(max)
    {}
    SerialItem(SerialItem * item, int32_t tier, int32_t number):
      Item(item, tier),
      max(item->max),
      number(number)
    {}
    int32_t getNumber();
    int32_t add(int32_t number);
    int32_t reduce(int32_t number);
    float getWeight();
    std::string to_string();
  protected:
    int32_t number;
};

#endif // _SERIAL_ITEM_H_
