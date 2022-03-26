#include "data/Event.h"

void Event::activate(Adventure * adventure) { 
  for (auto map : maps) { 
    adventure->getWorld()->addMap(map);
  }
  for (auto link : links) { 
    adventure->getWorld()->addMapLink(link);
  }
  switch (character_apparition_type) { 
    case SOFT:
      for (auto c : characters) { 
        adventure->softMoveCharacterToMap(c, c->getCurrentMapId(), c->getX(), c->getY());
      }
      break;
    case HARD:
      for (auto c : characters) { 
        adventure->softMoveCharacterToMap(c, c->getCurrentMapId(), c->getX(), c->getY());
      }
      break;
  }
  if (hasQuest) { 
    adventure->addQuest(quest);
  }
  for (auto e : effects) { 
    for (auto c : adventure->getParty()) { 
      c->addEffect(e);
    }
  }
  SpeechManager::add(speech);
}
