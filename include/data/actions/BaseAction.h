#ifndef _BASE_ACTION_H_
#define _BASE_ACTION_H_

#include "data/actions/Action.h"

#include "Values.h"

class BaseAction : public Action {
    using Action::Action;
  public:
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
};

#endif // _BASE_ACTION_H_
