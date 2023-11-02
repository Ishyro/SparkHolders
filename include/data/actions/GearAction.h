#ifndef _GEAR_ACTION_H_
#define _GEAR_ACTION_H_

#include "data/actions/Action.h"

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
    {}
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
  private:
    ItemSlot * slot1;
    ItemSlot * slot2;
};

#endif // _GEAR_ACTION_H_
