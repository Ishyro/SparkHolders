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
    const int type;
    Action(
      const int type,
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
    int getTick();
    int getTime();
    Action * getPrevious();
    Action * getNext();
    void setTick(int tick);
    void setTime(int time);
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
