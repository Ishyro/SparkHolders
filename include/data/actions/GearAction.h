#ifndef _GEAR_ACTION_H_
#define _GEAR_ACTION_H_

#include "data/actions/Action.h"

#include "data/items/ContainerItem.h"

#include "Values.h"

class GearAction : public Action {
  public:
    GearAction(
      const int type,
      Adventure * adventure,
      const Action * previous,
      Character * user,
      ItemSlot * slot1,
      ItemSlot * slot2
    ):
      Action(
        type,
        adventure,
        previous,
        user
      ),
      slot1(slot1),
      slot2(slot2)
    {
      for(ItemSlot * slot : user->getGear()->getItems()) {
        if(slot->slot == slot1->slot && slot->x == slot1->x && slot->y == slot1->y) {
          slot1->item = slot->item;
        }
        else if(slot2 != nullptr && slot->slot == slot2->slot && slot->x == slot2->x && slot->y == slot2->y) {
          slot2->item = slot->item;
        }
      }
    }
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
  private:
    ItemSlot * slot1;
    ItemSlot * slot2;
};

#endif // _GEAR_ACTION_H_
