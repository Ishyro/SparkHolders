#ifndef _ITEM_H_
#define _ITEM_H_

#include <list>
#include <string>

#include "Values.h"

namespace item {
  static int64_t id_counter = 0;
}

class Item {
  public:
    const std::string name;
    const int64_t id;
    const int32_t type;
    const int32_t subtype;
    const int32_t tier;
    const int32_t max_tier;
    const float weight;
    const int32_t sizeX;
    const int32_t sizeY;
    const int32_t gold_value;
    const bool droppable;
    const bool usable;
    const bool consumable;
    const int32_t use_time;
    const std::list<Effect *> effects;

    Item(
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
      std::list<Effect *> effects
    ):
      name(name),
      id(id),
      type(type),
      subtype(subtype),
      tier(tier),
      max_tier(max_tier),
      weight(weight),
      sizeX(sizeX),
      sizeY(sizeY),
      gold_value(gold_value),
      droppable(droppable),
      usable(usable),
      consumable(consumable),
      use_time(use_time),
      effects(effects)
    {}
    Item(const Item * item, int32_t tier):
      name(item->name),
      id(++item::id_counter),
      type(item->type),
      subtype(item->subtype),
      tier(std::min(tier, item->max_tier)),
      max_tier(item->max_tier),
      weight(item->weight),
      sizeX(item->sizeX),
      sizeY(item->sizeY),
      gold_value(item->gold_value),
      droppable(item->droppable),
      usable(item->usable),
      consumable(item->consumable),
      use_time(item->use_time),
      effects(item->effects)
    {}
    virtual std::string to_string() = 0;
    bool isFood();
    virtual float getWeight() = 0;
    static Item * init(const Item * item, int32_t tier, int32_t number);
    static Item * from_string(std::string to_read, Adventure * adventure);
    bool operator == (const Item& i) const { return id == i.id; }
    bool operator != (const Item& i) const { return !operator==(i); }
};

#endif // _ITEM_H_
