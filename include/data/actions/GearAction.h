#ifndef _GEAR_ACTION_H_
#define _GEAR_ACTION_H_

#include "data/actions/Action.h"

#include "Values.h"

class GearAction : public Action {
  public:
    const long item_id;
    GearAction(
      const int type,
      Adventure * adventure,
      const Action * previous,
      Character * user,
      const long item_id
    ):
      Action(
        type,
        adventure,
        previous,
        user
      ),
      item_id(item_id)
    {}
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
};

#endif // _GEAR_ACTION_H_
