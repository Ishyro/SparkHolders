#include "data/Character.h"
#include "data/Map.h"
#include "data/Adventure.h"

#include "data/items/Item.h"

#include "communication/SpeechManager.h"

#include "data/Quest.h"

void Quest::getCurrentStep() { SpeechManager::add(steps.front()->todo); }
bool Quest::stepDone(Adventure * adventure) {
  Step * current_step = steps.front();
  bool result = false;
  switch(current_step->type) {
    case QUEST_SLAY:
      result = current_step->target != nullptr;
      break;
    case QUEST_OBTAIN:
      result = current_step->goal_item != nullptr;
      break;
    case QUEST_DISCOVER:
      /*
      for(Character * c : adventure->getParty()) {
        if(c->getCurrentMap()->id == current_step->discover_map_id) {
          result = true;
          break;
        }
      }
      */
      return true;
      break;
    case QUEST_TALK:
      // TODO
      break;
  }
  if(result) {
    SpeechManager::add(current_step->when_done);
    steps.pop_front();
  }
  return result;
}
bool Quest::questDone() { return steps.empty(); }
