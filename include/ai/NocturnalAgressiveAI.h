#ifndef _NOCTURNAL_AGRESSIVE_AI_H_
#define _NOCTURNAL_AGRESSIVE_AI_H_

#include "Values.h"

#include "ai/AI.h"

class NocturnalAgressiveAI : public AI {
  using AI::AI;
  public:
    Action * getAction(Adventure * adventure, Character * c);
};

#endif // _NOCTURNAL_AGRESSIVE_AI_H_
