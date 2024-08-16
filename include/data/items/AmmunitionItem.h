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
      std::list<Effect *> effects,
      int32_t number,
      int32_t max,
      Projectile * projectile
    ):
      SerialItem(
        name,
        id,
        type,
        subtype,
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
    AmmunitionItem(AmmunitionItem * item, int32_t tier, int32_t number):
      SerialItem(item, tier, number),
      projectile(item->projectile)
    {}
    Projectile * getProjectile();
    std::string to_string();
  private:
    Projectile * projectile;
};

#endif // _AMMUNITION_ITEM_H_
