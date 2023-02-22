#ifndef _ROAMER_AI_H_
#define _ROAMER_AI_H_

#include "Values.h"

#include "ai/AI.h"

class RoamerAI : public AI {
  using AI::AI;
  public:
    Action * getActions(Adventure * adventure, Character * c);
};

#endif // _ROAMER_AI_H_
