#ifndef _AI_H_
#define _AI_H_

#include "Values.h"

class AI {
  public:
    AI(){}
    AI(int x, int y):origin_x(x),origin_y(y){}
    virtual Action * getAction(Adventure * adventure, Character * c) = 0;
    static int getFleeOrientation(Adventure * adventure, Character * self, int x, int y);
    static int getFollowOrientation(Adventure * adventure, Character * self, int x, int y);
  protected:
    int origin_x;
    int origin_y;
    bool hungry;
    bool sleepy;
    void selectHungriness(Character * self);
    void selectTiredness(Character * self);
    Action * eat(Adventure * adventure, Character * self);
    Action * trackPrey(Adventure * adventure, Character * self);
};

#endif // _AI_H_
