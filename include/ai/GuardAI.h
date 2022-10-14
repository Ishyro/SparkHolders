#ifndef _GUARD_AI_H_
#define _GUARD_AI_H_

#include "Values.h"

#include "ai/AI.h"

class GuardAI : public AI {
  using AI::AI;
  public:
    Action * getAction(Adventure * adventure, Character * c);
};

#endif // _GUARD_AI_H_
