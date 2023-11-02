#ifndef _AMMUNITION_ITEM_H_
#define _AMMUNITION_ITEM_H_

#include <list>
#include <string>

#include "data/items/SerialItem.h"

#include "Values.h"

class AmmunitionItem : public SerialItem {
  public:
    AmmunitionItem(
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
      int number,
      int max,
      Projectile * projectile
    ):
      SerialItem(
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
        effects,
        number,
        max
      ),
      projectile(projectile)
    {}
    AmmunitionItem(AmmunitionItem * item, int tier, int number):
      SerialItem(item, tier, number),
      projectile(item->projectile)
    {}
    Projectile * getProjectile();
    std::string to_string();
  private:
    Projectile * projectile;
};

#endif // _AMMUNITION_ITEM_H_
