#ifndef _LOOT_H_
#define _LOOT_H_

#include <list>
#include <string>

#include "Values.h"

namespace item {
  static long id_cpt = 0;
}

class Item {
  public:
    const std::string name;
    const long id = ++item::id_cpt;
    const bool equipable;
    const bool consumable;
    const int type;
    const bool droppable;
    const float weight;
    const int gold_value;
    const int swap_time;
    const int use_time;
    const std::list<Effect *> effects;
    Item(
      std::string name,
      bool equipable,
      bool consumable,
      int type,
      bool droppable,
      float weight,
      int gold_value,
      int swap_time,
      int use_time,
      std::list<Effect *> effects,
      float damage_reductions[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      equipable(equipable),
      consumable(consumable),
      type(type),
      droppable(droppable),
      weight(weight),
      gold_value(gold_value),
      swap_time(swap_time),
      use_time(use_time),
      effects(effects)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damage_reductions[i] = damage_reductions[i];
      }
    }
    Item(const Item * item):
      name(item->name),
      equipable(item->equipable),
      consumable(item->consumable),
      type(item->type),
      droppable(item->droppable),
      weight(item->weight),
      gold_value(item->gold_value),
      swap_time(item->swap_time),
      use_time(item->use_time),
      effects(item->effects)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damage_reductions[i] = item->damage_reductions[i];
      }
    }
    float getDamageReductionFromType(int damage_type);
    bool isFood();
    std::string to_string();
    static Item * from_string(std::string to_read);
    bool operator == (const Item& i) const { return id == i.id; }
    bool operator != (const Item& i) const { return !operator==(i); }
  private:
    float damage_reductions[DAMAGE_TYPE_NUMBER];
};

#endif // _LOOT_H_
