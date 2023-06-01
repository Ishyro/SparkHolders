#ifndef _SERIAL_ITEM_H_
#define _SERIAL_ITEM_H_

#include <list>
#include <string>

#include "data/items/Item.h"

#include "Values.h"

class SerialItem : public Item {
  public:
    SerialItem(
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
      int number
    ):
      Item(
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
        effects
      ),
      number(number)
    {}
    SerialItem(SerialItem * item, int tier, int number):
      Item(item, tier),
      number(number)
    {}
    bool isFood();
    int getNumber();
    int add(int number);
    int reduce(int number);
    std::string to_string();
  protected:
    int number;
};

#endif // _SERIAL_ITEM_H_
