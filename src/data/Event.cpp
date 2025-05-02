#include "data/Character.h"
#include "data/World.h"
#include "data/Adventure.h"
#include "data/Speech.h"

#include "communication/SpeechManager.h"

#include "data/Event.h"

void Event::activate(Adventure * adventure) {
  switch(character_apparition_type) {
    case APPARITION_SOFT:
      for(Character * c : characters) {
        adventure->softMoveCharacterToMap(c, c->getCoord());
      }
      break;
    case APPARITION_HARD:
      for(Character * c : characters) {
        adventure->hardMoveCharacterToMap(c, c->getCoord());
      }
      break;
  }
  if(hasQuest) {
    adventure->addQuest(quest);
  }
  for(Effect * e : effects) {
    for(Character * c : adventure->getParty()) {
      c->addEffect(e);
    }
  }
  SpeechManager::add(speech);
}
