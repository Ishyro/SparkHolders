#ifndef _DIURNAL_AGRESSIVE_AI_H_
#define _DIURNAL_AGRESSIVE_AI_H_

#include "Values.h"

#include "ai/AI.h"

class DiurnalAgressiveAI : public AI {
  using AI::AI;
  public:
    Action * getAction(Adventure * adventure, Character * c);
};

#endif // _DIURNAL_AGRESSIVE_AI_H_
