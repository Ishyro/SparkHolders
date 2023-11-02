#ifndef _ITEM_H_
#define _ITEM_H_

#include <list>
#include <string>

#include "Values.h"

namespace item {
  static long id_cpt = 0;
}

class Item {
  public:
    const std::string name;
    const long id;
    const int type;
    const int type2;
    const int tier;
    const int max_tier;
    const float weight;
    const int sizeX;
    const int sizeY;
    const int gold_value;
    const bool droppable;
    const bool usable;
    const bool consumable;
    const int use_time;
    const std::list<Effect *> effects;

    Item(
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
      name(name),
      id(id),
      type(type),
      type2(type2),
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
    Item(const Item * item, int tier):
      name(item->name),
      id(++item::id_cpt),
      type(item->type),
      type2(item->type2),
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
    static Item * init(const Item * item, int tier, int number);
    static Item * from_string(std::string to_read, Adventure * adventure);
    bool operator == (const Item& i) const { return id == i.id; }
    bool operator != (const Item& i) const { return !operator==(i); }
};

#endif // _ITEM_H_
