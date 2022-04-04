#ifndef _AI_H_
#define _AI_H_

#include "Values.h"

class AI {
  public:
    AI(){}
    AI(long x, long y):origin_x(x),origin_y(y){}
    virtual Action * getAction(Adventure * adventure, Character * c) = 0;
    static int getFleeOrientation(Character * self, long x, long y);
    static int getFollowOrientation(Character * self, long x, long y);
  protected:
    long origin_x;
    long origin_y;
};

#endif // _AI_H_
