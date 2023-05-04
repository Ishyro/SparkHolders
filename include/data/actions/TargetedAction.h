#ifndef _MOVE_ACTION_H_
#define _MOVE_ACTION_H_

#include "data/actions/Action.h"

#include "Values.h"

class TargetedAction : public Action {
    using Action::Action;
  public:
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
    Target * getTarget();
    void setTarget(Target * target);
    void setUserOrientationToTarget(Adventure * adventure);
    float rangeFromTarget(Adventure * adventure);
  protected:
    Target * target;
};

#endif // _MOVE_ACTION_H_
