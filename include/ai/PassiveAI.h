#ifndef _PASSIVE_AI_H_
#define _PASSIVE_AI_H_

#include "Values.h"

#include "ai/AI.h"

class PassiveAI : public AI {
  public:
    Action * getAction(Adventure * adventure, Character * c);
};

#endif // _PASSIVE_AI_H_
