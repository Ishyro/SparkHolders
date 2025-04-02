#ifndef _TARGETED_ACTION_H_
#define _TARGETED_ACTION_H_

#include "data/actions/Action.h"

#include "Values.h"

class TargetedAction : public Action {
  public:
    TargetedAction(
      const int32_t type,
      Adventure * adventure,
      const Action * previous,
      Character * user,
      MathUtil::Target * target
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
    MathUtil::Target * getTarget();
    void setUserOrientationToTarget(Adventure * adventure);
    float rangeFromTarget(Adventure * adventure);
  protected:
    MathUtil::Target * target;
};

#endif // _TARGETED_ACTION_H_
