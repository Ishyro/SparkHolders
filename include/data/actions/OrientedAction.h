#ifndef _ORIENTED_ACTION_H_
#define _ORIENTED_ACTION_H_

#include "data/actions/Action.h"

#include "Values.h"

class OrientedAction : public Action {
  public:
    OrientedAction(
      const int32_t type,
      Adventure * adventure,
      const Action * previous,
      Character * user,
      float orientation
    ):
      Action(
        type,
        adventure,
        previous,
        user
      ),
      orientation(orientation)
    {}
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
    float getOrientation();
  protected:
    float orientation;
};

#endif // _OIRENTED_ACTION_H_
