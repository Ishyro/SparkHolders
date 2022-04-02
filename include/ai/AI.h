#ifndef _AI_H_
#define _AI_H_

#include "Values.h"

class AI {
  public:
    virtual Action * getAction(Adventure * adventure, Character * c) = 0;
};

#endif // _AI_H_
