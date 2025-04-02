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
      float orientation_z,
      float orientation_x = 90.F
    ):
      Action(
        type,
        adventure,
        previous,
        user
      ),
      orientation_z(orientation_z),
      orientation_x(orientation_x)
    {}
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
    float getOrientationZ();
  protected:
    float orientation_z;
    float orientation_x;
};

#endif // _OIRENTED_ACTION_H_
