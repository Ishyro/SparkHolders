#ifndef _ETHERAL_CASTER_AI_H_
#define _ETHERAL_CASTER_AI_H_

#include "Values.h"
#include "data/Settings.h"

#include "ai/AI.h"

class EtheralCasterAI : public AI {
  using AI::AI;
  public:
    Action * getActions(Adventure * adventure, Character * c);
  private:
    bool casted = false;
    int32_t current_round = 0;
    int32_t target_round = Settings::getWeekDurationInRound();
};

#endif // _ETHERAL_CASTER_AI_H_
