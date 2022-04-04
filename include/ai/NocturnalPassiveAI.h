#ifndef _NOCTURNAL_PASSIVE_AI_H_
#define _NOCTURNAL_PASSIVE_AI_H_

#include "Values.h"

#include "ai/AI.h"

class NocturnalPassiveAI : public AI {
  using AI::AI;
  public:
    Action * getAction(Adventure * adventure, Character * c);
};

#endif // _NOCTURNAL_PASSIVE_AI_H_
