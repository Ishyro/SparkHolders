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
    const float damage_reductions[DAMAGE_TYPE_NUMBER];
    const bool consumable;
    const int type;
    const int gold_value;
    const std::list<Effect *> effects;
    Item(std::string name) {
      // TODO INI_READER
    }
    float getDamageReduction(int damage_type);
  private:
    static long id_cpt;
};

#endif // _LOOT_H_
