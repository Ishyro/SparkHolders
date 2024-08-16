#ifndef _CONTAINER_ITEM_H_
#define _CONTAINER_ITEM_H_

#include <list>

#include "data/items/Item.h"
#include "data/items/GearItem.h"

#include "Values.h"

typedef struct ItemSlot {
  int32_t x;
  int32_t y;
  int32_t slot;
  Item * item;
} ItemSlot;


class ContainerItem : public GearItem {
  public:
    const bool canTakeFrom;
    const bool repercuteWeight;
    const bool isLimited;
    const int32_t limit_type;
    const int32_t contentX;
    const int32_t contentY;

    ContainerItem(
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
      int32_t swap_time,
      const bool canTakeFrom,
      const bool repercuteWeight,
      const bool isLimited,
      const int32_t limit_type,
      const int32_t contentX,
      const int32_t contentY
    ):
      GearItem(name,
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
        swap_time
      ),
      canTakeFrom(canTakeFrom),
      repercuteWeight(repercuteWeight),
      isLimited(isLimited),
      limit_type(limit_type),
      contentX(contentX),
      contentY(contentY),
      content(std::list<ItemSlot *>())
    {}
    ContainerItem(ContainerItem * item, int32_t tier):
      GearItem(item, tier),
      canTakeFrom(item->canTakeFrom),
      repercuteWeight(item->repercuteWeight),
      isLimited(item->isLimited),
      limit_type(item->limit_type),
      contentX(item->contentX),
      contentY(item->contentY),
      content(std::list<ItemSlot *>())
    {}
    static bool overlap(ItemSlot * s1, ItemSlot * s2);
    bool add(Item * item, int32_t x, int32_t y);
    void add_all(std::list<Item *> items);
    Item * remove(int32_t x, int32_t y);
    bool move(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
    float getWeight();
    std::list<ItemSlot *> getItems();
    std::string to_string();
  private:
    std::list<ItemSlot *> content;
};

#endif // _CONTAINER_ITEM_H_
