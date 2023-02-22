#ifndef _PLAYER_AI_H_
#define _PLAYER_AI_H_

#include "Values.h"

#include "ai/AI.h"

class PlayerAI : public AI {
  public:
    Action * getActions(Adventure * adventure, Character * c);
};

#endif // _PLAYER_AI_H_
