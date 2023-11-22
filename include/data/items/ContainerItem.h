#ifndef _CONTAINER_ITEM_H_
#define _CONTAINER_ITEM_H_

#include <list>

#include "data/items/Item.h"
#include "data/items/GearItem.h"

#include "Values.h"

typedef struct ItemSlot {
  int x;
  int y;
  int slot;
  Item * item;
} ItemSlot;


class ContainerItem : public GearItem {
  public:
    const bool canTakeFrom;
    const bool repercuteWeight;
    const bool isLimited;
    const int limit_type;
    const int contentX;
    const int contentY;

    ContainerItem(
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
      int swap_time,
      const bool canTakeFrom,
      const bool repercuteWeight,
      const bool isLimited,
      const int limit_type,
      const int contentX,
      const int contentY
    ):
      GearItem(name,
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
    ContainerItem(ContainerItem * item, int tier):
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
    bool add(Item * item, int x, int y);
    void add_all(std::list<Item *> items);
    Item * remove(int x, int y);
    bool move(int x1, int y1, int x2, int y2);
    float getWeight();
    std::list<ItemSlot *> getItems();
    std::string to_string();
  private:
    std::list<ItemSlot *> content;
};

#endif // _CONTAINER_ITEM_H_
