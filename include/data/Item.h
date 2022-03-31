#ifndef _LOOT_H_
#define _LOOT_H_

#include <list>
#include <string>

#include "Values.h"

#include "data/Effect.h"

#include "utils/FileOpener.h"

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
    const int gold_value;
    const std::list<Effect *> effects;
    Item(
      std::string name,
      bool equipable,
      bool consumable,
      int type,
      int gold_value,
      std::list<Effect *> effects,
      float damage_reductions[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      equipable(equipable),
      consumable(consumable),
      type(type),
      gold_value(gold_value),
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
      gold_value(item->gold_value),
      effects(item->effects)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damage_reductions[i] = item->damage_reductions[i];
      }
    }
    float getDamageReduction(int damage_type);
  private:
    static long id_cpt;
    float damage_reductions[DAMAGE_TYPE_NUMBER];
};

#endif // _LOOT_H_
