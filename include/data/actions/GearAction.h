#ifndef _GEAR_ACTION_H_
#define _GEAR_ACTION_H_

#include "data/actions/Action.h"

#include "Values.h"

class GearAction : public Action {
    using Action::Action;
  public:
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
    GearPiece * getGearPiece();
    void setGearPiece(GearPiece * piece);
  private:
    GearPiece * piece;
};

#endif // _GEAR_ACTION_H_
