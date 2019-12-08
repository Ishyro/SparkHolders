#ifndef _LOOT_H_
#define _LOOT_H_

#include <list>

#include "src/data/Item.h"
#include "src/data/Weapon.h"

class Loot {
  public:
    const int type;
    const long x;
    const long y;
    const long gold;
    const std::list<Weapon *> weapons;
    const std::list<Item *> items;
    Loot(int type, long x, long y, long gold, std::list<Weapon *> weapons, std::list<Item *> items):
      type(type),
      x(x),
      y(y),
      gold(gold),
      weapons(weapons),
      items(items) {}
};

#endif // _LOOT_H_
