#ifndef _ACTION_H_
#define _ACTION_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "data/Character.h"

#include "Values.h"

class Action {
  public:
    const int32_t type;
    Action(
      const int32_t type,
      Adventure * adventure,
      const Action * previous,
      Character * user
    ):
      type(type),
      previous((Action *) previous),
      next(nullptr),
      user(user)
    {}
    virtual Action * execute(Adventure * adventure) = 0;
    virtual void computeTime(Adventure * adventure) = 0;
    void computeTick(float tick);
    Character * getUser();
    int32_t getTick();
    int32_t getTime();
    Action * getPrevious();
    Action * getNext();
    void setTick(int32_t tick);
    void setTime(int32_t time);
    void setPrevious(Action * action);
    void setNext(Action * action);
    bool operator < (const Action& a) const { return tick < a.tick; }
  protected:
    float tick;
    float time;
    Character * user;
    Action * previous;
    Action * next;
};

#endif // _ACTION_H_
