#include "data/Quest.h"

void Quest::getCurrentStep() { SpeechManager::add(steps.front()->todo); }
bool Quest::stepDone(Adventure * adventure) { 
  Step * current_step = steps.front();
  bool result = false;
  switch(current_step->type) { 
    case SLAY:
      result = current_step->target != nullptr;
      break;
    case OBTAIN_ITEM:
      result = current_step->goal_item != nullptr;
      break;
    case OBTAIN_WEAPON:
      result = current_step->goal_weapon != nullptr;
      break;
    case DISCOVER:
      for (auto c : adventure->getParty()) { 
        if (c->getCurrentMapId() == current_step->disover_map_id) { 
          result = true;
          break;
        }
      }
      break;
    case TALK:
      // TODO
      break;
  }
  if (result) { 
    SpeechManager::add(current_step->when_done);
    steps.pop_front();
  }
  return result;
}
bool Quest::questDone() { return steps.empty(); }
