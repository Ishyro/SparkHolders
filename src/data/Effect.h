#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <string>

#include "src/Values.h"
#include "src/data/Character.h"

class Effect {
  public:
    const std::string name;
    const int type;
    const int power;
    const int duration;
    Effect(std::string name);
    bool decrTick(); // true if tick_left == 0
    void activate(Character * target);
  private:
    int tick_left;
};

#endif // _EFFECT_H_
