#include "data/actions/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "ai/AI.h"

#include "ai/PlayerAI.h"

Action * PlayerAI::getActions(Adventure * adventure, Character * c) {
  Action * result;
  result = nullptr;
  c->setCurrentAction(result);
  return result;
}
