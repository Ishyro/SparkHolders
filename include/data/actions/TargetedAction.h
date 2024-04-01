#ifndef _MOVE_ACTION_H_
#define _MOVE_ACTION_H_

#include "data/actions/Action.h"

#include "Values.h"

class TargetedAction : public Action {
  public:
    TargetedAction(
      const int32_t type,
      Adventure * adventure,
      const Action * previous,
      Character * user,
      Target * target
    ):
      Action(
        type,
        adventure,
        previous,
        user
      ),
      target(target)
    {}
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
    Target * getTarget();
    void setUserOrientationToTarget(Adventure * adventure);
    float rangeFromTarget(Adventure * adventure);
  protected:
    Target * target;
};

#endif // _MOVE_ACTION_H_
